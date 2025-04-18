#include "cmd_options.h"
#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(ProgramOptions, TestHelpOption) {
    int argc = 2;
    char **argv = new char *[argc + 1];

    std::string strings[] = {"ProgramName", "--help"};
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strings[i].size() + 1];
        std::strcpy(argv[i], strings[i].c_str());
    }

    argv[argc] = nullptr;

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));

    delete[] argv[1];
    std::string help("--helpw");
    argv[1] = new char[help.size() + 1];
    argv[1] = std::strcpy(argv[1], help.c_str());

    ASSERT_FALSE(po.Parse(argc, argv));

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST(ProgramOptions, TestСommand) {
    int argc = 3;
    char **argv = new char *[argc + 1];

    std::string strings[] = {"ProgramName", "--command", "encrypt"};
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strings[i].size() + 1];
        std::strcpy(argv[i], strings[i].c_str());
    }

    argv[argc] = nullptr;

    CryptoGuard::ProgramOptions po;

    ASSERT_TRUE(po.Parse(argc, argv));

    delete[] argv[2];
    std::string cmd("enrypt");
    argv[2] = new char[cmd.size() + 1];
    argv[2] = std::strcpy(argv[2], cmd.c_str());

    ASSERT_FALSE(po.Parse(argc, argv));

    delete[] argv[1];
    std::string option("--cmmand");
    argv[1] = new char[option.size() + 1];
    argv[1] = std::strcpy(argv[1], option.c_str());

    ASSERT_FALSE(po.Parse(argc, argv));

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST(ProgramOptions, TestInputOption) {
    int argc = 3;
    char **argv = new char *[argc + 1];

    std::string strings[] = {"ProgramName", "--input", "input.txt"};
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strings[i].size() + 1];
        std::strcpy(argv[i], strings[i].c_str());
    }

    argv[argc] = nullptr;

    CryptoGuard::ProgramOptions po;

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetInputFile().c_str(), "input.txt");

    delete[] argv[1];
    std::string option("-i");
    argv[1] = new char[option.size() + 1];
    argv[1] = std::strcpy(argv[1], option.c_str());

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetInputFile().c_str(), "input.txt");

    delete[] argv[1];
    option.clear();
    option.append("--inpt");
    argv[1] = new char[option.size() + 1];
    argv[1] = std::strcpy(argv[1], option.c_str());

    ASSERT_FALSE(po.Parse(argc, argv));

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST(ProgramOptions, TestOutputOption) {
    int argc = 3;
    char **argv = new char *[argc + 1];

    std::string strings[] = {"ProgramName", "--output", "output.txt"};
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strings[i].size() + 1];
        std::strcpy(argv[i], strings[i].c_str());
    }

    argv[argc] = nullptr;

    CryptoGuard::ProgramOptions po;

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetOutputFile().c_str(), "output.txt");

    delete[] argv[1];
    std::string option("-o");
    argv[1] = new char[option.size() + 1];
    argv[1] = std::strcpy(argv[1], option.c_str());

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetOutputFile().c_str(), "output.txt");

    delete[] argv[1];
    option.clear();
    option.append("--outpt");
    argv[1] = new char[option.size() + 1];
    argv[1] = std::strcpy(argv[1], option.c_str());

    ASSERT_FALSE(po.Parse(argc, argv));

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

TEST(ProgramOptions, TestPassword) {
    int argc = 3;
    char **argv = new char *[argc + 1];

    std::string strings[] = {"ProgramName", "--password", "12345"};
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strings[i].size() + 1];
        std::strcpy(argv[i], strings[i].c_str());
    }

    argv[argc] = nullptr;

    CryptoGuard::ProgramOptions po;

    ASSERT_TRUE(po.Parse(argc, argv));
    EXPECT_STREQ(po.GetPassword().c_str(), "12345");

    delete[] argv[1];
    std::string option("-p");
    argv[1] = new char[option.size() + 1];
    argv[1] = std::strcpy(argv[1], option.c_str());

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetPassword().c_str(), "12345");

    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}