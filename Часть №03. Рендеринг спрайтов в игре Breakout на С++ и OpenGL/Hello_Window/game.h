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

// Описываем текущее состояние игры
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Класс Game содержит все связанные с игрой параметры состояния и различный функционал. Он объединяет все игровые данные в один класс,
// обеспечивая тем самым простой доступ к каждому из компонентов класса и возможность управления ими
class Game
{
public:
    // Игровое состояние
    GameState State;	
    bool Keys[1024];
    unsigned int Width, Height;
	
    // Конструктор
    Game(unsigned int width, unsigned int height);
	
	// Деструктор
    ~Game();
	
    // Инициализация начального состояния игры (загрузка всех шейдеров, текстур, уровней)
    void Init();
	
    // Игровой цикл
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif
