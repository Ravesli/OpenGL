/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"


// Представление отдельно взятой частицы и её состояния
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

// Класс ParticleGenerator действует как контейнер для рендеринга большого количества частиц, 
// многократно порождая и обновляя частицы и убивая их через заданный промежуток времени
class ParticleGenerator
{
public:
    // Конструктор
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
	
    // Обновляем все частицы
    void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    
	// Рендерим все частицы
    void Draw();
private:
    // Состояние
    std::vector<Particle> particles;
    unsigned int amount;
    
	// Рендеринг
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
	
    // Инициализация буферов и вершинных атрибутов
    void init();
	
    // Возвращаем индекс первой незадействованной в данный момент частицы (при Life <= 0.0f) или 0, если таких частиц в данный момент нет
    unsigned int firstUnusedParticle();
	
    // Возрождение частиц
    void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif