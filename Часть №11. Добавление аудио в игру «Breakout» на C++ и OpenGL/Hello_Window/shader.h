/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// Шейдерный объект общего назначения. Компилируется из файла,
// генерирует сообщения об ошибках времени компиляции/связывания и 
// содержит несколько служебных функций для удобства управления
class Shader
{
public:
    // Состояние
    unsigned int ID; 
	
    // Конструктор
    Shader() { }
	
    // Устанавливаем текущий шейдер в качестве активного
    Shader &Use();
	
    // Компилируем шейдер из переданного исходного кода
    void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr); // примечание: Исходный код геометрического шейдера является опциональным 
    
	// Служебные функции
    void    SetFloat    (const char *name, float value, bool useShader = false);
    void    SetInteger  (const char *name, int value, bool useShader = false);
    void    SetVector2f (const char *name, float x, float y, bool useShader = false);
    void    SetVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
    void    SetVector3f (const char *name, float x, float y, float z, bool useShader = false);
    void    SetVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
    void    SetVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
    void    SetVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
    void    SetMatrix4  (const char *name, const glm::mat4 &matrix, bool useShader = false);
private:
    // Проверяем, не произошла ли ошибка компиляции или связывания, и если да, то выводим логи ошибок
    void checkCompileErrors(unsigned int object, std::string type); 
};

#endif
