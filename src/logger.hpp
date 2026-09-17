#include <iostream>
#include <fstream>
#include <vector>

#pragma once
class Logger
{
public:
    static void log(const std::vector<std::string> &contents, const std::string &file)
    {
        std::ofstream write{file, std::ios::app};
        if (!write.is_open())
        {
            std::cout << "could not open file" << std::endl;
            return;
        }

        for (const std::string &x : contents)
            write << x << ',';
        write << std::endl;
        std::cout << "logger done" << std::endl;
    }

    Logger() = default;

    Logger(const Logger &) = default;
    Logger &operator=(const Logger &) = default;

    Logger(Logger &&) = default;
    Logger &operator=(Logger &&) = default;

    ~Logger() = default;
};