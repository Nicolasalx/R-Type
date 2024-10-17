/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** log
*/

#include "Logger.hpp"
#include <chrono>
#include <iostream>

#if WIN32

void eng::logInfo(const std::string &msg)
{
    std::cerr << "\033[96minfo: \033[0m" << msg << '\n';
}

void eng::logWarning(const std::string &msg)
{
    std::cerr << "\033[95mwarning: \033[0m" << msg << '\n';
}

void eng::logError(const std::string &msg)
{
    std::cerr << "\033[91merror: \033[0m" << msg << '\n';
}

void eng::logTimeInfo(const std::string &msg)
{
    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

    eng::logInfo(std::to_string(currentTimeMs) + ": " + msg);
}

void eng::logTimeWarning(const std::string &msg)
{
    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

    eng::logWarning(std::to_string(currentTimeMs) + ": " + msg);
}

void eng::logTimeError(const std::string &msg)
{
    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

    eng::logError(std::to_string(currentTimeMs) + ": " + msg);
}

#else

    #include <format>

void eng::logInfo(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[2m{}:\033[96m{}\033[0;2m: "
        "\033[96minfo: \033[0m{}\n",
        std::string(info.file_name()).substr(std::string(info.file_name()).find_last_of('/') + 1),
        info.line(),
        msg
    );
}

void eng::logWarning(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[95mwarning: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void eng::logError(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[91merror: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void eng::logTimeInfo(const std::string &msg, std::source_location info)
{
    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

    eng::logInfo(std::to_string(currentTimeMs) + ": " + msg, info);
}

void eng::logTimeWarning(const std::string &msg, std::source_location info)
{
    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

    eng::logWarning(std::to_string(currentTimeMs) + ": " + msg, info);
}

void eng::logTimeError(const std::string &msg, std::source_location info)
{
    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

    eng::logError(std::to_string(currentTimeMs) + ": " + msg, info);
}

#endif
