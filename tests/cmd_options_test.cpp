#include "cmd_options.h"
#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(ProgramOptions, TestCommandHelp) {
    int argc = 2;
    char arg0[] = "ProgramName";
    char arg1[] = "--help";
    char *argv[] = {arg0, arg1};

    CryptoGuard::ProgramOptions po;
    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestInvalidCommandHelp) {
    int argc = 2;
    char arg0[] = "ProgramName";
    char arg1[] = "--helpw";
    char *argv[] = {arg0, arg1};

    CryptoGuard::ProgramOptions po;
    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestNotFullOptionСommand) {
    int argc = 3;
    char arg0[] = "ProgramName";
    char arg1[] = "--command";
    char arg2[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestInputOption) {
    int argc = 9;
    char arg0[] = "ProgramName";
    char arg1[] = "--input";
    char arg2[] = "input.txt";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "-p";
    char arg6[] = "123";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetInputFile().c_str(), "input.txt");

    char arg1_new[] = "-i";
    argv[1] = arg1_new;

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetInputFile().c_str(), "input.txt");
}

TEST(ProgramOptions, TestIncorrectInputOption) {
    int argc = 9;
    char arg0[] = "ProgramName";
    char arg1[] = "--inpt";
    char arg2[] = "input.txt";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "-p";
    char arg6[] = "123";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestOutputOption) {
    int argc = 9;
    char arg0[] = "ProgramName";
    char arg1[] = "--input";
    char arg2[] = "input.txt";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "-p";
    char arg6[] = "123";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetOutputFile().c_str(), "output.txt");

    char arg3_new[] = "-o";
    argv[3] = arg3_new;

    ASSERT_TRUE(po.Parse(argc, argv));

    EXPECT_STREQ(po.GetOutputFile().c_str(), "output.txt");
}

TEST(ProgramOptions, TestIncorrectOutputOption) {
    int argc = 9;
    char arg0[] = "ProgramName";
    char arg1[] = "--input";
    char arg2[] = "input.txt";
    char arg3[] = "--outpt";
    char arg4[] = "output.txt";
    char arg5[] = "-p";
    char arg6[] = "123";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestPasswordOption) {
    int argc = 9;
    char arg0[] = "ProgramName";
    char arg1[] = "--input";
    char arg2[] = "input.txt";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "--password";
    char arg6[] = "12345";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_TRUE(po.Parse(argc, argv));
    EXPECT_STREQ(po.GetPassword().c_str(), "12345");

    char arg5_new[] = "-p";
    argv[5] = arg5_new;

    ASSERT_TRUE(po.Parse(argc, argv));
    EXPECT_STREQ(po.GetPassword().c_str(), "12345");
}

TEST(ProgramOptions, TestIncorrectPasswordOption) {
    int argc = 9;
    char arg0[] = "ProgramName";
    char arg1[] = "--input";
    char arg2[] = "input.txt";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "-aswrd";
    char arg6[] = "12345";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestEmptyCommand) {
    int argc = 2;
    char arg0[] = "ProgramName";
    char arg1[] = "";
    char *argv[] = {arg0, arg1};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestIncompleteCommandArgument) {
    int argc = 8;
    char arg0[] = "ProgramName";
    char arg1[] = "--input";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "--password";
    char arg6[] = "12345";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestIncompleteCommand1) {
    int argc = 8;
    char arg0[] = "ProgramName";
    char arg2[] = "input.txt";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "--password";
    char arg6[] = "12345";
    char arg7[] = "--command";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg2, arg3, arg4, arg5, arg6, arg7, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}

TEST(ProgramOptions, TestIncompleteCommand2) {
    int argc = 8;
    char arg0[] = "ProgramName";
    char arg1[] = "--input";
    char arg2[] = "input.txt";
    char arg3[] = "-o";
    char arg4[] = "output.txt";
    char arg5[] = "-paswrd";
    char arg6[] = "12345";
    char arg8[] = "encrypt";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg8};

    CryptoGuard::ProgramOptions po;

    ASSERT_FALSE(po.Parse(argc, argv));
}