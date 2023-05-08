#include "ConsoleInterface.h"

AbsMenu::AbsMenu(const std::string &name, ConsoleInterface *ci) : name_(name), ci_(ci) {}

std::string &AbsMenu::GetName() { return name_; }

void AbsMenu::GoHome() {
    ci_->menus_.front()->Action();
}


void ConsoleInterface::Start() {
    if (menus_.empty()) {
        throw std::invalid_argument("Empty ConsoleInterface");
    }
    menus_.front()->Action();
}

Menu *ConsoleInterface::AddMenu(const std::initializer_list<std::string> &options, const std::string &name) {
    menus_.emplace_back(std::unique_ptr<AbsMenu>(new Menu(options, name, this)));
    return dynamic_cast<Menu*>(menus_.back().get());
}

Menu::Menu(const std::initializer_list<std::string> &options, const std::string &name, ConsoleInterface *ci) :
            AbsMenu(name, ci), options_(options.begin(), options.end()) {}

void Menu::Action() {
    std::cout << name_ << '\n';
    for (int k = 0; k < options_.size(); ++k) {
        std::cout << k + 1 << ": " << options_[k].name_ << '\n';
    }
    Style::MenuRequest();
    std::string input;
    std::cin >> input;
    if (input == "home") {
        GoHome();
    } else if (input != "exit") {
        int choice = std::atoi(input.data());
        if (choice > 0 && choice <= options_.size()) {
            try {
                options_[choice - 1]();
            } catch (std::exception &e) {
                Style::ErrorPrint(e.what());
                Action();
            }
        } else {
            Style::ErrorPrint(input);
            Action();
        }
    }
}

void Menu::Connect(int k, AbsMenu *next_menu, const std::function<void(void)> &action) {
    options_[k].action_ = std::move(action);
    options_[k].next_menu_ = next_menu;
}

Menu::Option::Option(const std::string &name) :
    name_(name), action_([&name] { throw std::invalid_argument("'" + name + "' option is empty\n"); }) {}

void Menu::Option::operator()() {
    action_();
    next_menu_->Action();
}