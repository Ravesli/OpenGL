/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"


// Класс PostProcessor содержит все используемые в игре "Breakout" эффекты постобработки. 
// Он производит рендеринг игры в текстурированный прямоугольник,
// после чего мы можем задействовать определенные эффекты типа "Shake", "Confuse" или "Chaos".
// Для работы класса перед рендерингом игры требуется вызвать функцию BeginRender(), 
// а в конце рендеринга - функцию EndRender()
class PostProcessor
{
public:
    // Состояние
    Shader PostProcessingShader;
    Texture2D Texture;
    unsigned int Width, Height;
    
	// Опции
    bool Confuse, Chaos, Shake;
    
	// Конструктор
    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    
	// Подготовка операций фреймбуфера постпроцессора перед рендерингом игры
    void BeginRender();
	
    // Функция должна вызываться после рендеринга игры, все визуализированные данные сохраняются в текстуре
    void EndRender();
	
    // Рендерим текстуру объекта класса PostProcessor (в качестве большого, охватывающего экран, спрайта)
    void Render(float time);
private:
    // Состояния рендеринга
    unsigned int MSFBO, FBO; // MSFBO = Мультисэмплированный FBO. Обычный FBO, используемый для блиттирования мультисэмплированного цветового буфера в текстуру
    unsigned int RBO; // RBO используется для мультисэмплированного цветового буфера
    unsigned int VAO;
	
    // Инициализация прямоугольника для рендеринга текстуры постобработки
    void initRenderData();
};

#endif

