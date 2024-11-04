#include <iostream>
#include <vector>
#include <ctime>
#include <random>

// Класс Tower (Башня)
class Tower {
public:
    int health; // Прочность башни
    int damage; // Урон башни

    Tower() : damage(0), health(100) {}

    /**
     * @brief Выполняет атаку башни.
     * @return Урон, нанесенный башней.
     */
    int attack() {
        int att = rand() % 31 + 20;
        damage += att;
        return att;
    }

    /**
     * @brief Проверяет, жива ли башня.
     * @return true, если прочность башни больше 0; false, если башня разрушена.
     */
    bool isAlive() const {
        return health > 0;
    }

    /**
     * @brief Уменьшает здоровье башни в зависимости от нанесенного урона.
     * @param dragon_damage Урон, полученный от атаки дракона.
     */
    void getTowerDamage(int dragon_damage) {
        health = (health - dragon_damage < 0) ? 0 : (health - dragon_damage);
    }
};

// Класс Castle (Замок), состоящий из башен
class Castle {
public:
    int numTowers;
    std::vector<Tower> towers;
    int allCastelDamage;

    Castle() {
        numTowers = rand() % 6 + 3;
        towers.resize(numTowers);
        allCastelDamage = 0;
    }

    /**
     * @brief Уменьшает прочность указанных башен в соответствии с переданными значениями урона.
     * @param dragon_damage Массив с уроном от каждой головы дракона.
     * @param tower_numbers Массив с номерами башен, на которые направлен урон.
     */
    void getCastleDamage(int dragon_damage[], int tower_numbers[]) {
        int len = 3;
        for (int i = 0; i < len; i++) {
            towers[tower_numbers[i]].getTowerDamage(dragon_damage[i]);
        }
    }

    /**
     * @brief Выполняет атаку замка, атакует указанные башни.
     * @param tower_numbers Массив с номерами башен, которые будут атаковать.
     * @return Общий урон, нанесенный замком.
     */
    int castleAttack(int tower_numbers[]) {
        int total_damage = 0;
        int len = 3;
        for (int i = 0; i < len; i++) {
            if (tower_numbers[i] >= 0 && tower_numbers[i] < numTowers && towers[tower_numbers[i]].isAlive()) {
                int damage = towers[tower_numbers[i]].attack();
                total_damage += damage;
            }
        }
        allCastelDamage += total_damage;
        return total_damage;
    }

    int getAliveTowers(){
        int aliveTowers = numTowers;
        for (int i = 0; i < numTowers; i++) {
            aliveTowers = (towers[i].health <= 0) ? --aliveTowers : aliveTowers;
        }
        return aliveTowers;
    }
    
    void getCastelStatistic(){
        std::cout << "All Castle damage: " << allCastelDamage << std::endl; 
        for (int i = 0; i < numTowers; i++) {
            std::cout << "Damage of tower " << i << ": " << towers[i].damage << std::endl;
        }
        std::cout << "Alive towers: " << getAliveTowers() << std::endl;
    }
};

// Класс Dragon (Дракон)
class Dragon {
public:
    int health;
    int headDamage[3] = {0, 0, 0};  // Урон каждой из трех голов
    int totalDamage; // Суммарный урон дракона

    Dragon() : health(625), totalDamage(0) {}

    /**
     * @brief Выбирает центральную башню для атаки.
     * @param numTowers Количество башен в замке.
     * @return Номер выбранной башни.
     */
    int mainTower(int numTowers) {
        return rand() % numTowers;
    }

    /**
     * @brief Атакует три башни и наносит урон каждой.
     * @param dragonAttack Массив для хранения урона от каждой головы.
     */
    void attackTowers(int dragonAttack[]) {
        int len = 3;
        for (int i = 0; i < len; i++) {
            int damage = rand() % 31 + 20;
            dragonAttack[i] = damage;
            headDamage[i] += damage;
            totalDamage += damage;
        }
    }

    /**
     * @brief Уменьшает здоровье дракона на величину урона, полученного от замка.
     * @param castle_damage Урон, нанесенный замком.
     */
    void getTowerDamage(int castle_damage) {
        health = (health - castle_damage < 0) ? 0 : (health - castle_damage);
    }

