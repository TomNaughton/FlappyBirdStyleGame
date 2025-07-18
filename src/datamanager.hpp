#pragma once

#include "savedata.hpp"
#include "saveload.hpp"

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