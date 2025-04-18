#pragma once

#include <cstddef>
#include <exception>
#include <memory>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace CryptoGuard {

class ExceptionCryptoGuard : public std::runtime_error {
public:
    ExceptionCryptoGuard(const std::string &value) : runtime_error(value), error_code_(0) {}

    ExceptionCryptoGuard(const std::string &value, unsigned long error_code)
        : runtime_error(value), error_code_(error_code) {}

    unsigned long GetErrCode() const { return error_code_; }

private:
    unsigned long error_code_;
};

class CryptoGuardCtx {
public:
    CryptoGuardCtx();
    ~CryptoGuardCtx();

    CryptoGuardCtx(const CryptoGuardCtx &) = delete;
    CryptoGuardCtx &operator=(const CryptoGuardCtx &) = delete;

    CryptoGuardCtx(CryptoGuardCtx &&) noexcept;
    CryptoGuardCtx &operator=(CryptoGuardCtx &&) noexcept;

    // API
    void EncryptFile(std::iostream &inStream, std::iostream &outStream, std::string_view password);
    void DecryptFile(std::iostream &inStream, std::iostream &outStream, std::string_view password);
    std::string CalculateChecksum(std::iostream &inStream);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};

}  // namespace CryptoGuard