    /**
     * @brief Проверяет, жив ли дракон.
     * @return true, если здоровье дракона больше 0; false, если дракон погиб.
     */
    bool isAlive() const {
        return health > 0;
    }

 
    void getDragonStatistic(){
        std::cout << "All Dragon damage: " << totalDamage << std::endl;
        int len = 3; 
        for (int i = 0; i < len; i++) {
            std::cout << "Damage of dragon head " << i << ": " << headDamage[i] << std::endl;
        }
        std::cout << "Dragon health: " << health << std::endl;
    }
};

   
/**
 * @brief Моделирует бой по первой стратегии.
 * @param castle Ссылка на объект замка.
 * @param dragon Ссылка на объект дракона.
 * @param round Ссылка на счетчик раундов.
 */
void simulateBattleStrategyFirst(Castle &castle, Dragon &dragon, int &round) {
    std::cout << "Starting Battle - Strategy 1" << std::endl;
    while (dragon.isAlive() && castle.getAliveTowers() > 0) {
        
        int towers_numbers[3];
        towers_numbers[1] = dragon.mainTower(castle.numTowers);
        if (castle.towers[towers_numbers[1]].isAlive()) {
            ++round;
            towers_numbers[0] = (towers_numbers[1] == 0) ? castle.numTowers - 1 : towers_numbers[1] - 1;
            towers_numbers[2] = (towers_numbers[1] == castle.numTowers - 1) ? 0 : towers_numbers[1] + 1;

            int dragonAttack[3];
            dragon.attackTowers(dragonAttack);

            castle.getCastleDamage(dragonAttack, towers_numbers);

            int castle_damage = castle.castleAttack(towers_numbers);
            dragon.getTowerDamage(castle_damage);
        }
    }
    (dragon.isAlive() == false) ? std::cout << "Sir, the Dragon is dead!\n\n" : std::cout << "Sir, Towers fell!\n\n";

    std::cout << "End of Battle - Strategy 1" << std::endl;
}

/**
 * @brief Моделирует бой по второй стратегии.
 * @param castle Ссылка на объект замка.
 * @param dragon Ссылка на объект дракона.
 * @param round Ссылка на счетчик раундов.
 */
void simulateBattleStrategySecond(Castle &castle, Dragon &dragon, int &round) {
    std::cout << "Starting Battle - Strategy 2" << std::endl;
    while (dragon.isAlive() && castle.getAliveTowers() > 0) {
        
        int towers_numbers[3];
        towers_numbers[1] = dragon.mainTower(castle.numTowers);

        if (castle.towers[towers_numbers[1]].isAlive()) {

            towers_numbers[0] = (towers_numbers[1] == 0) ? castle.numTowers - 1 : towers_numbers[1] - 1;
            towers_numbers[2] = (towers_numbers[1] == castle.numTowers - 1) ? 0 : towers_numbers[1] + 1;

            while (castle.towers[towers_numbers[1]].isAlive() && dragon.isAlive()) {
                ++round;
                int dragonAttack[3];
                dragon.attackTowers(dragonAttack);
                castle.getCastleDamage(dragonAttack, towers_numbers);

                int castle_damage = castle.castleAttack(towers_numbers);
                dragon.getTowerDamage(castle_damage);
            }
        }
    }
    (dragon.isAlive() == false) ? std::cout << "Sir, the Dragon is dead!\n\n" : std::cout << "Sir, Towers fell!\n\n";

    std::cout << "End of Battle - Strategy 2" << std::endl;
}

/**
 * @brief Выводит статистику после завершения боя.
 * @param castle Ссылка на объект замка.
 * @param dragon Ссылка на объект дракона.
 * @param round Количество раундов.
 */
void statistic(Castle &castle, Dragon &dragon, int round){
    dragon.getDragonStatistic();
    castle.getCastelStatistic();
    std::cout << "Number of rounds: " << round << std::endl;
}

int main() {
    srand(time(0));

    Castle castle;
    Dragon dragon;
    int rounds = 0;

    simulateBattleStrategyFirst(castle, dragon, rounds);
    statistic(castle, dragon, rounds);

    Castle castle1;
    Dragon dragon1;
    int rounds1 = 0;

    simulateBattleStrategySecond(castle1, dragon1, rounds1);
    statistic(castle1, dragon1, rounds1);

    return 0;
}
