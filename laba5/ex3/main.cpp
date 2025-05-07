#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "tram_manager.h"

using namespace std;

// Enum класс для представления команд
enum class Command {
    CREATE_TRAM,
    TRAMS_IN_STOP,
    STOPS_IN_TRAM,
    TRAMS,
    EXIT,
    UNKNOWN
};

// Функция для преобразования строки в команду
Command parseCommand(const string& input) {
    static const map<string, Command> commandMap = {
        {"CREATE_TRAM", Command::CREATE_TRAM},
        {"TRAMS_IN_STOP", Command::TRAMS_IN_STOP},
        {"STOPS_IN_TRAM", Command::STOPS_IN_TRAM},
        {"TRAMS", Command::TRAMS},
        {"EXIT", Command::EXIT}
    };

    string upperInput = input;
    transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);
    
    auto it = commandMap.find(upperInput);
    return (it != commandMap.end()) ? it->second : Command::UNKNOWN;
}

void printHelp() {
    cout << "Available commands:\n"
         << "CREATE_TRAM <name> <stop1> <stop2> ...\n"
         << "TRAMS_IN_STOP <stop>\n"
         << "STOPS_IN_TRAM <tram>\n"
         << "TRAMS\n"
         << "EXIT\n";
}

void processCommand(Command cmd, istringstream& iss, TramManager& manager) {
    switch(cmd) {
        case Command::CREATE_TRAM: {
            string tramName;
            if (!(iss >> tramName)) {
                cerr << "ERROR: Missing tram name\n";
                return;
            }
            
            vector<string> stops;
            string stop;
            while (iss >> stop) {
                stops.push_back(stop);
            }
            
            string error = manager.createTram(tramName, stops);
            if (!error.empty()) {
                cerr << error << "\n";
            }
            break;
        }
        
        case Command::TRAMS_IN_STOP: {
            string stop;
            if (!(iss >> stop)) {
                cerr << "ERROR: Missing stop name\n";
                return;
            }
            
            auto trams = manager.getTramsInStop(stop);
            if (trams.empty()) {
                cout << "No trams in stop " << stop << "\n";
            } else {
                cout << "Trams in stop " << stop << ": ";
                for (const auto& tram : trams) {
                    cout << tram << " ";
                }
                cout << "\n";
            }
            break;
        }
        
        case Command::STOPS_IN_TRAM: {
            string tram;
            if (!(iss >> tram)) {
                cerr << "ERROR: Missing tram name\n";
                return;
            }
            
            auto stopsInfo = manager.getStopsInTram(tram);
            if (stopsInfo.empty()) {
                cout << "Tram " << tram << " not found\n";
            } else {
                cout << "Stops for tram " << tram << ":\n";
                for (const auto& [stop, trams] : stopsInfo) {
                    cout << " - " << stop << ": ";
                    if (trams.empty()) {
                        cout << "no other trams";
                    } else {
                        for (const auto& t : trams) {
                            cout << t << " ";
                        }
                    }
                    cout << "\n";
                }
            }
            break;
        }
        
        case Command::TRAMS: {
            auto allTrams = manager.getAllTrams();
            if (allTrams.empty()) {
                cout << "No trams registered\n";
            } else {
                cout << "All trams:\n";
                for (const auto& [tram, stops] : allTrams) {
                    cout << " - " << tram << ": ";
                    for (const auto& stop : stops) {
                        cout << stop << " ";
                    }
                    cout << "\n";
                }
            }
            break;
        }
        
        case Command::EXIT:
        case Command::UNKNOWN:
            break;
    }
}

int main() {
    TramManager manager;
    string line;
    
    cout << "Tram Management System (type 'exit' to quit)\n";
    while (true) {
        cout << "> ";
        getline(cin, line);
        if (line.empty()) continue;
        
        istringstream iss(line);
        string inputCmd;
        iss >> inputCmd;
        
        Command cmd = parseCommand(inputCmd);
        
        if (cmd == Command::EXIT) break;
        if (cmd == Command::UNKNOWN) {
            cerr << "ERROR: Unknown command\n";
            printHelp();
            continue;
        }
        
        processCommand(cmd, iss, manager);
    }
    
    return 0;
}
