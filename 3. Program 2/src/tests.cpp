#include <iostream>

void assertBoolEquality(bool a, bool b, std::string test_name)
{
    if (a == b)
    {
        std::cout << "TEST PASSED!"
                  << " [" << test_name << "]" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED.."
                  << " [" << test_name << "]" << std::endl;
    }
}

void assertBoolInEquality(bool a, bool b, std::string test_name)
{
    if (a == b)
    {
        std::cout << "TEST PASSED!"
                  << " [" << test_name << "]" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED.."
                  << " [" << test_name << "]" << std::endl;
    }
}

void assertNumbersEquality(float a, float b, std::string test_name)
{
    if (a == b)
    {
        std::cout << "TEST PASSED!"
                  << " [" << test_name << "]" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED.."
                  << " [" << test_name << "]" << std::endl;
    }
}