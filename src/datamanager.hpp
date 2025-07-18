#pragma once

#include "savedata.hpp"
#include "saveload.hpp"

class DataManager {
    public:
        SaveData data;
        SaveLoad *saveLoad;

        DataManager();
        ~DataManager() = default;

        void Save();
        void Load();
};