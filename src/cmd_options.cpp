#include "cmd_options.h"

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
    } catch (const bpo::invalid_command_line_syntax &err) {
        std::print(std::cerr, "{}\n", err.what());
        return false;
    } catch (...) {
        return false;
    }

    if (vm.empty()) {
        return false;
    }

    if (vm.count("help")) {
        std::cout << desc_ << std::endl;
        return false;
    }

    if (!CheckCmdDependence(vm)) {
        return false;
    }

    for (const auto &[cmd, val] : vm) {
        if (!ParseArg(cmd, val)) {
            return false;
        }
    }

    return true;
}

bool ProgramOptions::ParseArg(const std::string &arg, const bpo::variable_value &vm) {
    if (arg == "command") {
        command_ = commandMapping_.at(vm.as<std::string>());
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

bool ProgramOptions::CheckCmdDependence(const bpo::variables_map &vm) {
    if (!vm.count("command")) {
        return false;
    }

    const auto command = vm["command"].as<std::string>();

    if (commandMapping_.find(command) == commandMapping_.end()) {
        std::print(std::cerr, "Unsupport command {}\n", command);
        return false;
    }

    if (command == "decrypt" || command == "encrypt") {
        if (vm.count("input") && vm.count("output") && vm.count("password")) {
            return true;
        }
    } else if (command == "checksum") {
        if (vm.count("input")) {
            return true;
        }
    }

    return false;
}

}  // namespace CryptoGuard
