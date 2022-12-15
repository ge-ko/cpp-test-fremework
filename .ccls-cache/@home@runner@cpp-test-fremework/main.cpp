#include "test.hpp"
using namespace tst;

int create_test_suite()
{
    verbose = true;
    return test_suite_t()
    << []()
    {
        test("1 + 1 == 2");
        check(1 + 1 == 2);
    }
    << []()
    {
        test("1 + 1 == 3"); // will fail
        check(1 + 1 == 3, "expected to fail");
    };
}

int main()
{
    return create_test_suite();
}