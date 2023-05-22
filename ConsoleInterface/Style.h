#pragma once

#include <string>
#include <iostream>

struct Style {
    inline static std::string error_color = "\033[0;31m";
    inline static std::string request_color = "\033[0;35m";
    inline static std::string default_color = "\033[0m";
    inline static std::string exit_word = "exit";
    inline static std::string home_word = "home";

    static void InputRequest(int count, const std::string &request) {
        std::cout << request_color << (count == 1 ? "Enter " : "Enter separated by a space: ")\
            << request << " or '" << exit_word << " or '" << home_word << "'\n" << default_color;
    }

    static void MenuRequest() {
        std::cout << request_color << "Enter number of your choice or '"\
            << exit_word << " or '" << home_word << "'\n" << default_color;
    }

    static void ErrorPrint(const std::string &error) {
        std::cout << error_color << "Incorrect input: \"" << error << "\". Please try again\n" << default_color;
    }
};
