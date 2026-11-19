# C2. Указатели и ссылки

Самая важная тема перед Geant4: в его коде указатели **везде**.
`G4Step* step`, `new G4Box(...)`, `step->GetTrack()` — всё это указатели.

## 1. Память и адрес
Каждая переменная лежит где-то в памяти, и у этого места есть **адрес** (номер ячейки).
```cpp
double e = 2.5;
std::cout << e  << std::endl;   // 2.5             — значение
std::cout << &e << std::endl;   // 0x7ffd5a3c1e08  — адрес (& = «адрес чего-то»)
```

## 2. Указатель — переменная, хранящая адрес
```cpp
double  e = 2.5;
double* p = &e;      // p — «указатель на double», хранит адрес e

std::cout << p  << std::endl;   // 0x7ffd...  — адрес
std::cout << *p << std::endl;   // 2.5        — значение по адресу (* = «разыменовать»)

*p = 7.0;                       // меняем значение ПО АДРЕСУ
std::cout << e << std::endl;    // 7 — e изменилась!
```
Запомнить: **`*` в объявлении** (`double* p`) — «это указатель»;
**`*` перед переменной** (`*p`) — «взять то, на что он указывает».

```
  e  [ 7.0 ]  ← адрес 0x...e08
  p  [ 0x...e08 ]  ──────┘
```

## 3. nullptr — «указывает в никуда»
```cpp
G4LogicalVolume* mother = nullptr;   // пока ни на что не указывает
if (mother != nullptr) { ... }       // или короче: if (mother) { ... }
```
Разыменовать `nullptr` → программа падает (**Segmentation fault**). Самая частая ошибка в Geant4.
Пример: `step->GetPostStepPoint()->GetPhysicalVolume()` равен `nullptr`,
когда частица вылетела из World. Обращение к его имени без проверки = падение.

## 4. Стрелка `->` — доступ к методам объекта через указатель
```cpp
G4Track* track = step->GetTrack();
// step->GetTrack()  — то же самое, что (*step).GetTrack()
```
В Geant4 пишут **цепочки** стрелок: каждый метод возвращает указатель на следующий объект.
```cpp
G4String name = step->GetTrack()->GetDefinition()->GetParticleName();
//              G4Step*  → G4Track*   → G4ParticleDefinition* → G4String
G4double edep = step->GetTotalEnergyDeposit();
```

## 5. Стек и куча: зачем `new`
**Стек** — обычные локальные переменные. Умирают при выходе из `{ }`:
```cpp
void F() {
    double x = 1.;   // создалась
}                    // уничтожилась автоматически
```
**Куча** — объекты, созданные через `new`. Живут, пока их явно не удалят `delete`:
```cpp
double* p = new double(5.);   // создали в куче, получили указатель
delete p;                     // удалили
```

### Почему в Geant4 всё через `new`
Геометрию вы строите внутри функции `Construct()`. Если создать объект на стеке,
он умрёт при выходе из функции — а Geant4 будет пользоваться им всё моделирование.
```cpp
G4VPhysicalVolume* MyDetector::Construct() {
    G4Box box("Box", 10*cm, 10*cm, 10*cm);         // ❌ умрёт в конце Construct()
    auto* solid = new G4Box("Box", 10*cm, 10*cm, 10*cm);   // ✅ живёт дальше
    auto* logic = new G4LogicalVolume(solid, water, "Box");
    new G4PVPlacement(nullptr, G4ThreeVector(), logic, "Box", worldLV, false, 0);
    //  ↑ результат даже не сохраняем: Geant4 сам запомнил объект
    ...
}
```
`auto* solid` — «компилятор, сам выведи тип» (здесь `G4Box*`). Подробнее в C6.

### Кто удаляет? Правило владения
- Геометрию (solids, logical/physical volumes) Geant4 регистрирует в своих хранилищах и **удаляет сам**.
- Всё, что вы отдали в `runManager->SetUserInitialization(...)` / `SetUserAction(...)`, **удаляет RunManager**.
- Вы удаляете только то, что создали сами и никому не отдали — обычно лишь `delete runManager;` в `main()`.

**Не делайте `delete` того, что отдали Geant4** — будет двойное удаление и падение.

## 6. Ссылки — второе имя переменной
```cpp
double  e = 2.5;
double& r = e;    // r — это e под другим именем (& в объявлении = «ссылка»)
r = 4.0;
std::cout << e;   // 4
```
Ссылка всегда к чему-то привязана (не бывает «нулевой»), перепривязать её нельзя.

### Главное применение — аргументы функций
```cpp
void AddByValue(double total, double x) { total += x; }   // меняет КОПИЮ — бесполезно
void AddByRef  (double& total, double x) { total += x; }  // меняет оригинал

double sum = 0.;
AddByValue(sum, 1.);   // sum == 0
AddByRef(sum, 1.);     // sum == 1
```
И для **больших объектов без копирования** (с `const` — «только читаю»):
```cpp
const G4ThreeVector& pos = step->GetPreStepPoint()->GetPosition();
G4cout << pos.x()/cm << G4endl;
```

## 7. Шпаргалка
| Запись | Смысл |
|---|---|
| `T* p` | p — указатель на T |
| `&x` | адрес x |
| `*p` | значение по адресу p |
| `p->Method()` | вызвать метод объекта, на который указывает p |
| `T& r = x` | r — ссылка (второе имя) на x |
| `nullptr` | указатель «в никуда» |
| `new T(...)` | создать в куче, вернуть `T*` |
| `delete p` | удалить созданное через `new` |

`&` и `*` имеют **два смысла**: в объявлении типа (`double* p`, `double& r`) и как операторы (`*p`, `&x`).
Это путает всех — нормально.

## Проверь себя (ответы → `answers/C2.md`, код → `exercises/cpp/c2_pointers.cc`)
1. Что напечатает?
   ```cpp
   double a = 5.;
   double* p = &a;
   double& r = a;
   *p = 10.;
   r = r + 1.;
   std::cout << a << " " << *p << " " << r << std::endl;
   ```
2. Что не так с этой функцией?
   ```cpp
   double* MakeEnergy() {
       double e = 2.;
       return &e;
   }
   ```
3. Напишите `void AddEnergy(double& total, double edep)` — прибавляет edep к total.
4. Напишите `void AddEnergyPtr(double* total, double edep)` — то же через указатель,
   но если `total == nullptr`, ничего не делает.
5. Почему в `Construct()` пишут `new G4Box(...)`, а не `G4Box box(...)`?
6. Чем опасна строка
   `G4String vol = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();`?
