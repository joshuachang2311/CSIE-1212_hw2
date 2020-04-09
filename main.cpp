#include "Map.h"

#include <iostream>
#include <string>

int main() {
    ben_birdland::Map<std::string, int> map;
    while (true) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            std::string key;
            int value;
            std::cin >> key >> value;
            map.Insert(key, value);
        } else if (command == "at") {
            std::string key;
            std::cin >> key;
            std::cout << map[key] << '\n';
        } else if (command == "contains") {
            std::string key;
            std::cin >> key;
            std::cout << (map.Contains(key) ? "true" : "false") << '\n';
        } else if (command == "remove") {
            std::string key;
            std::cin >> key;
            map.Remove(key);
        } else if (command == "print") {
            for (const auto &pair : map) {
                std::cout << "pair(" << pair.first << ", " << pair.second << ")\n";
            }
        } else if (command == "exit") {
            break;
        }
    }

    return 0;
}
