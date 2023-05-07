#pragma once

#include <string>
#include <initializer_list>
#include <list>
#include <functional>
#include <tuple>
#include <memory>

#include "Style.h"
#include <vector>
#include <functional>
#include <limits>

class Menu;
class Option;

class AbsMenu {
    protected:
        std::string name_;
    public:
        virtual void Action() = 0;
        AbsMenu(const std::string &name) : name_(name) {}
        std::string &GetName() { return name_; }
        virtual ~AbsMenu() = default;
};

template<class ...Args>
class Input : public AbsMenu {
    friend class Menu;

    public:
        Input(const std::string &request,
                const std::function<void(Args&...)> &action,
                AbsMenu *next_menu, const std::string &name = "");

        void Action() override;

    private:
        std::function<void(Args&...)> action_;
        AbsMenu *next_menu_;
        bool exit_ = false;
        std::string request_;

        template<class T>
        T Read();
};

class Menu : public AbsMenu {
    public:
        Menu(const std::initializer_list<std::string> &options, const std::string &name);

        void Action() override;
        void Connect(int k, AbsMenu *next_menu, const std::function<void(void)> &action = []{});

    private:
        class Option {
            friend class Menu;
            public:
                Option() {}
                Option(const std::string &name);

                void operator()();

            private:
                std::string name_;
                std::function<void(void)> action_;
                AbsMenu *next_menu_;
        };

        std::vector<Option> options_;
        const std::string name_;
};

class ConsoleInterface {
    public:
        ConsoleInterface() {}

        Menu *AddMenu(const std::initializer_list<std::string> &options, const std::string &name = "");

        template<class ...Args, class Func>
        Input<Args...> *AddInput(const std::string &request, const Func &action, AbsMenu *next_menu) {
            menus_.emplace_back(std::unique_ptr<AbsMenu>(new Input<Args...>(request, std::function<void(Args&...)>(action), next_menu)));
            return dynamic_cast<Input<Args...>*>(menus_.back().get());
        }

        void Start();

    private:
        std::list<std::unique_ptr<AbsMenu>> menus_;
};




template<class ...Args>
Input<Args...>::Input(const std::string &request,
        const std::function<void(Args&...)> &action,
        AbsMenu *next_menu, const std::string &name) :
        AbsMenu(name), action_(std::move(action)), next_menu_(next_menu), request_(request) {}

template<class ...Args>
void Input<Args...>::Action() {
    std::cout << GetName() << '\n'; 
    Style::InputRequest(sizeof...(Args), request_);
    try {
        auto args = std::make_tuple(Read<Args>()...);
        if (exit_) {
            exit_ = false;
            return;
        }
        std::apply(action_, args);
    } catch (const std::exception &e) {
        if (exit_) {
            exit_ = false;
            return;
        }
        Style::ErrorPrint(e.what());
        Action();
    }
    next_menu_->Action();
}

template<class ...Args>
template<class T>
T Input<Args...>::Read() {
    std::string word;
    if (!(std::cin >> word)) {
        std::string error_input;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error(error_input);
    }
    if (word == Style::exit_word) {
        exit_ = true;
    }
    std::istringstream iss(word);
    T value;
    if (!(iss >> value)) {
        throw std::runtime_error(word);
    }
    return value;
}


