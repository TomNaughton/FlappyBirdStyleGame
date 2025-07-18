#include "SceneManager.hpp"

std::unique_ptr<Scene> SceneManager::currentScene = nullptr;

SceneManager::SceneManager() {
    changeScene("MainMenu");
}

void SceneManager::changeScene(const std::string& name) {
    if (name == "MainMenu")
        currentScene = std::make_unique<MainMenu>();
    else if (name == "InGame")
        currentScene = std::make_unique<InGame>();
    else if (name == "Settings")
        currentScene = std::make_unique<Settings>();
}

Scene* SceneManager::getCurrentScene() {
    if(currentScene == nullptr) {
        changeScene("MainMenu"); // Default to MainMenu if no scene is set
    }
    return currentScene.get();
}