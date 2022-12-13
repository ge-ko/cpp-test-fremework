#include <iostream>
#include <vector>
#include <functional>
#include <numeric>

struct null_buffer_t : public std::streambuf
{
  int overflow(int c) { return c; }
};

null_buffer_t null_buffer;
std::ostream null_stream(&null_buffer);

std::ostream out = null_stream;
std::ostream err = std::cerr;

void test(const std::string& name)
{
    
}

void check(bool condition, const std::string& message = std::string()) throw(std::logic_error)
{
    if (!condition) throw std::logic_error(message);
}

struct test_suite_t
{
    using test_t = std::function<void(void)>;
    test_suite_t() = default;
    test_suite_t(test_t test) { tests.push_back(test); }
    test_suite_t(const test_suite_t& rhs)
    {
        tests.insert(tests.end(), rhs.tests.begin(), rhs.tests.end());
    }
    test_suite_t& operator<<(const std::string& command)
    {
        if (command == "verbose")
        {
            out = std::cout;
            err = std::cerr; 
        }
    }
    test_suite_t& operator<<(test_t test)
    {
        tests.push_back(test);
        return *this;
    }
    test_suite_t& operator<<(const test_suite_t& rhs)
    {
        tests.insert(tests.end(), rhs.tests.begin(), rhs.tests.end());
        return *this;
    }
    operator int()
    {
        int pass = 0, fail = 0;
        for (auto test : tests)
        {
            try
            {
                test();
                ++pass;
            }
            catch (const std::logic_error& e)
            {
                ++fail;
                if (e.what() != nullptr)
                    std::cout << e.what() << '\n';
            }
            catch (...)
            {
                ++fail;
            }
        }
        std::cout <<   "pass: " << pass
                  << ", fail: " << fail
                  << ", total: " << pass + fail
                  << '\n';
        return std::min(fail, 255);
    }
private:
    std::vector<test_t> tests;
};