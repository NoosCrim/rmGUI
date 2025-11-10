#pragma once

#include "test_shared.hpp"
#include <string>

#ifndef TEST_NAME
#define TEST_NAME UNNAMED_TEST
#endif


#define TEST_CONDITION(CONDITION) \
    if(!CONDITION) \
        TEST_FAIL_CHECK("TEST_CONDITION failed: condition: <" #CONDITION "> at [" FILE_LINE_STR "]");



const char* const test_name_g = STRINGIFY(TEST_NAME);