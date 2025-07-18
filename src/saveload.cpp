#include "saveload.hpp"

SaveLoad::SaveLoad(std::string path) {
    filePath = path;
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SaveData, highScore)

void SaveLoad::saveToFile(const SaveData& data, const std::string& filename) {
    std::ofstream out(filename);
    if (out) {
        json j = data;
        out << j.dump(4); // pretty print with indent
    }
}

SaveData SaveLoad::loadFromFile(const std::string& filename) {
    SaveData data;

    std::ifstream in(filename);
    if (in) {
        json j;
        in >> j;
        data = j.get<SaveData>();
    }

    return data;
}

void SaveLoad::Save(SaveData data) {
    saveToFile(data, filePath);
}

SaveData SaveLoad::Load() {
    return loadFromFile(filePath);
}