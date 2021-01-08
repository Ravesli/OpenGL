/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"


// Статический singleton-класс ResourceManager, содержащий несколько функций для загрузки текстур и шейдеров. 
// Каждая загруженная текстура и/или шейдер сохраняются для дальнейшего использования
class ResourceManager
{
public:
    // Хранилище ресурсов
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
	
    // Если gShaderFile !=  nullptr, то также загружается и геометрический шейдер
    static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    
	// Получаем сохраненный шейдер
    static Shader GetShader(std::string name);
	
    // Загружаем (и генерируем) текстуру из файла
    static Texture2D LoadTexture(const char *file, bool alpha, std::string name);
	
    // Получаем сохраненную текстуру
    static Texture2D GetTexture(std::string name);
    
	// Корректное освобождение памяти всех загруженных ресурсов
    static void Clear();
private:
    // private-конструктор, т.е. мы не хотим, чтобы создавались реальные объекты менеджера ресурсов. Его члены и функции должны быть общедоступными (статичными)
    ResourceManager() { }
	
    // Загружаем и генерируем шейдер из файла
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    
	// Загружаем текстуру из файла
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif
