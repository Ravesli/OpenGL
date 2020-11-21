/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class SpriteRenderer
{
public:
    // Конструктор (инициализирует шейдеры/объекты)
    SpriteRenderer(const Shader& shader);
    // Деструктор
    ~SpriteRenderer();
    // Рендерим текстурированный прямоугольник по заданному спрайту
    void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    // Состояние рендера
    Shader       shader;
    unsigned int quadVAO;
    // Инициализирует и настраивает атрибуты буфера и атрибуты вершин
    void initRenderData();
};

#endif
