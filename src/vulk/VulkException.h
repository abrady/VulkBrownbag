#pragma once

#include <iostream>

#include <stdexcept>

#include <fmt/core.h>
#include <fmt/format.h>

// C++20 concept to check if a type is an enum
template <typename T>
concept EnumType = std::is_enum_v<T>;

#ifdef _MSC_VER
#include <stacktrace>
class VulkException : public std::exception
{
    void init()
    {
        msg += "\nStacktrace:\n";
        for (const auto &frame : st)
        {
            msg += fmt::format("  {}\n", std::to_string(frame));
        }
    }

public:
    VulkException(const char *message) : msg(message), st(std::stacktrace::current())
    {
        init();
    }

    VulkException(std::string message) : msg(message), st(std::stacktrace::current())
    {
        init();
    }

    const char *what() const noexcept override
    {
        return msg.c_str();
    }

    const std::stacktrace &get_stacktrace() const
    {
        return st;
    }

private:
    std::string msg;
    std::stacktrace st;
};
#define VULKEXCEPTION(msg) VulkException(msg)
#else
class VulkException : public std::runtime_error
{
public:
    // VulkException(char const *file, int line, const char *message) : std::runtime_error(std::string(file) + ":" + std::to_string(line) + message) {
    // }

    VulkException(char const *file, int line, std::string message) : std::runtime_error(std::string(file) + ":" + std::to_string(line) + message)
    {
    }
};
#define VULKEXCEPTION(msg) VulkException(__FILE__, __LINE__, msg)
#endif

#define VULK_THROW(msg) throw VULKEXCEPTION(msg)
#define VULK_THROW_FMT(format_str, ...) throw VULKEXCEPTION(fmt::format(format_str, __VA_ARGS__))
#define VULK_THROW_IF(cond, msg) \
    do                           \
    {                            \
        if (cond)                \
        {                        \
            VULK_THROW(msg);     \
        }                        \
    } while (0)