#include "cmd_options.h"
#include "crypto_guard_ctx.h"
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <print>
#include <stdexcept>
#include <string>

void FuncEncryptDecrypt(CryptoGuard::ProgramOptions &options, CryptoGuard::CryptoGuardCtx &cryptoCtx) {
    std::ifstream input_file(options.GetInputFile(), std::ios::in);
    if (!input_file.is_open()) {
        throw std::runtime_error{"Can't open input file"};
    }

    std::ofstream out_file(options.GetOutputFile());
    if (!out_file.is_open()) {
        throw std::runtime_error{"Can't open output file"};
    }

    std::stringstream in_stream;
    in_stream << input_file.rdbuf();

    std::stringstream out_stream;

    using COMMAND_TYPE = CryptoGuard::ProgramOptions::COMMAND_TYPE;
    if (options.GetCommand() == COMMAND_TYPE::ENCRYPT) {
        cryptoCtx.EncryptFile(in_stream, out_stream, options.GetPassword());
    } else if (options.GetCommand() == COMMAND_TYPE::DECRYPT) {
        cryptoCtx.DecryptFile(in_stream, out_stream, options.GetPassword());
    }

    out_file << out_stream.rdbuf();

    input_file.close();
    out_file.close();
}

int main(int argc, char *argv[]) {
    try {
        CryptoGuard::ProgramOptions options;

        if (!options.Parse(argc, argv)) {
            return 1;
        }

        CryptoGuard::CryptoGuardCtx cryptoCtx;

        using COMMAND_TYPE = CryptoGuard::ProgramOptions::COMMAND_TYPE;
        switch (options.GetCommand()) {
        case COMMAND_TYPE::ENCRYPT: {
            FuncEncryptDecrypt(options, cryptoCtx);
            std::print("File encoded successfully\n");
            break;
        }
        case COMMAND_TYPE::DECRYPT: {
            FuncEncryptDecrypt(options, cryptoCtx);
            std::print("File decoded successfully\n");
            break;
        }
        case COMMAND_TYPE::CHECKSUM: {
            std::ifstream input_file(options.GetInputFile(), std::ios::in);
            if (!input_file.is_open()) {
                throw std::runtime_error{"Can't open input file"};
            }

            std::stringstream in_stream;
            in_stream << input_file.rdbuf();

            std::string check_sum = cryptoCtx.CalculateChecksum(in_stream);
            input_file.close();
            std::print("Checksum: {}\n", check_sum);
            break;
        }
        default:
            throw std::runtime_error{"Unsupported command"};
        }
    } catch (const CryptoGuard::ExceptionCryptoGuard &e) {
        std::print(std::cerr, "Error code: {}\nErr message: {}\n", e.GetErrCode(), e.what());
        return 1;
    } catch (const std::exception &e) {
        std::print(std::cerr, "Error: {}\n", e.what());
        return 1;
    }

    return 0;
}