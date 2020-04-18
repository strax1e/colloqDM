#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include "BigInteger.h"

struct LogCell
{
    LogCell() = default;
    LogCell(const LogCell&) = default;
    LogCell(LogCell&&) = default;
    LogCell& operator =(const LogCell&) = default;
    LogCell& operator =(LogCell&&) = default;

    LogCell(std::string&& left, std::string&& right, std::string&& operation,
        std::string&& result, std::string&& must, std::string&& test): left(left), right(right),
            operation(operation), result(result), must(must), test(test){}

    std::string left;
    std::string right;
    std::string operation;
    std::string result;
    std::string must;
    std::string test;
};

typedef std::vector<LogCell> logBook;

template < typename T >
void test(logBook& logBook, T&& left, T&& right, T&& must,
          const std::string& operation, const std::string& testNumber = "-")
{
    std::stringstream leftSS;
    std::stringstream rightSS;
    std::stringstream resultSS;
    std::stringstream mustSS;

    std::string leftS;
    std::string rightS;
    std::string resultS;
    std::string mustS;

    leftSS << left;
    leftSS >> leftS;
    rightSS << right;
    rightSS >> rightS;
    mustSS << must;
    mustSS >> mustS;

    try
    {
        if (operation == "+=")
            left += right;
        else if (operation == "-=")
            left -= right;
        else if (operation == "*=")
            left *= right;
        else if (operation == "/=")
            left /= right;

        else if (operation == "+")
            left = left + right;
        else if (operation == "-")
            left = left - right;
        else if (operation == "*")
            left = left * right;
        else if (operation == "/")
            left = left / right;
        else if (operation == "%")
            left = left % right;

        else if (operation == "==")
            left = BigInteger(left == right);
        else if (operation == "!=")
            left = BigInteger(left != right);
        else if (operation == ">")
            left = BigInteger(left > right);
        else if (operation == "<")
            left = BigInteger(left < right);
        else if (operation == ">=")
            left = BigInteger(left >= right);
        else if (operation == "<=")
            left = BigInteger(left <= right);

        else
        {
            logBook.push_back( LogCell(std::move(leftS), std::move(rightS), std::string(operation),
                                "undefined operation", std::move(mustS), std::string(testNumber)) );
            return;
        }

        resultSS << left;
        resultSS >> resultS;

        if (resultS == mustS)
            return;

        logBook.push_back( LogCell(std::move(leftS), std::move(rightS), std::string(operation),
                            std::move(resultS), std::move(mustS), std::string(testNumber)) );
    }
    catch(std::exception& ex)
    {
        logBook.push_back( LogCell(std::move(leftS), std::move(rightS), std::string(operation),
                            std::string(ex.what()), std::move(mustS), std::string(testNumber)) );
    }
}

typedef BigInteger num;

int main()
{
    logBook logBook;
    test(logBook, num(17),  num(5),  num(3),  "/", "1");
    test(logBook, num(-17), num(5),  num(-4), "/", "2");
    test(logBook, num(17),  num(-5), num(-3), "/", "3");
    test(logBook, num(-17), num(-5), num(4),  "/", "4");

    test(logBook, num(17),  num(5),  num(2), "%", "5");
    test(logBook, num(-17), num(5),  num(3), "%", "6");
    test(logBook, num(17),  num(-5), num(2), "%", "7");
    test(logBook, num(-17), num(-5), num(3), "%", "8");

    test(logBook, num(-1), num(5), num(0), "==", "9");
    test(logBook, num(-1), num(5), num(1), "!=", "10");
    test(logBook, num(-1), num(5), num(0), ">",  "11");
    test(logBook, num(-1), num(5), num(1), "<",  "12");
    test(logBook, num(-1), num(5), num(0), ">=", "13");
    test(logBook, num(-1), num(5), num(1), "<=", "14");

    for (auto&& i : logBook)
    {
        std::cout << "test: ";
        std::cout.width(4);
        std::cout.setf(std::ios::left);
        std::cout << i.test;
        std::cout << ": " << i.left << " " << i.operation << " " << i.right;
        std::cout  << ";  result: " << i.result << ";   must be: " << i.must << "\n";
    }
}
