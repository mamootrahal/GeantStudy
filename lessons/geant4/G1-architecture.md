# G1. Что такое Geant4 и как он устроен

Geant4 (GEometry ANd Tracking) — C++-библиотека (toolkit) для Монте-Карло моделирования
прохождения частиц через вещество. Это не программа: вы пишете свою `main()`, а Geant4 даёт кирпичики.

## Иерархия Run → Event → Track → Step
```
Run (сеанс)
 └── Event (событие) × N         ← одна первичная частица + каскад
      └── Track (трек) × M       ← история одной частицы
           └── Step (шаг) × K    ← элементарный отрезок движения
```
Шаг заканчивается, когда случается физический процесс или частица пересекает границу объёма.

## Обязательные классы
| Вопрос | Класс |
|---|---|
| Где? | `G4VUserDetectorConstruction` — геометрия и материалы |
| Какая физика? | `G4VUserPhysicsList` (обычно готовый: `FTFP_BERT`, `QGSP_BIC`) |
| Что летит? | `G4VUserPrimaryGeneratorAction` (`G4ParticleGun`) |

## Необязательные user actions
`G4UserRunAction`, `G4UserEventAction`, `G4UserTrackingAction`, `G4UserSteppingAction`, `G4UserStackingAction`.
Всем управляет `G4RunManager`. `ActionInitialization` создаёт actions (нужно для многопоточности).

## Схема main()
```cpp
int main() {
    auto* runManager = new G4RunManager;
    runManager->SetUserInitialization(new MyDetectorConstruction);
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager->SetUserInitialization(new MyActionInitialization);
    runManager->Initialize();
    runManager->BeamOn(1000);
    delete runManager;
}
```

## Мелочи
- Внутренние единицы: **мм, МэВ, нс**. Пишем `10*cm`, выводим `edep/keV`.
- Cuts задаются **длиной пробега** (по умолчанию 0.7 мм), а не энергией.

## Проверь себя
1. Чем отличается Track от Event?
2. Какие 3 класса обязательны?
3. В каком action накапливать энергию за событие и где её выводить?
