#pragma once
#include <string>
#include <fstream>
#include "json.hpp"
#include "SaveData.hpp"
using json = nlohmann::json;

class SaveLoad {
    public:
        SaveLoad() = default;
        SaveLoad(std::string path);
        ~SaveLoad() = default;

        void Save(SaveData data);
        SaveData Load();

    private:
        std::string filePath;

        void saveToFile(const SaveData& data, const std::string& filename);
        SaveData loadFromFile(const std::string& filename);
};