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
#include "ball_object.h"

// Представляет текущее состояние игры
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};


// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// Начальный размер ракетки игрока
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Начальная скорость перемещения ракетки игрока
const float PLAYER_VELOCITY(500.0f);
// Начальная скорость мяча
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Радиус мяча
const float BALL_RADIUS = 12.5f;



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
    void DoCollisions();
    // сброс уровня и положения ракетки
    void ResetLevel();
    void ResetPlayer();
};

#endif

