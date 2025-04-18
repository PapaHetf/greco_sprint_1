#include "crypto_guard_ctx.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <ios>
#include <sys/types.h>

TEST(Chiper, TestEncryptNoGood) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    in_stream.setstate(std::ios_base::failbit);
    ASSERT_THROW(ctx.EncryptFile(in_stream, out_stream, "12345"), CryptoGuard::ExceptionCryptoGuard);
}

TEST(Chiper, TestEncryptEof) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    in_stream.setstate(std::ios_base::eofbit);
    ASSERT_THROW(ctx.EncryptFile(in_stream, out_stream, "12345"), CryptoGuard::ExceptionCryptoGuard);
}

TEST(Chiper, TestEncryptNoThrow) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    ASSERT_NO_THROW(ctx.EncryptFile(in_stream, out_stream, "12345"));
}

TEST(Chiper, TestDecryptNoGood) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    in_stream.setstate(std::ios_base::failbit);
    ASSERT_THROW(ctx.DecryptFile(in_stream, out_stream, "12345"), CryptoGuard::ExceptionCryptoGuard);
}

TEST(Chiper, TestDecryptEof) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    in_stream.setstate(std::ios_base::eofbit);
    ASSERT_THROW(ctx.DecryptFile(in_stream, out_stream, "12345"), CryptoGuard::ExceptionCryptoGuard);
}

TEST(Chiper, TestDecryptNoThrow) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    ASSERT_NO_THROW(ctx.EncryptFile(in_stream, out_stream, "12345"));
}

TEST(Chiper, TestEncryptDecrypt) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;
    std::stringstream end_stream;

    in_stream << "Kiss me hard before you go\nSummertime sadness\0";
    ctx.EncryptFile(in_stream, out_stream, "12345");
    ctx.DecryptFile(out_stream, end_stream, "12345");

    ASSERT_STREQ(in_stream.str().c_str(), end_stream.str().c_str());

    std::stringstream end_stream_two;
    ctx.DecryptFile(out_stream, end_stream, "1234");
    ASSERT_STRNE(in_stream.str().c_str(), end_stream_two.str().c_str());
}

TEST(Chiper, TestCheckSum1) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;

    in_stream.setstate(std::ios_base::badbit);

    ASSERT_THROW(ctx.CalculateChecksum(in_stream), CryptoGuard::ExceptionCryptoGuard);
}

TEST(Chiper, TestCheckSum2) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;

    in_stream << "Fade_to_black";

    ASSERT_STREQ(ctx.CalculateChecksum(in_stream).c_str(),
                 "3e4482119aac1aefb7dd94b2292214234527d9f994ce2434dddc60fd27c7a686");
}

TEST(Chiper, TestCheckSum3) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;
    std::stringstream end_stream;

    in_stream << "Kiss me hard before you go\nSummertime sadness\0";

    std::string encrypt_summ = ctx.CalculateChecksum(in_stream);

    in_stream << "Kiss me hard before you go\nSummertime sadness\0";

    ctx.EncryptFile(in_stream, out_stream, "12345");
    ctx.DecryptFile(out_stream, end_stream, "12345");

    std::string decrypt_summ = ctx.CalculateChecksum(end_stream);

    ASSERT_STREQ(decrypt_summ.c_str(), encrypt_summ.c_str());
}