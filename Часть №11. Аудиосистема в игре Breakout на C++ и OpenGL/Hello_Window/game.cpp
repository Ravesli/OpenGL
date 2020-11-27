/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <algorithm>

#include <irrklang/irrKlang.h>
using namespace irrklang;

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"

// Данные, относящиеся к состоянию игры
SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;
ISoundEngine* SoundEngine = createIrrKlangDevice();

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
    SoundEngine->drop();
}

void Game::Init()
{
    // загрузка шейдеров
    ResourceManager::LoadShader("../shaders/sprite.vs", "../shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("../shaders/particle.vs", "../shaders/particle.frag", nullptr, "particle");
    ResourceManager::LoadShader("../shaders/post_processing.vs", "../shaders/post_processing.frag", nullptr, "postprocessing");
    // конфигурирование шейдеров
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // загрузка текстур
    ResourceManager::LoadTexture("../textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../textures/block.png", false, "block");
    ResourceManager::LoadTexture("../textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("../textures/particle.png", true, "particle");
    ResourceManager::LoadTexture("../textures/powerup_speed.png", true, "powerup_speed");
    ResourceManager::LoadTexture("../textures/powerup_sticky.png", true, "powerup_sticky");
    ResourceManager::LoadTexture("../textures/powerup_increase.png", true, "powerup_increase");
    ResourceManager::LoadTexture("../textures/powerup_confuse.png", true, "powerup_confuse");
    ResourceManager::LoadTexture("../textures/powerup_chaos.png", true, "powerup_chaos");
    ResourceManager::LoadTexture("../textures/powerup_passthrough.png", true, "powerup_passthrough");
    // установка специфичных для рендеринга элементов управления
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    // загрузка уровней
    GameLevel one; one.Load("../levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("../levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("../levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("../levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // конфигурирование игровых объектов
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
    // аудио
    SoundEngine->play2D("../audio/breakout.mp3", true);
}

void Game::Update(float dt)
{
    // обновление объектов
    Ball->Move(dt, this->Width);
    // проверка столкновения
    this->DoCollisions();
    // обновление частиц
    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
    // обновление бонусов
    this->UpdatePowerUps(dt);
    // уменьшение времени тряски
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
    // проверка условия сброса состояния игры
    if (Ball->Position.y >= this->Height) // достиг ли мяч нижней границы окна?
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // перемещаем ракетку
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // начало рендеринга в постпроцессинг-фреймбуфер
        Effects->BeginRender();
        // отрисовка фона
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // отрисовка уровня
        this->Levels[this->Level].Draw(*Renderer);
        // отрисовка ракетки
        Player->Draw(*Renderer);
        // отрисовка бонусов
        for (PowerUp& powerUp : this->PowerUps)
            if (!powerUp.Destroyed)
                powerUp.Draw(*Renderer);
        // отрисовка частиц	
        Particles->Draw();
        // отрисовка мяча
        Ball->Draw(*Renderer);
        // завершение рендеринга в постпроцессинг-фреймбуфер
        Effects->EndRender();
        // рендеринг постпроцессинг-прямоугольника
        Effects->Render(glfwGetTime());
    }
}


void Game::ResetLevel()
{
    if (this->Level == 0)
        this->Levels[0].Load("../levels/one.lvl", this->Width, this->Height / 2);
    else if (this->Level == 1)
        this->Levels[1].Load("../levels/two.lvl", this->Width, this->Height / 2);
    else if (this->Level == 2)
        this->Levels[2].Load("../levels/three.lvl", this->Width, this->Height / 2);
    else if (this->Level == 3)
        this->Levels[3].Load("../levels/four.lvl", this->Width, this->Height / 2);
}

void Game::ResetPlayer()
{
    // сброс состояния уровня/ракетки
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
    // также отключаем все активированные бонусы
    Effects->Chaos = Effects->Confuse = false;
    Ball->PassThrough = Ball->Sticky = false;
    Player->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
}

// бонусы
bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type);

void Game::UpdatePowerUps(float dt)
{
    for (PowerUp& powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;

            if (powerUp.Duration <= 0.0f)
            {
                // убираем бонус из списка (в дальнейшем он будет удален)
                powerUp.Activated = false;
                // деактивируем эффекты
                if (powerUp.Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {	// сбрасываем только в том случае, если больше никакх других бонусов типа “sticky” не активировано
                        Ball->Sticky = false;
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	// сбрасываем только в том случае, если больше никакх других бонусов типа "pass-through" не активировано
                        Ball->PassThrough = false;
                        Ball->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	// сбрасываем только в том случае, если больше никакх других бонусов типа "confuse" не активировано
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp.Type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {	// сбрасываем только в том случае, если больше никакх других бонусов типа "chaos" не активировано
                        Effects->Chaos = false;
                    }
                }
            }
        }
    }
    // Убираем из вектора все бонусы, которые уничтожены И неактивированы (пропавшие с экрана или бонусы, действие которых закончилось)
    // Заметьте, для удаления каждого бонуса, который уже уничтожен или неактивирован, мы используем лямбда-выражение
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}

bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}
void Game::SpawnPowerUps(GameObject& block)
{
    if (ShouldSpawn(75)) // шанс 1 из 75
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (ShouldSpawn(15)) // негативные бонусы должны появляться чаще
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (ShouldSpawn(15))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void ActivatePowerUp(PowerUp& powerUp)
{
    if (powerUp.Type == "speed")
    {
        Ball->Velocity *= 1.2;
    }
    else if (powerUp.Type == "sticky")
    {
        Ball->Sticky = true;
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass-through")
    {
        Ball->PassThrough = true;
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        Player->Size.x += 50;
    }
    else if (powerUp.Type == "confuse")
    {
        if (!Effects->Chaos)
            Effects->Confuse = true; // активируется при условии, что в данный момент не был активирован эффект “хаоса”
    }
    else if (powerUp.Type == "chaos")
    {
        if (!Effects->Confuse)
            Effects->Chaos = true;
    }
}

bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
    // Проверяем, что у нас нескольких все ещё активных бонусов одного типа,
    // в противном случае мы не будем отключать эффект бонуса (пока не будем)
    for (const PowerUp& powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}

// Определение столкновения
bool CheckCollision(GameObject& one, GameObject& two);
Collision CheckCollision(BallObject& one, GameObject& two);
Direction VectorDirection(glm::vec2 closest);

void Game::DoCollisions()
{
    for (GameObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // если произошло столкновение
            {
                // разрушаем кирпич (если он не твёрдый)
                if (!box.IsSolid)
                {
                    box.Destroyed = true;
                    this->SpawnPowerUps(box);
                    SoundEngine->play2D("../audio/bleep.mp3", false);
                }
                else
                {   // если же кирпич - твёрдый, то активируем эффект тряски
                    ShakeTime = 0.05f;
                    Effects->Shake = true;
                    SoundEngine->play2D("../audio/solid.wav", false);
                }
                // обработка столкновения
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(Ball->PassThrough && !box.IsSolid)) // если активирован эффект "pass-through", то отключаем обработку столкновений на разрушаемых кирпичах 
                {
                    if (dir == LEFT || dir == RIGHT) // горизонтальное столкновение
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // обращаем горизонтальную скорость
                        // перемещение
                        float penetration = Ball->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->Position.x += penetration; // двигаем мяч обратно вправо
                        else
                            Ball->Position.x -= penetration; // двигаем мяч обратно влево
                    }
                    else // вертикальное столкновение
                    {
                        Ball->Velocity.y = -Ball->Velocity.y; // обращаем вертикальную скорость
                        // перемещение
                        float penetration = Ball->Radius - std::abs(diff_vector.y);
                        if (dir == UP)
                            Ball->Position.y -= penetration; // двигаем мяч обратно вверх
                        else
                            Ball->Position.y += penetration; // двигаем мяч обратно вниз
                    }
                }
            }
        }
    }

    // также проводим проверку на столкновение бонуса с ракеткой игрока. Если такое событие произошло, то активируем бонус.
    for (PowerUp& powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            // сначала проверяем, достиг ли бонус нижнего края окна и если это так, то: оставляем его неактивированным и уничтожаем
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = true;

            if (CheckCollision(*Player, powerUp))
            {	// столкновение с ракеткой игрока, активируем бонус
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.Activated = true;
                SoundEngine->play2D("../audio/powerup.wav", false);
            }
        }
    }
    // проверка столкновений для ракетки игрока (если мяч не зафиксирован на ней)
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // смотрим, в каком месте мяч ударился о ракетку, и в зависимости от этого изменяем скорость
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
        // и соответствующим образом передвигаем
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // сумма проекций вектора скорости на соответствующие оси должна быть постоянной (умножаем на величину старой скорости, чтобы итоговая скорость не менялась)
        // фикс проблемы "липкой ракетки"
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);

        // Если активирован "Sticky" бонус, то мяч прилипает к ракетке пока не будут вычислены новые векторы скорости
        Ball->Stuck = Ball->Sticky;
        SoundEngine->play2D("../audio/bleep.wav", false);
    }
}

