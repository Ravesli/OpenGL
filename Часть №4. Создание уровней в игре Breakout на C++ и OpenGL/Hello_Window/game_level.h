/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// Класс GameLevel содержит все кирпичи уровней игры, 
/// а также функционал для загрузки с жесткого диска информации и их рендеринга
class GameLevel
{
public:
    // состояние уровня
    std::vector<GameObject> Bricks;
    // конструктор
    GameLevel() { }
    // загрузка уровня из файла
    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    // рендеринг уровня
    void Draw(SpriteRenderer& renderer);
    // проверка, пройден ли уровень (все разрушаемые кирпичи должны быть уничтожены)
    bool IsCompleted();
private:
    // инициализация уровня данными из tileData
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif
