#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <ctime>

/**
 * @brief Абстрактный класс для оружия.
 * Этот класс служит базовым для различных типов оружия.
 */
class Weapon {
public:
    std::string name;        // Название оружия
    double caliber;          // Калибр оружия
    int maxAmmo;             // Максимальное количество патронов в магазине
    int currentAmmo;         // Текущее количество патронов
    double reloadTime;       // Время перезарядки
    int fireModes;           // Количество режимов стрельбы
    double weight;           // Вес оружия
    double effectiveRange;   // Прицельная дальность
    int accuracy;            // Кучность стрельбы
    int modifications;       // Количество устанавливаемых модификаций

    /**
     * @brief Конструктор для создания оружия.
     * @param n Название оружия.
     * @param cal Калибр оружия.
     * @param maxA Максимальное количество патронов.
     * @param reloadT Время перезарядки.
     * @param fireM Количество режимов стрельбы.
     * @param wt Вес оружия.
     * @param effR Прицельная дальность.
     * @param acc Кучность стрельбы.
     * @param mods Количество модификаций.
     */
    Weapon(std::string n, double cal, int maxA, double reloadT, int fireM, double wt, double effR, int acc, int mods)
        : name(n), caliber(cal), maxAmmo(maxA), reloadTime(reloadT), fireModes(fireM), weight(wt), effectiveRange(effR), accuracy(acc), modifications(mods), currentAmmo(maxA) {}

    /**
     * @brief Перезарядка оружия.
     */
    virtual double reload() = 0;

    /**
     * @brief Стрельба по мишени.
     * @param distance Расстояние до мишени.
     * @param mode Режим стрельбы.
     */
    virtual int fire(double distance, int mode) = 0;

    /**
     * @brief Вывод характеристик оружия.
     */
    void displayCharacteristics() {
        std::cout << "Weapon: " << name << "\n"
                  << "Caliber: " << caliber << "mm\n"
                  << "Max Ammo: " << maxAmmo << "\n"
                  << "Reload Time: " << reloadTime << " sec\n"
                  << "Weight: " << weight << " kg\n"
                  << "Effective Range: " << effectiveRange << " m\n"
                  << "Accuracy: " << accuracy << "%\n"
                  << "Modifications: " << modifications << "\n\n";
    }

    /**
     * @brief Рассчитывает шанс попадания по мишени.
     * @param distance Расстояние до мишени.
     * @return Шанс попадания в процентах.
     */
    double calculateHitChance(double distance) const {
        double ratio = distance / effectiveRange;
        return (ratio < 0.2) ? 100 : (100 * std::exp(-10 * ratio));
    }
};

/**
 * @brief Класс Пистолет.
 */
class Pistol : public Weapon {
public:
    Pistol() : Weapon("Pistol", 9.0, 15, 1.5, 1, 1.2, 50, 75, 2) {}

    /**
     * @brief Перезарядка пистолета.
     * @return Время перезарядки.
     */
    double reload() override {
        currentAmmo = maxAmmo;
        return reloadTime;
    }

    /**
     * @brief Стрельба из пистолета.
     * @param distance Расстояние до мишени.
     * @param mode Режим стрельбы.
     * @return Возвращает 1 при попадании, 0 при промахе.
     */
    int fire(double distance, int mode) override {
        if (currentAmmo <= 0) return 0;
        --currentAmmo;
        double hitChance = calculateHitChance(distance);
        return (rand() % 100) < hitChance ? 1 : 0;
    }
};

/**
 * @brief Класс Однозарядная винтовка.
 */
class BoltActionRifle : public Weapon {
public:
    BoltActionRifle() : Weapon("Bolt-Action Rifle", 7.62, 1, 2.5, 1, 4.0, 800, 90, 3) {}

    /**
     * @brief Перезарядка однозарядной винтовки.
     * @return Время перезарядки.
     */
    double reload() override {
        currentAmmo = 1;
        return reloadTime;
    }

    /**
     * @brief Стрельба из однозарядной винтовки.
     * @param distance Расстояние до мишени.
     * @param mode Режим стрельбы.
     * @return Возвращает 1 при попадании, 0 при промахе.
     */
    int fire(double distance, int mode) override {
        if (currentAmmo <= 0) return 0;
        --currentAmmo;
        double hitChance = calculateHitChance(distance);
        return (rand() % 100) < hitChance ? 1 : 0;
    }
};

/**
 * @brief Класс Помповый дробовик.
 */
class PumpShotgun : public Weapon {
public:
    PumpShotgun() : Weapon("Pump Shotgun", 12.0, 8, 0.5, 1, 3.5, 30, 70, 1) {}

    /**
     * @brief Перезарядка помпового дробовика.
     * @return Время перезарядки с учетом загрузки каждого патрона.
     */
    double reload() override {
        currentAmmo = maxAmmo;
        return reloadTime * maxAmmo;
    }

    /**
     * @brief Стрельба из помпового дробовика.
     * @param distance Расстояние до мишени.
     * @param mode Режим стрельбы.
     * @return Возвращает 1 при попадании, 0 при промахе.
     */
    int fire(double distance, int mode) override {
        if (currentAmmo <= 0) return 0;
        --currentAmmo;
        double hitChance = calculateHitChance(distance);
        return (rand() % 100) < hitChance ? 1 : 0;
    }
};

/**
 * @brief Класс стрельбища, который содержит мишени и симулирует стрельбу.
 */
class ShootingRange {
public:
    std::vector<double> targets;  // Мишени с разными дистанциями

    /**
     * @brief Конструктор для создания стрельбища с мишенями.
     */
    ShootingRange() {
        for (int i = 10; i <= 1000; i *= 2) {
            targets.push_back(i);
        }
    }

    /**
     * @brief Симуляция стрельбы по мишеням.
     * @param weapon Оружие, которое будет тестироваться.
     */
    void simulate(Weapon& weapon) {
        std::cout << "\n------------------------------\n";
        std::cout << "Testing " << weapon.name << "...\n";
        weapon.displayCharacteristics();

        for (double distance : targets) {
            int hits = 0;
            int totalShots = 100;
            double totalTime = 0;

            for (int i = 0; i < totalShots; ++i) {
                if (weapon.currentAmmo == 0){
                    totalTime += weapon.reload();
                }
                hits += weapon.fire(distance, 1);
                totalTime += 0.7;               
            }
            double accuracy = (double) hits / totalShots * 100;
            double fireRate = totalShots / totalTime * 60;
            std::cout << "Distance: " << distance << "m - Accuracy: " << accuracy << "% - Fire Rate: " << fireRate << " shots/min\n";
        }
        std::cout << "------------------------------\n";
    }
};

/**
 * @brief Главная функция программы.
 */
int main() {
    srand(time(0));

    Pistol pistol;
    BoltActionRifle rifle;
    PumpShotgun shotgun;

    ShootingRange range;

    range.simulate(pistol);
    range.simulate(rifle);
    range.simulate(shotgun);

    return 0;
}