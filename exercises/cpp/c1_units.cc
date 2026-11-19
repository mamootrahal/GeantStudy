// Урок C1: задания 3 и 4.
// Пока без Geant4: единицы объявлены вручную, как они устроены внутри CLHEP.
#include <iostream>

const double MeV = 1.;
const double keV = 1.e-3 * MeV;

// Задание 3: верните энергию, выраженную в кэВ
double ToKeV(double energy) {
    return keV;
}

int main() {
    std::cout << ToKeV(2 * MeV) << std::endl;   // ожидается 2000

    // Задание 4: цикл, печатающий 1, 2, 4, 8, 16 МэВ

    return 0;
}
