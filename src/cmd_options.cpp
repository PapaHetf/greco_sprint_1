#include "cmd_options.h"
#include <boost/container/container_fwd.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>

namespace CryptoGuard {

ProgramOptions::ProgramOptions() : desc_("Allowed options") {
    desc_.add_options()("help", "available options;")("command", bpo::value<std::string>(),
                                                      "support command encrypt, decrypt, checksum")(
        "input,i", bpo::value<std::string>(), "path to the input file")("output,o", bpo::value<std::string>(),
                                                                        "path to the result output file")(
        "password,p", bpo::value<std::string>(), "password for encrypt/decrypt");
}

ProgramOptions::~ProgramOptions() = default;

bool ProgramOptions::Parse(int argc, char *argv[]) {
    bpo::variables_map vm;

    try {
        bpo::store(bpo::parse_command_line(argc, argv, desc_), vm);
        bpo::notify(vm);
    } catch (bpo::unknown_option &err) {
        std::print(std::cerr, "{}\n", err.what());
        return false;
    }

    if (vm.count("help")) {
        std::cout << desc_ << std::endl;
        return false;
    }
    if (vm.empty()) {
        return false;
    }
    for (const auto &[cmd, val] : vm) {
        try {
            if (!ParseArg(cmd, val)) {
                return false;
            }
        } catch (const std::out_of_range &err) {
            std::print(std::cerr, "{} {} Unsupport command\n", __FILE__, __LINE__);
            return false;
        }
    }
    return true;
}

bool ProgramOptions::ParseArg(const std::string &arg, const bpo::variable_value &vm) {
    if (arg == "command") {
        auto command = vm.as<std::string>();
        command_ = commandMapping_.at(command);
    } else if (arg == "input") {
        inputFile_ = vm.as<std::string>();
    } else if (arg == "output") {
        outputFile_ = vm.as<std::string>();
    } else if (arg == "password") {
        password_ = vm.as<std::string>();
    } else {
        return false;
    }

    return true;
}

}  // namespace CryptoGuard
