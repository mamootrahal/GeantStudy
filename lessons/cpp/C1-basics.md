# C1. Основы C++ для Geant4

## Минимальная программа
```cpp
#include <iostream>
int main() {
    std::cout << "Hello" << std::endl;
    return 0;
}
```
В Geant4 вместо `std::cout`/`std::endl` пишут `G4cout`/`G4endl` (корректно для многопоточности и GUI).

## Типы
| C++ | Geant4 |
|---|---|
| `int` | `G4int` |
| `double` | `G4double` |
| `bool` | `G4bool` |
| `std::string` | `G4String` |

⚠️ `1/2 == 0` (целочисленное деление). Пишите `1./2`.

## Единицы — это просто константы
```cpp
const double mm = 1., cm = 10.*mm, m = 1000.*mm;
const double MeV = 1., keV = 1.e-3*MeV, GeV = 1.e3*MeV;

G4double size = 10*cm;    // хранится 100
G4cout << size/m;         // 0.1
```
Правило: записываем — **умножаем** на единицу, выводим — **делим**.
`G4BestUnit(edep, "Energy")` выберет единицу сам.

## Функции
```cpp
G4double Square(G4double x) { return x * x; }
void PrintEnergy(G4double e) { G4cout << e/MeV << " MeV" << G4endl; }
```
В Geant4 ваш код — это методы, которые Geant4 вызывает сам, например
`void UserSteppingAction(const G4Step* step)`.

## Условия и циклы
```cpp
if (particleName == "gamma" && edep > 1*keV) { ... } else { ... }
for (G4int i = 0; i < 10; i++) { ... }
```

## Проверь себя (ответы → `answers/C1.md`, код → `exercises/cpp/c1_units.cc`)
1. Что напечатает:
   ```cpp
   G4double length = 5*cm;
   G4cout << length << G4endl;
   G4cout << length/mm << G4endl;
   G4cout << length/m << G4endl;
   ```
2. Чему равно `G4double r = 3/4;` и как исправить?
3. Функция `G4double ToKeV(G4double energy)`.
4. Цикл, печатающий 1, 2, 4, 8, 16 МэВ.
