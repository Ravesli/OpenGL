/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

// Представляет текущее состояние игры
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Начальный размер ракетки игрока
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Начальная скорость перемещения ракетки игрока
const float PLAYER_VELOCITY(500.0f);


// Класс Game содержит все относящиеся к игре состояния и необходимый функционал.
// Объединяет все данные, связанные с игрой, в один класс для
// простого доступа к каждому из компонентов.

class Game
{
public:
    // состояние игры
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    // конструктор/деструктор
    Game(unsigned int width, unsigned int height);
    ~Game();
    // инициализация состояния игры (загрузка всех шейдеров/текстур/уровней)
    void Init();
    // игровой цикл
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif

