#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

class Spare {
public:
    int repl_cost;              ///< Стоимость замены запасной части
    int cost_of_one_repair;     ///< Стоимость одного ремонта
    int time_for_repair;        ///< Время на ремонт 
    int service_life;           ///< Служебный срок 
    int degree_wear;            ///< Степень износа 

    /**
     * @brief Конструктор класса Spare.
     * @param repC Стоимость замены запасной части
     * @param rC Стоимость одного ремонта
     * @param rT Время на ремонт
     * @param lS Служебный срок
     */
    Spare(int repC, int rC, int rT, int lS)
        : repl_cost(repC), cost_of_one_repair(rC), time_for_repair(rT), service_life(lS), degree_wear(0) {}

    /**
     * @brief Проверяет, сломана ли запчасть.
     * @return true, если запчасть сломана; иначе false.
     */
    virtual bool isBroken() const = 0;

    /**
     * @brief Моделирует работу запчасти, увеличивая степень износа.
     * @param intensity Интенсивность работы запчасти.
     */
    virtual void work(int intensity) = 0;
};

class Val : public Spare {
public:
    /**
     * @brief Конструктор класса Val.
     */
    Val() : Spare(100, 20, 2, 100) {}

    /**
     * @brief Проверяет, сломан ли клапан.
     * @return true, если клапан сломан; иначе false.
     */
    bool isBroken() const override {
        return degree_wear >= service_life;
    }

    /**
     * @brief Моделирует работу клапана, увеличивая степень износа.
     * @param intensity_work Интенсивность работы клапана.
     */
    void work(int intensity_work) override {
        degree_wear += intensity_work * 0.1;
    }
};

class Electricmotor : public Spare {
public:
    /**
     * @brief Конструктор класса Electricmotor.
     */
    Electricmotor() : Spare(200, 50, 3, 200) {}

    /**
     * @brief Проверяет, сломан ли электродвигатель.
     * @return true, если электродвигатель сломан; иначе false.
     */
    bool isBroken() const override {
        return degree_wear >= service_life;
    }

    /**
     * @brief Моделирует работу электродвигателя, увеличивая степень износа.
     * @param intensity_work Интенсивность работы электродвигателя.
     */
    void work(int intensity_work) override {
        degree_wear += intensity_work * 0.2;
    }
};

class ControlPanel : public Spare {
public:
    /**
     * @brief Конструктор класса ControlPanel.
     */
    ControlPanel() : Spare(150, 20, 5, 150) {}

    /**
     * @brief Проверяет, сломана ли панель управления.
     * @return true, если панель управления сломана; иначе false.
     */
    bool isBroken() const override {
        return degree_wear >= service_life;
    }

    /**
     * @brief Моделирует работу панели управления, увеличивая степень износа.
     * @param intensity_work Интенсивность работы панели управления.
     */
    void work(int intensity_work) override {
        degree_wear += intensity_work * 0.15;
    }
};

class CuttingHead : public Spare {
public:
    /**
     * @brief Конструктор класса CuttingHead.
     */
    CuttingHead() : Spare(180, 40, 4, 180) {}

    /**
     * @brief Проверяет, сломан ли резак.
     * @return true, если резак сломан; иначе false.
     */
    bool isBroken() const override {
        return degree_wear >= service_life;
    }

    /**
     * @brief Моделирует работу резака, увеличивая степень износа.
     * @param intensity_work Интенсивность работы резака.
     */
    void work(int intensity_work) override {
        degree_wear += intensity_work * 0.18;
    }
};

class Machine {
public:
    vector<Spare*> tools;      ///< Вектор запасных частей
    int totalRepairCosts = 0;  ///< Общие расходы на ремонт
    int brokenTime = 0;        ///< Время простоя из-за поломок
    int brokenParts = 0;       ///< Количество поломанных частей
    int replParts = 0;         ///< Количество замененных частей

    /**
     * @brief Конструктор класса Machine.
     */
    Machine() : totalRepairCosts(0), brokenTime(0), brokenParts(0) {
        tools.push_back(new Val());
        tools.push_back(new Electricmotor());
        tools.push_back(new ControlPanel());
        tools.push_back(new CuttingHead());
    }


    /**
     * @brief Деструктор класса Machine.
     * Освобождает память, занятую запасными частями.
     */
     ~Machine() {
        for (auto part : tools) {
            delete part;
        }
    }

    /**
     * @brief Метод работы станка: изнашивает запчасти в зависимости от времени и интенсивности.
     * @param intensity Интенсивность работы.
     */
    int work(int intensity) {
        int flag = 1;
        int hours = 0;
        for (auto part : tools) {
            part->work(intensity); 
            if (part->isBroken()) { 
                hours += 4;
                brokenTime += 4; 
                totalRepairCosts += part->repl_cost;
                replParts++;
                part->degree_wear = 0;
                flag = 0;
                break;
            }else if (rand() % 100 == 5) {
                hours += part->time_for_repair;
                brokenTime += part->time_for_repair;
                totalRepairCosts += part->cost_of_one_repair;
                brokenParts++; 
                flag = 0;
                break;
            }
        }
        if (flag == 1){
            hours += 1;
        }
        return hours;
    }
};

int main() {
    srand(time(0));

    int daysInYear = 365;                       // Количество дней в году
    int numberOfMachines = 5;                   // Количество станков
    int allRepl = 0;                            // Общее количество замененных частей
    int allBrok = 0;                            // Общее количество поломок
    int arrBrok[numberOfMachines] = {0};        // Массив поломок для каждого станка
    int arrCost[numberOfMachines] = {0};        // Массив расходов на ремонт для каждого станка
    int allBrokeTime = 0;                       // Общее время простоя
    int arrBrokeTime[numberOfMachines] = {0};   // Массив времени простоя для каждого станка

    std::vector<Machine> machines(numberOfMachines);

    int hours = 12 * 365;
    int h_old = hours;
    int intensity = rand() % 10 + 1;
    while (hours > 0){
        if (h_old - hours >= 30 * 12){
            intensity = rand() % 10 + 1;
            h_old = hours;
        }
        for (auto& machine : machines) {           
            hours -= machine.work(intensity);
        }
    }

    int i = 0;
    for (auto& machine : machines) {           
            allRepl += machine.replParts;
            allBrok += machine.brokenParts;
            arrBrok[i] += machine.brokenParts;
            arrCost[i] += machine.totalRepairCosts;
            allBrokeTime += machine.brokenTime;
            arrBrokeTime[i] += machine.brokenTime;
            i++;
    }

    cout << "One year operation statistics:" << endl;
    cout << "\nNumber of replaced parts: " << allRepl << endl; // сломались по сроку службу
    cout << "\nNumber of breakdowns for each machine:" << endl; // сломались неожиданно
    for (int i = 0; i < numberOfMachines; ++i) {
        cout << i+1 << ". " << arrBrok[i] << endl;
    }
    cout << "\nTotal number of breakdowns for all machines: " << allBrok << endl; // сломались неожиданно
    cout << "\nRepair costs for each machine:" << endl;
    for (int i = 0; i < numberOfMachines; ++i) {
        cout << i+1 << ". " << arrCost[i] << endl;
    }
    cout << "\nTotal downtime of all machines: " << allBrokeTime << " hours" << "(" << allBrokeTime / 24 << " days)" << endl;
    int* maxElement = max_element(arrBrokeTime, arrBrokeTime + numberOfMachines);
    cout << "\nMaximum total downtime of a single machine: " << *maxElement << " hours" << "(" << *maxElement / 24 << " days)" << endl;

    return 0;
}
