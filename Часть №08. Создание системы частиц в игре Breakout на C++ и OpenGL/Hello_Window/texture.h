/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

// Texture2D хранит в себе текстуру и может производить её конфигурирование.
// Он также содержит служебные функции для простоты управления
class Texture2D
{
public:
    // Содержит ID объекта текстуры, используемый для всех операций с конкретной текстурой
    unsigned int ID;
	
    // Размеры текстурного изображения
    unsigned int Width, Height; // ширина и высота (в пикселях) загруженного изображения
    
	// Формат текстуры
    unsigned int Internal_Format; // внутренний формат текстуры
    unsigned int Image_Format; // формат загружаемого изображения
    
	// Конфигурация текстуры
    unsigned int Wrap_S; // режим наложения по оси S
    unsigned int Wrap_T; // режим наложения по оси T
    unsigned int Filter_Min; // режим фильтрации, если пикселей текстуры < пикселей экрана
    unsigned int Filter_Max; // режим фильтрации, если пикселей текстуры > пикселей экрана
    
	// Конструктор (устанавливает заданные по умолчанию значения)
    Texture2D();
	
    // Генерируем текстуру из изображения
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    
	// Связываем текстуру в виде текущего активного GL_TEXTURE_2D текстурного объекта
    void Bind() const;
};

#endif
