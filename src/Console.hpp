#pragma once
#include <iostream>
#include <string>

enum class Color { RED, GREEN, YELLOW, DEFAULT };

class Console
{
public:
    Console() = delete;
    ~Console() = delete;

    static void LOG(const std::string& text, Color color = Color::DEFAULT)
    {
        std::cout << getColorCode(color) << text << "\033[0m";
    }

    static void LOGN(const std::string& text, Color color = Color::DEFAULT)
    {
        LOG(text + "\n", color);
    }

private:
    static std::string getColorCode(Color color)
    {
        switch (color)
        {
        case Color::RED:     return "\033[31m";
        case Color::GREEN:   return "\033[32m";
        case Color::YELLOW:  return "\033[33m";
        default:             return "\033[0m";
        }
    }
};
