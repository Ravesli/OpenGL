/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"


// Размер бонусного блока
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
// Начальная скорость бонусного блока
const glm::vec2 VELOCITY(0.0f, 150.0f);


// Класс PowerUp наследует переменные состояния и функции рендеринга от класса GameObject, 
// но при этом содержит дополнительную информацию:
// состояние бонусов, их длительность, активен ли бонус в данный момент или нет.
// Тип бонуса сохранен в строковой переменной
class PowerUp : public GameObject
{
public:
    // Состояние бонуса
    std::string Type;
    float Duration;
    bool Activated;
	
    // Конструктор
    PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
        : GameObject(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { }
};

#endif