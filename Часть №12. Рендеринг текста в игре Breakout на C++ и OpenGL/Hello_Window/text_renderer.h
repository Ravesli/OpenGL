/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"


/// Содержит всю информацию о состоянии, относящуюся к символу, загруженному с помощью FreeType
struct Character {
    unsigned int TextureID; // ID текстуры глифа
    glm::ivec2   Size;      // размер глифа
    glm::ivec2   Bearing;   // смещение от линии шрифта до верхнего-левого угла глифа
    unsigned int Advance;   // горизонтальное смещение для перехода к следующему глифу
};


// Класс TextRenderer предназначен для рендеринга текста, отображаемого шрифтом, 
// загруженным с помощью библиотеки FreeType. Загруженный шрифт обрабатывается и сохраняется 
// для последующего рендеринга в виде списка символов.
 
class TextRenderer
{
public:
    // содержит список предварительно скомпилированных символов
    std::map<char, Character> Characters;
    // шейдер, используемый для рендеринга текста
    Shader TextShader;
    // конструктор
    TextRenderer(unsigned int width, unsigned int height);
    // предварительно компилирует список символов из заданного шрифта
    void Load(std::string font, unsigned int fontSize);
    // рендеринг строки текста с использованием предварительно скомпилированного списка символов
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
    // состояние рендеринга
    unsigned int VAO, VBO;
};

#endif 