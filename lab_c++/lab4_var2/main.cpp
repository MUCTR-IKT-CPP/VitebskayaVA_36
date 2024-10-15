// Вариант 2

// Создайте класс Кот, Собака, Вомбат со свойствами: имя, возраст, сытость (от 0 до 100), усталость.
// Создайте класс Животное, который будет хранить указатель на пустоту, за которым скрывается конкретное животное, а так же его тип.
// Создайте класс Контактный зоопарк, который будет хранить в себе массив животных.

// Заполните зоопарк случайными животными в количестве введенным пользователем, так что бы было хотя бы по одному животному одного вида.
// Добавьте методы у зоопарка для добавления одного нового животного которого введет пользователь.

// (Усложнение) Смоделируйте работу смотрителя зоопарка в течении одного рабочего дня(12 часов), задача которого наблюдать за животными, 
// в случае если животное утомилось (усталость > 80), он отправляет его в отдельный вольер спать(скорость восстановления определяется типом животного), 
// если животное хочет кушать, он отправляется на фиксированное время так же в отдельный вольер. 
// Наработка усталости и уменьшения сытости обоих параметров определяется типом животного и интенсивностью посещения зоопарка в текущий момент. 
// Интенсивность меняется каждый час работы в диапазоне от 0 до 1.

#include<iostream>
#include <vector>
#include <ctime>

using namespace std;

class Cat {
public:
    string name;     // Имя кота
    int age;         // Возраст кота
    int fullness;    // Сытость (от 0 до 100)
    int tiredness;   // Усталость (от 0 до 100)

    /**
     * @brief Конструктор кота.
     * @param name Имя кота
     * @param age Возраст кота
     */
    Cat(string name, int age) : name(name), age(age), fullness(100), tiredness(0) {}

    /**
     * @brief Метод, моделирующий течение времени и изменяющий усталость и сытость кота.
     * @param intensity Интенсивность посещений зоопарка (от 0 до 1)
     */
    void passTime(float intensity) {
        fullness -= 10 * intensity;
        tiredness += 10 * intensity;
    }

    /**
     * @brief Метод, который уменьшает усталость кота (отдых).
     */
    void rest() {
        tiredness -= 20;
        if (tiredness < 0) tiredness = 0;
    }

    /**
     * @brief Метод, который восстанавливает сытость кота.
     */
    void eat() {
        fullness = 100;
    }
};

class Dog {
public:
    string name;     
    int age;         
    int fullness;    
    int tiredness;   

    /**
     * @brief Конструктор собаки.
     * @param name Имя собаки
     * @param age Возраст собаки
     */
    Dog(string name, int age) : name(name), age(age), fullness(100), tiredness(0) {}

    /**
     * @brief Метод, моделирующий течение времени и изменяющий усталость и сытость собаки.
     * @param intensity Интенсивность посещений зоопарка (от 0 до 1)
     */
    void passTime(float intensity) {
        fullness -= 15 * intensity;
        tiredness += 20 * intensity;
    }

    /**
     * @brief Метод, который уменьшает усталость собаки (отдых).
     */
    void rest() {
        tiredness -= 40;
        if (tiredness < 0) tiredness = 0;
    }

    /**
     * @brief Метод, который восстанавливает сытость собаки.
     */
    void eat() {
        fullness = 100;
    }
};

class Wombat {
public:
    string name;     
    int age;         
    int fullness;    
    int tiredness;   

    /**
     * @brief Конструктор вомбата.
     * @param name Имя вомбата
     * @param age Возраст вомбата
     */
    Wombat(string name, int age) : name(name), age(age), fullness(100), tiredness(0) {}

    /**
     * @brief Метод, моделирующий течение времени и изменяющий усталость и сытость вомбата.
     * @param intensity Интенсивность посещений зоопарка (от 0 до 1)
     */
    void passTime(float intensity) {
        fullness -= 20 * intensity;
        tiredness += 20 * intensity;
    }

    /**
     * @brief Метод, который уменьшает усталость вомбата (отдых).
     */
    void rest() {
        tiredness -= 30;
        if (tiredness < 0) tiredness = 0;
    }

    /**
     * @brief Метод, который восстанавливает сытость вомбата.
     */
    void eat() {
        fullness = 100;
    }
};

class Animal {
public:
    void* animal;    // Указатель на конкретное животное (Cat, Dog, Wombat)
    string type;     // Тип животного (Cat, Dog, Wombat)

    /**
     * @brief Конструктор для создания животного.
     * @param animal Указатель на объект животного
     * @param type Тип животного (строка)
     */
    Animal(void* animal, string type) : animal(animal), type(type) {}
};

class Zoo {
public:
    vector<Animal> animals;  // Массив животных в зоопарке

    /**
     * @brief Метод для добавления животного в зоопарк.
     * @param animal Животное для добавления
     */
    void addAnimal(Animal animal) {
        animals.push_back(animal);
    }