bool CheckCollision(GameObject& one, GameObject& two) // Столкновение вида AABB - AABB
{
    // перекрытие по оси x?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // перекрытие по оси y?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // если перекрытия происходят относительно обеих осей, то мы имеем столкновение
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject& one, GameObject& two) // Столкновение вида AABB - Окружность
{
    // сначала вычисляем точку центра окружности 
    glm::vec2 center(one.Position + one.Radius);
    // вычисляем информацию по AABB(координаты центра, и половинки длин сторон)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // получаем вектор разности между центром окружности и центром AABB
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // добавляя переменную clamped к AABB_center мы получим ближайшую к окружности точку, лежащую на стороне AABB
    glm::vec2 closest = aabb_center + clamped;
    // получаем вектор между центром окружности и ближайшей к ней точкой AABB, проверяем чтобы длина этого вектора была меньше радиуса окружности 
    difference = closest - center;

    if (glm::length(difference) < one.Radius) // именно "<", а не "<=", т.к. в противном случае в конце стандии обработки столкновения (когда объекты касаются друг друга) произойдет повторное столкновение
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

// вычисляем в каком направлении смотрит вектор (С,В,Ю или З)
Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// Вверх
        glm::vec2(1.0f, 0.0f),	// Вправо
        glm::vec2(0.0f, -1.0f),	// Вниз
        glm::vec2(-1.0f, 0.0f)	// Влево
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

