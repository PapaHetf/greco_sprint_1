#include "cmd_options.h"
#include "crypto_guard_ctx.h"
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <print>
#include <stdexcept>
#include <string>

std::pair<std::fstream, std::fstream> OpenInOutFiles(std::string_view in, std::string_view out) {

    std::fstream input_file(in.data(), std::ios::in);
    if (!input_file.is_open()) {
        throw std::runtime_error{"Can't open input file"};
    }

    std::fstream out_file(out.data(), std::ios::out);
    if (!out_file.is_open()) {
        throw std::runtime_error{"Can't open output file"};
    }
    return {std::move(input_file), std::move(out_file)};
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
            auto [in, out] = OpenInOutFiles(options.GetInputFile(), options.GetOutputFile());
            cryptoCtx.EncryptFile(in, out, options.GetPassword());
            std::print("File encoded successfully\n");
            break;
        }
        case COMMAND_TYPE::DECRYPT: {
            auto [in, out] = OpenInOutFiles(options.GetInputFile(), options.GetOutputFile());
            cryptoCtx.DecryptFile(in, out, options.GetPassword());
            std::print("File decoded successfully\n");
            break;
        }
        case COMMAND_TYPE::CHECKSUM: {
            std::fstream input_file(options.GetInputFile(), std::ios::in);
            if (!input_file.is_open()) {
                throw std::runtime_error{"Can't open input file"};
            }

            std::string check_sum = cryptoCtx.CalculateChecksum(input_file);
            // input_file.close();
            std::print("Checksum: {}\n", check_sum);
            break;
        }
        default:
            throw std::runtime_error{"Unsupported command"};
        }
    } catch (const std::exception &e) {
        std::print(std::cerr, "Error: {}\n", e.what());
        return 1;
    }

    return 0;
}