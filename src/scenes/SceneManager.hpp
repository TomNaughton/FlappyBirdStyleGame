#pragma once

#include <memory>
#include <map>
#include "Scene.hpp"
#include "MainMenu.hpp"
#include "InGame.hpp"
#include "Settings.hpp"

class SceneManager {
public:
    SceneManager();

    static void changeScene(const std::string& name);
    static Scene* getCurrentScene();

private:
    static std::unique_ptr<Scene> currentScene;
};
