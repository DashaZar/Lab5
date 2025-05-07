#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>
#include <random>
#include <set>
#include <cctype>

using namespace std;

// Функция для преобразования строки в верхний регистр
string toUpper(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Структура, представляющая билет
struct Ticket {
    string number;    // Номер билета (например, "T123")
    int duration;     // Время обработки билета в минутах
    
    // Оператор для сравнения билетов по времени обработки (для сортировки)
    bool operator>(const Ticket& other) const {
        return duration > other.duration;
    }
};

// Класс системы управления очередями
class QueueSystem {
private:
    int windowsCount;             // Количество окон обслуживания
    vector<Ticket> tickets;       // Все билеты в системе
    vector<vector<Ticket>> windows; // Окна обслуживания (каждое окно - вектор билетов)
    int ticketCounter = 1;        // Счетчик для генерации номеров билетов (не используется в текущей реализации)

    // Генерация случайного номера билета
    string generateTicketNumber() {
        static random_device rd;   // Источник случайных чисел
        static mt19937 gen(rd());  // Генератор случайных чисел
        static uniform_int_distribution<> dis(0, 999); // Распределение от 0 до 999
        
        stringstream ss;
        ss << "T" << setw(3) << setfill('0') << dis(gen); // Форматирование в виде TXXX
        return ss.str();
    }

public:
    // Конструктор, инициализирующий количество окон
    QueueSystem(int numWindows) : windowsCount(numWindows) {
        windows.resize(windowsCount); // Создаем нужное количество окон
    }

    // Добавление нового билета в систему
    void enqueue(int duration) {
        Ticket ticket;
        ticket.number = generateTicketNumber(); // Генерируем номер
        ticket.duration = duration;            // Устанавливаем длительность
        tickets.push_back(ticket);              // Добавляем в общий список
        cout << ">>> " << ticket.number << endl; // Выводим номер билета
    }

    // Распределение билетов по окнам
    void distribute() {
        // Сортируем билеты по убыванию времени обработки
        sort(tickets.begin(), tickets.end(), greater<Ticket>());
        
        // Используем минимальную кучу для отслеживания загрузки окон
        // Храним пары (суммарное время обработки, индекс окна)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
        
        // Инициализируем все окна с нулевой загрузкой
        for (int i = 0; i < windowsCount; ++i) {
            minHeap.push({0, i});
        }
        
        // Распределяем билеты по окнам
        for (const auto& ticket : tickets) {
            // Берем окно с минимальной текущей загрузкой
            auto [currentTime, windowIdx] = minHeap.top();
            minHeap.pop();
            
            // Добавляем билет в это окно
            windows[windowIdx].push_back(ticket);
            // Увеличиваем суммарное время обработки для этого окна
            currentTime += ticket.duration;
            // Возвращаем окно обратно в кучу с обновленным временем
            minHeap.push({currentTime, windowIdx});
        }
        
        // Вычисляем общее время обработки для каждого окна
        vector<int> windowTimes(windowsCount, 0);
        for (int i = 0; i < windowsCount; ++i) {
            for (const auto& ticket : windows[i]) {
                windowTimes[i] += ticket.duration;
            }
        }
        
        // Выводим результаты распределения
        for (int i = 0; i < windowsCount; ++i) {
            if (!windows[i].empty()) {
                cout << ">>> Окно " << (i + 1) << " (" << windowTimes[i] << " минут): ";
                bool first = true;
                // Выводим все билеты для этого окна
                for (const auto& ticket : windows[i]) {
                    if (!first) cout << ", ";
                    cout << ticket.number;
                    first = false;
                }
                cout << endl;
            }
        }
        
        // Находим и выводим максимальное время обработки среди всех окон
        int maxTime = *max_element(windowTimes.begin(), windowTimes.end());
        cout << ">>> Максимальное время обработки: " << maxTime << " минут" << endl;
    }
};

int main() {
    cout << ">>> Введите кол-во окон" << endl;
    cout << "<<< ";
    int windows;
    if (!(cin >> windows)) {
        cout << ">>> Ошибка: введите число" << endl;
        return 1;
    }
    
    // Создаем систему с заданным количеством окон
    QueueSystem system(windows);
    string command;
    
    // Основной цикл обработки команд
    while (true) {
        cout << "<<< ";
        if (!(cin >> command)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        // Приводим команду к верхнему регистру для унификации
        string upperCommand = toUpper(command);
        
        if (upperCommand == "ENQUEUE") {
            int duration;
            if (cin >> duration) {
                // Добавляем новый билет с указанной длительностью
                system.enqueue(duration);
            } else {
                cout << ">>> Ошибка: введите число для длительности" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } 
        else if (upperCommand == "DISTRIBUTE") {
            // Распределяем билеты по окнам и выводим результат
            system.distribute();
            break; // Завершаем работу после распределения
        } 
        else {
            cout << ">>> Неизвестная команда. Используйте ENQUEUE или DISTRIBUTE." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    return 0;
}
