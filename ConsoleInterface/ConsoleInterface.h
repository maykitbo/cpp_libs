#pragma once

#include <string>
#include <initializer_list>
#include <list>
#include <functional>
#include <tuple>
#include <memory>
#include <sstream>
#include "Style.h"
#include <vector>
#include <limits>

class Menu;
class Option;
class ConsoleInterface;

class AbsMenu {
    protected:
        std::string name_;
        ConsoleInterface *ci_;
    public:
        virtual void Action() = 0;
        AbsMenu(const std::string &name, ConsoleInterface *ci);
        std::string &GetName();
        virtual ~AbsMenu() = default;
        void GoHome();
};

template<class ...Args>
class Input : public AbsMenu {
    friend class Menu;

    public:
        Input(const std::string &request, ConsoleInterface *ci,
                const std::function<void(Args&...)> &action,
                AbsMenu *next_menu, const std::string &name = "");

        void Action() override;

    private:
        std::function<void(Args&...)> action_;
        AbsMenu *next_menu_;
        bool exit_ = false, home_ = false;
        std::string request_;

        template<class T>
        T Read();
};

class Menu : public AbsMenu {
    public:
        Menu(const std::initializer_list<std::string> &options, const std::string &name, ConsoleInterface *ci);

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
        AbsMenu *AddInput(const std::string &request, const Func &action, AbsMenu *next_menu) {
            menus_.emplace_back(std::unique_ptr<AbsMenu>(new Input<Args...>(request, this, std::function<void(Args&...)>(action), next_menu)));
            // return dynamic_cast<Input<Args...>*>(menus_.back().get());
            return menus_.back().get();
        }

        void Start();

    private:
        friend AbsMenu;
        std::list<std::unique_ptr<AbsMenu>> menus_;
};




template<class ...Args>
Input<Args...>::Input(const std::string &request, ConsoleInterface *ci,
        const std::function<void(Args&...)> &action,
        AbsMenu *next_menu, const std::string &name) :
        AbsMenu(name, ci), action_(std::move(action)), next_menu_(next_menu), request_(request) {}

template<class ...Args>
void Input<Args...>::Action() {
    std::cout << GetName() << '\n'; 
    Style::InputRequest(sizeof...(Args), request_);
    try {
        auto args = std::make_tuple((!exit_ && !home_ ? Read<Args>() : Args())...);
        if (exit_) {
            exit_ = false;
        } else if (home_) {
            home_ = false;
            GoHome();
        } else {
            std::apply(action_, args);
            next_menu_->Action();
        }
    } catch (const std::exception &e) {
        Style::ErrorPrint(e.what());
        Action();
    }
}

template<class ...Args>
template<class T>
T Input<Args...>::Read() {
    std::string word;
    std::cin >> word;
    T value;
    if (word == Style::exit_word) {
        exit_ = true;
    } else if (word == Style::home_word) {
        home_ = true;
    } else {
        std::istringstream iss(word);
        if (!(iss >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error(word);
        }
    }
    return value;
}


