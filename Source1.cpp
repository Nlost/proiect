#include <string.h>
#include <vector>
#include<iostream>


class Cake {
public:
    Cake(const std::string& name, const std::string& recipe, int preparation_time)
        : name_(name), recipe_(recipe), preparation_time_(preparation_time) {}

    std::string name() const { return name_; }
    std::string recipe() const { return recipe_; }
    int preparation_time() const { return preparation_time_; }

private:
    std::string name_;
    std::string recipe_;
    int preparation_time_;
};

class DiabeticCake : public Cake {
public:
    DiabeticCake(const std::string& name, const std::string& recipe, int preparation_time,
        int sugar_quantity)
        : Cake(name, recipe, preparation_time), sugar_quantity_(sugar_quantity) {}

    int sugar_quantity() const { return sugar_quantity_; }

private:
    int sugar_quantity_;
};

class CakeMaker {
public:
    Cake MakeCake(const std::string& recipe, int preparation_time) {
        // simulate waiting time based on the recipe
        return Cake(recipe, recipe, preparation_time);
    }
};

class VendingMachine {
public:
    VendingMachine(int max_capacity) : max_capacity_(max_capacity), storage_() {}

    void PickOrder(const Cake& cake) {
        auto it = std::find_if(storage_.begin(), storage_.end(), [&cake](const auto& c) { return c == cake; });
        if (it != storage_.end()) {
            // serve the cake to the user
            storage_.erase(it);
        }
        else {
            // create an order for the cake maker
            CakeMaker cake_maker;
            storage_.push_back(cake_maker.MakeCake(cake.recipe(), cake.preparation_time()));
        }
    }

    std::vector<std::string> ReturnCookieNames() const {
        std::vector<std::string> names;
        for (const auto& cake : storage_) {
            names.push_back(cake.name());
        }
        return names;
    }

    void Refill() {
        // fill the warehouse with the last cake ordered
    }

private:
    int max_capacity_;
    std::vector<Cake> storage_;
};




int main() {
    VendingMachine vending_machine(10);

    while (true) {
        std::cout << "Enter a command:\n";
        std::cout << "1) Display possible cookies\n";
        std::cout << "2) Order cake\n";
        std::cout << "3) Display existing cookies\n";

        int command;
        std::cin >> command;

        if (command == 1) {
            // display the list of possible cookies
            std::cout << "Possible cookies:\n";
            std::cout << " - Chocolate cake\n";
            std::cout << " - Vanilla cake\n";
            std::cout << " - Diabetic chocolate cake\n";
            std::cout << " - Diabetic vanilla cake\n";
        }
        else if (command == 2) {
            std::cout << "Enter the name of the cake to order:\n";
            std::string name;
            std::cin >> name;

            if (name == "Chocolate cake") {
                Cake chocolate_cake("Chocolate cake", "Chocolate cake recipe", 30);
                vending_machine.PickOrder(chocolate_cake);
            }
            else if (name== "Vanilla cake") {
                Cake vanilla_cake("Vanilla cake", "Vanilla cake recipe", 30);
                vending_machine.PickOrder(vanilla_cake);
            }
            else if (name == "Diabetic chocolate cake") {
                DiabeticCake diabetic_chocolate_cake("Diabetic chocolate cake", "Diabetic chocolate cake recipe", 30, 5);
                vending_machine.PickOrder(diabetic_chocolate_cake);
            }
            else if (name == "Diabetic vanilla cake") {
                DiabeticCake diabetic_vanilla_cake("Diabetic vanilla cake", "Diabetic vanilla cake recipe", 30, 5);
                vending_machine.PickOrder(diabetic_vanilla_cake);
            }
            else {
                std::cout << "Invalid cake name\n";
            }
        }
        else if (command == 3) {
            // display the list of existing cookies in the warehouse
            auto existing_cookies = vending_machine.ReturnCookieNames();
            std::cout << "Existing cookies:\n";
            for (const auto& name : existing_cookies) {
                std::cout << " - " << name << "\n";
            }
        }
        else {
            std::cout << "Invalid command\n";
        }
    }

    return 0;
}