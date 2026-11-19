// Урок C2: задания 3 и 4.
// Компиляция:  g++ -Wall -o c2 c2_pointers.cc && ./c2
#include <iostream>

// Задание 3: прибавить edep к total через ссылку
void AddEnergy(double& total, double edep) {
    total += edep;
    return;
}

// Задание 4: то же через указатель; если total == nullptr — ничего не делать
void AddEnergyPtr(double* total, double edep) {
    // ваш iкod
    if (total == nullptr) {
        return;
    }
    else {
        *total += edep;
    }
}

int main() {
    double sum = 0.;

    AddEnergy(sum, 1.5);
    AddEnergy(sum, 2.0);
    std::cout << sum << std::endl;     // ожидается 3.5

    AddEnergyPtr(&sum, 0.5);
    AddEnergyPtr(nullptr, 100.);       // не должно упасть
    std::cout << sum << std::endl;     // ожидается 4

    return 0;
}
