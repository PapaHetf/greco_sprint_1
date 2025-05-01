#include "crypto_guard_ctx.h"
#include <gtest/gtest.h>
#include <ios>
#include <stdexcept>
#include <sys/types.h>

TEST(Chiper, TestEncryptNoGood) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    in_stream.setstate(std::ios_base::failbit);
    ASSERT_THROW(ctx.EncryptFile(in_stream, out_stream, "12345"), std::runtime_error);
}

TEST(Chiper, TestEncryptEof) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    in_stream.setstate(std::ios_base::eofbit);
    ASSERT_THROW(ctx.EncryptFile(in_stream, out_stream, "12345"), std::runtime_error);
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
    ASSERT_THROW(ctx.DecryptFile(in_stream, out_stream, "12345"), std::runtime_error);
}

TEST(Chiper, TestDecryptEof) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    in_stream.setstate(std::ios_base::eofbit);
    ASSERT_THROW(ctx.DecryptFile(in_stream, out_stream, "12345"), std::runtime_error);
}

TEST(Chiper, TestDecryptNoThrow) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;
    std::stringstream out_stream;

    ASSERT_NO_THROW(ctx.EncryptFile(in_stream, out_stream, "12345"));
}

TEST(Chiper, TestEncryptDecrypt) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream("Kiss me hard before you go\nSummertime sadness\0");
    std::stringstream out_stream;
    std::stringstream end_stream;

    ctx.EncryptFile(in_stream, out_stream, "12345");
    ctx.DecryptFile(out_stream, end_stream, "12345");

    ASSERT_STREQ(in_stream.str().c_str(), end_stream.str().c_str());
}

TEST(Chiper, TestEncryptDecryptFailKey) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream("Kiss me hard before you go\nSummertime sadness\0");
    std::stringstream out_stream;
    std::stringstream end_stream;

    ctx.EncryptFile(in_stream, out_stream, "12345");
    ASSERT_THROW(ctx.DecryptFile(out_stream, end_stream, "1234"), std::runtime_error);
}

TEST(Chiper, TestCheckSum1) {
    CryptoGuard::CryptoGuardCtx ctx;

    std::stringstream in_stream;

    in_stream.setstate(std::ios_base::badbit);

    ASSERT_THROW(ctx.CalculateChecksum(in_stream), std::runtime_error);
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

    std::stringstream in_stream("Kiss me hard before you go\nSummertime sadness\0");
    std::stringstream out_stream;
    std::stringstream end_stream;

    std::string encrypt_summ = ctx.CalculateChecksum(in_stream);

    std::stringstream in_stream_new("Kiss me hard before you go\nSummertime sadness\0");

    ctx.EncryptFile(in_stream_new, out_stream, "12345");
    ctx.DecryptFile(out_stream, end_stream, "12345");

    std::string decrypt_summ = ctx.CalculateChecksum(end_stream);

    ASSERT_STREQ(decrypt_summ.c_str(), encrypt_summ.c_str());
}