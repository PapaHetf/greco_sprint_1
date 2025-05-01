#include "crypto_guard_ctx.h"

namespace CryptoGuard {

struct AesCipherParams {
    static const size_t KEY_SIZE = 32;             // AES-256 key size
    static const size_t IV_SIZE = 16;              // AES block size (IV length)
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();  // Cipher algorithm

    int encrypt;                              // 1 for encryption, 0 for decryption
    std::array<unsigned char, KEY_SIZE> key;  // Encryption key
    std::array<unsigned char, IV_SIZE> iv;    // Initialization vector
};

class CryptoGuardCtx::Impl {
public:
    using p_evp_cipher_ctx =
        std::unique_ptr<EVP_CIPHER_CTX, decltype([](EVP_CIPHER_CTX *ctx) { EVP_CIPHER_CTX_free(ctx); })>;
    using p_evp_md_ctx = std::unique_ptr<EVP_MD_CTX, decltype([](EVP_MD_CTX *mdctx) { EVP_MD_CTX_free(mdctx); })>;
    // EVP_MD_CTX_new
    Impl() {}

    ~Impl() {}

    void SendCryptoGuardException() {
        char buf[256];
        ERR_error_string_n(ERR_peek_last_error(), buf, 255);
        throw std::runtime_error{std::format("Internal error: {}", buf)};
    }

    AesCipherParams CreateChiperParamsFromPassword(std::string_view password) {
        AesCipherParams params;
        constexpr std::array<unsigned char, 8> salt = {'1', '2', '3', '4', '5', '6', '7', '8'};

        int result = EVP_BytesToKey(params.cipher, EVP_sha256(), salt.data(),
                                    reinterpret_cast<const unsigned char *>(password.data()), password.size(), 1,
                                    params.key.data(), params.iv.data());

        if (result == 0) {
            throw std::runtime_error{"Failed to create a key from password"};
        }

        return params;
    }

    void CipherFile(std::iostream &inStream, std::iostream &outStream, std::string_view password,
                    size_t param_encrypt) {
        if (inStream.fail() || inStream.eof()) {
            throw std::runtime_error("Input file is not good!");
        }

        OpenSSL_add_all_algorithms();

        auto params = CreateChiperParamsFromPassword(password);
        params.encrypt = param_encrypt;

        p_evp_cipher_ctx ctx(EVP_CIPHER_CTX_new());

        if (ctx.get() == nullptr) {
            SendCryptoGuardException();
        }

        // Инициализируем cipher
        if (!EVP_CipherInit_ex(ctx.get(), params.cipher, nullptr, params.key.data(), params.iv.data(),
                               params.encrypt)) {
            SendCryptoGuardException();
        }

        if (outStream.fail()) {
            throw std::runtime_error("Output file is not good!");
        }

        char inbuf[1024];
        unsigned char outbuf[1024 + EVP_MAX_BLOCK_LENGTH];

        int outlen;
        int inlen;

        for (;;) {
            inStream.read(inbuf, 1024);
            inlen = inStream.gcount();
            if (inlen <= 0) {
                break;
            }

            if (!EVP_CipherUpdate(ctx.get(), outbuf, &outlen, (unsigned char *)inbuf, inlen)) {
                SendCryptoGuardException();
            }

            outStream.write((char *)outbuf, outlen);
        }

        // Заканчиваем работу с cipher
        if (!EVP_CipherFinal_ex(ctx.get(), outbuf, &outlen)) {
            SendCryptoGuardException();
        }

        outStream.write((char *)outbuf, outlen);
        EVP_cleanup();
    }

    std::string CalculateChecksum(std::iostream &inStream) {
        if (inStream.fail()) {
            throw std::runtime_error("Input file is not good!");
        }

        const EVP_MD *md = EVP_get_digestbyname("sha256");
        if (md == nullptr) {
            SendCryptoGuardException();
        }

        p_evp_md_ctx mdctx(EVP_MD_CTX_new());

        if (mdctx == nullptr) {
            SendCryptoGuardException();
        }

        if (!EVP_DigestInit_ex2(mdctx.get(), md, NULL)) {
            SendCryptoGuardException();
        }

        char inbuf[1024];
        int inlen = 0;

        for (;;) {
            inStream.read(inbuf, 1024);
            inlen = inStream.gcount();
            if (inlen <= 0) {
                break;
            }

            if (!EVP_DigestUpdate(mdctx.get(), inbuf, inlen)) {
                SendCryptoGuardException();
            }
        }

        unsigned char md_value[EVP_MAX_MD_SIZE];
        unsigned int md_len;

        if (!EVP_DigestFinal_ex(mdctx.get(), md_value, &md_len)) {
            SendCryptoGuardException();
        }

        std::stringstream crc;

        for (size_t i = 0; i < md_len; ++i) {
            crc << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(md_value[i]);
        }
        return crc.str();
    }
};

CryptoGuardCtx::CryptoGuardCtx() {}

CryptoGuardCtx::~CryptoGuardCtx() {}

CryptoGuardCtx::CryptoGuardCtx(CryptoGuardCtx &&) noexcept = default;
CryptoGuardCtx &CryptoGuardCtx::operator=(CryptoGuardCtx &&) noexcept = default;

void CryptoGuardCtx::EncryptFile(std::iostream &inStream, std::iostream &outStream, std::string_view password) {
    if (!inStream.good() || !outStream.good()) {
        throw std::runtime_error("I/O stream is not good");
    }

    pImpl_->CipherFile(inStream, outStream, password, 1);
}

void CryptoGuardCtx::DecryptFile(std::iostream &inStream, std::iostream &outStream, std::string_view password) {
    if (!inStream.good() || !outStream.good()) {
        throw std::runtime_error("I/O stream is not good");
    }

    pImpl_->CipherFile(inStream, outStream, password, 0);
}

std::string CryptoGuardCtx::CalculateChecksum(std::iostream &inStream) {
    if (!inStream.good()) {
        throw std::runtime_error("Input stream is not good");
    }

    return pImpl_->CalculateChecksum(inStream);
}

}  // namespace CryptoGuard
