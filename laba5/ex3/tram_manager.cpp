// tram_manager.cpp
#include "tram_manager.h"
#include <algorithm>
#include <set>

std::string TramManager::createTram(const std::string& name, const std::vector<std::string>& stops) {
    // Проверка на минимальное количество остановок
    if (stops.size() < 2) {
        return "ERROR: At least two stops required";
    }
    
    // Проверка на повторяющиеся остановки
    std::set<std::string> unique_stops(stops.begin(), stops.end());
    if (unique_stops.size() != stops.size()) {
        return "ERROR: Duplicate stops in route";
    }
    
    // Проверка на существующее имя
    if (tram_routes_.count(name)) {
        return "ERROR: Tram '" + name + "' already exists";
    }
    
    // Проверка на дубликат маршрута
    for (const auto& [existing_name, existing_route] : tram_routes_) {
        if (existing_route == stops) {
            return "ERROR: Route duplicates existing tram '" + existing_name + "'";
        }
    }
    
    // Добавление нового маршрута
    tram_routes_[name] = stops;
    for (const auto& stop : stops) {
        stop_trams_[stop].insert(name);
    }
    
    return "";
}

// Остальные методы остаются без изменений
std::vector<std::string> TramManager::getTramsInStop(const std::string& stop) const {
    auto it = stop_trams_.find(stop);
    return (it != stop_trams_.end()) 
        ? std::vector<std::string>(it->second.begin(), it->second.end())
        : std::vector<std::string>();
}

std::vector<std::pair<std::string, std::set<std::string>>> TramManager::getStopsInTram(const std::string& tram) const {
    std::vector<std::pair<std::string, std::set<std::string>>> result;
    auto tram_it = tram_routes_.find(tram);
    
    if (tram_it != tram_routes_.end()) {
        for (const auto& stop : tram_it->second) {
            std::set<std::string> other_trams;
            if (auto stop_it = stop_trams_.find(stop); stop_it != stop_trams_.end()) {
                std::copy_if(stop_it->second.begin(), stop_it->second.end(),
                            std::inserter(other_trams, other_trams.end()),
                            [&tram](const auto& name) { return name != tram; });
            }
            result.emplace_back(stop, other_trams);
        }
    }
    return result;
}

std::map<std::string, std::vector<std::string>> TramManager::getAllTrams() const {
    return tram_routes_;
}
