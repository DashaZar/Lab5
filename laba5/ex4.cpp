#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <limits> // Добавляем для numeric_limits

using namespace std;

class RegionDirectory {
private:
    unordered_map<string, string> regions; // region -> center

public:
    void change(const string& region, const string& new_center) {
        if (regions.find(region) != regions.end()) {
            string old_center = regions[region];
            regions[region] = new_center;
            cout << "Region " << region << " has changed its administrative center from " 
                 << old_center << " to " << new_center << endl;
        } else {
            regions[region] = new_center;
            cout << "New region " << region << " with administrative center " << new_center << endl;
        }
    }

    void rename(const string& old_region, const string& new_region) {
        if (old_region == new_region || regions.count(old_region) == 0 || regions.count(new_region) > 0) {
            cerr << "Incorrect" << endl;
            return;
        }

        string center = regions[old_region];
        regions.erase(old_region);
        regions[new_region] = center;
        cout << old_region << " has been renamed to " << new_region << endl;
    }

    void about(const string& region) const {
        auto it = regions.find(region);
        if (it != regions.end()) {
            cout << region << " has administrative center " << it->second << endl;
        } else {
            cerr << "Incorrect" << endl;
        }
    }

    void all() const {
        vector<pair<string, string>> sorted_regions(regions.begin(), regions.end());
        sort(sorted_regions.begin(), sorted_regions.end());

        for (const auto& [region, center] : sorted_regions) {
            cout << region << " - " << center << endl;
        }
    }
};

string toUpper(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

int main() {
    RegionDirectory directory;
    int N;
    cin >> N;
    cin.ignore();

    for (int i = 0; i < N; ++i) {
        string command;
        cin >> command;
        command = toUpper(command);

        try {
            if (command == "CHANGE") {
                string region, center;
                cin >> region >> center;
                directory.change(region, center);
            } 
            else if (command == "RENAME") {
                string old_region, new_region;
                cin >> old_region >> new_region;
                directory.rename(old_region, new_region);
            } 
            else if (command == "ABOUT") {
                string region;
                cin >> region;
                directory.about(region);
            } 
            else if (command == "ALL") {
                directory.all();
            } 
            else {
                cerr << "Incorrect" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } catch (const exception& e) {
            cerr << "Incorrect" << endl;
        }
    }

    return 0;
}
