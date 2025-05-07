#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct Cell {
    string item;
    int quantity;
};

map<string, Cell> warehouse;

void initializeWarehouse() {
    vector<char> zones = {'A', 'B', 'C'}; // Исправлены кириллические символы
    for (char zone : zones) {
        for (int shelf = 1; shelf <= 20; ++shelf) {
            for (int section = 1; section <= 5; ++section) {
                for (int rack = 1; rack <= 2; ++rack) {
                    // Формирование адреса без ведущего нуля для стеллажей
                    string address = string(1, zone) 
                                   + to_string(shelf) 
                                   + to_string(section) 
                                   + to_string(rack);
                    warehouse[address] = {"", 0};
                }
            }
        }
    }
}

void processAddCommand(const vector<string>& tokens) {
    if (tokens.size() != 4) {
        cout << "Ошибка: неверный формат команды. Используйте: ADD <товар> <количество> <адрес>" << endl;
        return;
    }
    
    string item = tokens[1];
    int quantity;
    try {
        quantity = stoi(tokens[2]);
        if (quantity <= 0) throw invalid_argument("Ошибка");
    } catch (...) {
        cout << "Ошибка: количество должно быть положительным числом" << endl;
        return;
    }
    
    string address = tokens[3];
    
    if (warehouse.find(address) == warehouse.end()) {
        cout << "Ошибка: ячейка " << address << " не существует" << endl;
        return;
    }

    Cell& cell = warehouse[address];
    if (cell.quantity == 0) {
        if (quantity > 10) {
            cout << "Ошибка: в ячейку нельзя добавить более 10 единиц" << endl;
            return;
        }
        cell.item = item;
        cell.quantity = quantity;
        cout << "Добавлено " << quantity << " ед. товара \"" << item << "\" в " << address << endl;
    } else {
        if (cell.item != item) {
            cout << "Ошибка: ячейка содержит другой товар (\"" << cell.item << "\")" << endl;
            return;
        }
        if (cell.quantity + quantity > 10) {
            cout << "Ошибка: превышен лимит ячейки. Доступно место: " << 10 - cell.quantity << endl;
            return;
        }
        cell.quantity += quantity;
        cout << "Обновлено: " << item << " x" << cell.quantity << " в " << address << endl;
    }
}

void processRemoveCommand(const vector<string>& tokens) {
    if (tokens.size() != 4) {
        cout << "Ошибка: неверный формат команды. Используйте: REMOVE <товар> <количество> <адрес>" << endl;
        return;
    }
    
    string item = tokens[1];
    int quantity;
    try {
        quantity = stoi(tokens[2]);
        if (quantity <= 0) throw invalid_argument("Ошибка");
    } catch (...) {
        cout << "Ошибка: количество должно быть положительным числом" << endl;
        return;
    }
    
    string address = tokens[3];
    
    if (warehouse.find(address) == warehouse.end()) {
        cout << "Ошибка: ячейка " << address << " не существует" << endl;
        return;
    }

    Cell& cell = warehouse[address];
    if (cell.quantity == 0) {
        cout << "Ошибка: ячейка " << address << " пуста" << endl;
        return;
    }
    if (cell.item != item) {
        cout << "Ошибка: в ячейке находится другой товар (\"" << cell.item << "\")" << endl;
        return;
    }
    if (cell.quantity < quantity) {
        cout << "Ошибка: недостаточно товара. Доступно: " << cell.quantity << endl;
        return;
    }
    
    cell.quantity -= quantity;
    if (cell.quantity == 0) cell.item = "";
    cout << "Удалено " << quantity << " ед. товара \"" << item << "\" из " << address 
         << ". Остаток: " << cell.quantity << endl;
}

void processInfoCommand() {
    int totalItems = 0;
    map<char, int> zoneItems;
    vector<string> occupiedCells, emptyCells;

    for (const auto& [address, cell] : warehouse) {
        if (cell.quantity > 0) {
            totalItems += cell.quantity;
            zoneItems[address[0]] += cell.quantity;
            occupiedCells.push_back(address + ": \"" + cell.item + "\" x" + to_string(cell.quantity));
        } else {
            emptyCells.push_back(address);
        }
    }

    cout << "Общая загруженность: " 
         << fixed << setprecision(2) 
         << (totalItems / 6000.0 * 100) << "%\n";

    vector<char> zones = {'A', 'B', 'C'};
    for (char zone : zones) {
        double percent = (zoneItems[zone] / 2000.0) * 100;
        cout << "Зона " << zone << ": " << percent << "%\n";
    }

    cout << "\nЗаполненные ячейки (" << occupiedCells.size() << "):\n";
    for (const string& s : occupiedCells) cout << "  " << s << endl;

    cout << "\nПустые ячейки (" << emptyCells.size() << "):\n";
    for (const string& s : emptyCells) cout << "  " << s << endl;
}

int main() {
    initializeWarehouse();
    string line;

    cout << "=== Система управления складом ===\n"
         << "Доступные команды:\n"
         << "  ADD <товар> <кол-во> <адрес>\n"
         << "  REMOVE <товар> <кол-во> <адрес>\n"
         << "  INFO\n"
         << "Для выхода введите Ctrl+C\n\n";

    while (true) {
        cout << "> ";
        getline(cin, line);
        if (line.empty()) continue;

        istringstream iss(line);
        vector<string> tokens;
        string token;
        while (iss >> token) tokens.push_back(token);

        if (tokens.empty()) continue;

        string cmd = tokens[0];
        if (cmd == "ADD") processAddCommand(tokens);
        else if (cmd == "REMOVE") processRemoveCommand(tokens);
        else if (cmd == "INFO") processInfoCommand();
        else cout << "Ошибка: неизвестная команда \"" << cmd << "\"\n";
    }

    return 0;
}
