#pragma once
#include <exception>

#ifndef NO_TEST_LOG_BUFFERING
    #define OUT stdout
    #define ERR stderr
#else
    #define OUT stderr
    #define ERR stderr
#endif

#define _STRINGIFY(X) #X
#define STRINGIFY(MACRO) _STRINGIFY(MACRO)

#define FILE_LINE_STR __FILE__":" STRINGIFY(__LINE__)

#define STATUS_MSG_STR "[\033[33mSTATUS\033[0m] "
#define SUCCESS_MSG_STR "[\033[32mSUCCESS\033[0m] "
#define ERROR_MSG_STR "[\033[31mERROR\033[0m] "
#define FAIL_MSG_STR "[\033[31mFAILED\033[0m] "
#define CHECK_FAIL_MSG_STR "[\033[31mCHECK_FAIL\033[0m] "

extern const char* const test_name_g;

void fail_test_check();

void test_function();

class test_fail_exception : public std::exception
{
    const char* const message_;
public:
    inline constexpr virtual const char* what() const noexcept override
    {
        return message_;
    }
    test_fail_exception(const char* message):
        message_(message)
    {

    }
};

#define TEST_FAIL_MSG(FORMAT, ...) std::fprintf(OUT, FAIL_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_FAIL_MSG_ERR(FORMAT, ...) std::fprintf(ERR, FAIL_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_CHECK_FAIL_MSG(FORMAT, ...) std::fprintf(OUT, CHECK_FAIL_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_CHECK_FAIL_MSG_ERR(FORMAT, ...) std::fprintf(ERR, CHECK_FAIL_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_STATUS_MSG(FORMAT, ...) std::fprintf(OUT, STATUS_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_STATUS_MSG_ERR(FORMAT, ...) std::fprintf(ERR, STATUS_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_ERROR_MSG(FORMAT, ...) std::fprintf(OUT, ERROR_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_ERROR_MSG_ERR(FORMAT, ...) std::fprintf(ERR, ERROR_MSG_STR FORMAT "\n", ##__VA_ARGS__)

#define TEST_FAIL_EXCEPT(MESSAGE) throw(test_fail_exception(MESSAGE))
#define TEST_FAIL_CHECK(FORMAT, ...) { fail_test_check(); TEST_ERROR_MSG(FORMAT, ##__VA_ARGS__); }