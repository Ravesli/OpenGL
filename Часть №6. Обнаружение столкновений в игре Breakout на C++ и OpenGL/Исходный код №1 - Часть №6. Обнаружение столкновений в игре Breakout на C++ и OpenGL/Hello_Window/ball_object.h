/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


// Класс BallObject получен из класса GameObject.
// Помимо необходимой информации о состоянии мяча в нём, в отличие 
// от класса GameObject, присутствуют некоторые дополнительные функции.
class BallObject : public GameObject
{
public:
    // состояние мяча	
    float   Radius;
    bool    Stuck;
    // конструкторы
    BallObject();
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
    // перемещает мячик, удерживая его в пределах границ окна (за исключением нижнего края); возвращает новую позицию
    glm::vec2 Move(float dt, unsigned int window_width);
    // возвращает мяч в исходное состояние с заданным положением и скоростью
    void      Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif
