#include <cstdio>
#include "test_shared.hpp"

class test_fail_exception;

unsigned int test_check_fail_count_g = 0;

void fail_test_check()
{
    ++test_check_fail_count_g;
}
int main()
{
    try
    {
        test_function();
        if(test_check_fail_count_g != 0)
        {
            TEST_FAIL_MSG_ERR("%s failed because of %u failed checks", test_name_g, test_check_fail_count_g);
            return 1;
        }
    }
    catch(const test_fail_exception &exc)
    {
        TEST_FAIL_MSG_ERR("%s failed: %s\n", test_name_g, exc.what());
        return 1;
    }
    catch(const std::exception &exc)
    {
        TEST_ERROR_MSG_ERR("%s stopped because of external exception: %s\n", test_name_g, exc.what());
        return 1;
    }
    catch(...)
    {
        TEST_ERROR_MSG_ERR("%s stopped because of  unknown exception", test_name_g);
        return 1;
    }
    
}