    /**
     * @brief Метод для добавления нового животного от пользователя.
     */
    void addAnimalFromUser() {
        string type;
        cout << "Enter type (Cat, Dog, Wombat): ";
        cin >> type;

        if (type == "Cat" || type == "Dog" || type == "Wombat") {
            string name;
            cout << "Enter name: ";
            cin >> name;

            int age;
            cout << "Enter age: ";
            cin >> age;

            if (type == "Cat") {
                animals.push_back(Animal(new Cat(name, age), "Cat"));
            } else if (type == "Dog") {
                animals.push_back(Animal(new Dog(name, age), "Dog"));
            } else {
                animals.push_back(Animal(new Wombat(name, age), "Wombat"));
            }
        } else {
            cout << "Wrong type! Try again..." << endl;
        }
    }

    /**
     * @brief Метод для вывода всех животных в зоопарке.
     */
    void printZoo() {
        for (const auto& animal : animals) {
            if (animal.type == "Cat") {
                Cat* cat = static_cast<Cat*>(animal.animal);
                cout << "cat " << cat->name << ": age " << cat->age << endl;
            } else if (animal.type == "Dog") {
                Dog* dog = static_cast<Dog*>(animal.animal);
                cout << "dog " << dog->name << ": age " << dog->age << endl;
            } else if (animal.type == "Wombat") {
                Wombat* wombat = static_cast<Wombat*>(animal.animal);
                cout << "wombat " << wombat->name << ": age " << wombat->age << endl;
            }
        }
    }

    /**
     * @brief Метод для моделирования одного рабочего дня зоопарка (12 часов).
     */
    void simulateDay() {
        srand(time(0));

        for (int hour = 1; hour <= 12; ++hour) {
            double intensity = rand() / (RAND_MAX + 1.0); // Генерация случайной интенсивности от 0 до 1
            cout << "\nIntensity hour " << hour << ": " << intensity << endl;

            for (auto& animal : animals) {
                if (animal.type == "Cat") {
                    Cat* cat = static_cast<Cat*>(animal.animal);
                    cat->passTime(intensity);
                    checkAnimal(cat, "Cat");
                } else if (animal.type == "Dog") {
                    Dog* dog = static_cast<Dog*>(animal.animal);
                    dog->passTime(intensity);
                    checkAnimal(dog, "Dog");
                } else if (animal.type == "Wombat") {
                    Wombat* wombat = static_cast<Wombat*>(animal.animal);
                    wombat->passTime(intensity);
                    checkAnimal(wombat, "Wombat");
                }
            }
        }
    }

    /**
     * @brief Метод для проверки состояния животного (сытость и усталость).
     * @tparam T Тип животного (Cat, Dog, Wombat)
     * @param animal Указатель на животное
     * @param animalType Строка с типом животного
     */
    template <typename T>
    void checkAnimal(T* animal, const string& animalType) {
        if (animal->tiredness > 80) {
            cout << animalType << " " << animal->name << " tired. Sent to the enclosure for rest." << endl;
            animal->rest();
        }
        if (animal->fullness < 30) {
            cout << animalType << " " << animal->name << " hungry. Sent to the enclosure for feeding for one hour." << endl;
            animal->eat();
        }
    }
};

int main() {
    Zoo zoo;

    // Запрашиваем у пользователя количество животных
    int N = 0;
    while (N < 3) {
        cout << "Enter number of animals(>3):" << endl;
        cin >> N;
    }

    // Добавляем хотя бы по одному животному каждого типа
    zoo.addAnimal(Animal(new Cat("Fluffy", rand() % 10 + 1), "Cat"));
    zoo.addAnimal(Animal(new Dog("Oatmeal", rand() % 10 + 1), "Dog"));
    zoo.addAnimal(Animal(new Wombat("Kuzmich", rand() % 10 + 1), "Wombat"));

    // Заполняем оставшихся животных случайно
    int n = N - 3;
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        int animalType = rand() % 3; // 0 - Cat, 1 - Dog, 2 - Wombat
        if (animalType == 0) {
            zoo.addAnimal(Animal(new Cat("Cat_" + to_string(i + 1), rand() % 10 + 1), "Cat"));
        } else if (animalType == 1) {
            zoo.addAnimal(Animal(new Dog("Dog_" + to_string(i + 1), rand() % 10 + 1), "Dog"));
        } else {
            zoo.addAnimal(Animal(new Wombat("Wombat_" + to_string(i + 1), rand() % 10 + 1), "Wombat"));
        }
    }

    zoo.printZoo();

    // Возможность добавления животных от пользователя
    char choice;
    cout << "Want to add a new animal? (y/n): ";
    cin >> choice;

    while (choice == 'y') {
        zoo.addAnimalFromUser();
        cout << "Want to add a new animal? (y/n): ";
        cin >> choice;
    }

    zoo.printZoo();

    // Моделирование дня в зоопарке
    zoo.simulateDay();

    return 0;
}


