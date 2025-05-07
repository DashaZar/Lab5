// tram_manager.h
#ifndef TRAM_MANAGER_H
#define TRAM_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <set>

class TramManager {
public:
    std::string createTram(const std::string& name, const std::vector<std::string>& stops);
    std::vector<std::string> getTramsInStop(const std::string& stop) const;
    std::vector<std::pair<std::string, std::set<std::string>>> getStopsInTram(const std::string& tram) const;
    std::map<std::string, std::vector<std::string>> getAllTrams() const;

private:
    std::map<std::string, std::vector<std::string>> tram_routes_;
    std::map<std::string, std::set<std::string>> stop_trams_;
};

#endif
