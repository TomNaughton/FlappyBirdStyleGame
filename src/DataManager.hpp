#pragma once

#include "SaveData.hpp"
#include "SaveLoad.hpp"

class DataManager {
    public:        
        static void Init();

        static void Save();
        static void Load();

        static SaveData& GetData() {
            return data;
        }

    private:
        static SaveLoad saveLoad;
        static SaveData data;
};