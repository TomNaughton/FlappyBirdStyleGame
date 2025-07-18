#include "datamanager.hpp"

SaveLoad DataManager::saveLoad;
SaveData DataManager::data;

void DataManager::Init() {
    saveLoad = SaveLoad("savefile.json");

    data = saveLoad.Load();
}

void DataManager::Save() {
    saveLoad.Save(data);
}

void DataManager::Load() {
    data = saveLoad.Load();
}