#include "datamanager.hpp"

DataManager::DataManager() {
    saveLoad = new SaveLoad("savefile.json");

    data = saveLoad->Load();
}

void DataManager::Save() {
    saveLoad->Save(data);
}

void DataManager::Load() {
    data = saveLoad->Load();
}