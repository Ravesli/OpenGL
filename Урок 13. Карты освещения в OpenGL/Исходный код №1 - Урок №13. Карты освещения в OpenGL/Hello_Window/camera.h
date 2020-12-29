#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Определяем несколько возможных вариантов движения камеры. Используется в качестве абстракции, чтобы держаться подальше от специфичных для оконной системы методов ввода
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Параметры камеры по умолчанию
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// Абстрактный класс камеры, который обрабатывает входные данные и вычисляет соответствующие углы Эйлера, векторы и матрицы для использования в OpenGL
class Camera
{
public:
    // Атрибуты камеры
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
	
    // Углы Эйлера
    float Yaw;
    float Pitch;
	
    // Настройки камеры
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Конструктор, использующий векторы
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
	
    // Конструктор, использующий скаляры
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Возвращаем матрицу вида, вычисленную с использованием углов Эйлера и LookAt-матрицы 
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Обрабатываем входные данные, полученные от клавиатурной системы ввода. Принимаем входной параметр в виде определенного камерой перечисления (для абстрагирования его от оконных систем)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // Обрабатываем входные данные, полученные от системы ввода с помощью мыши. Ожидаем в качестве параметров значения смещения как в направлении x, так и в направлении y
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Убеждаемся, что когда тангаж выходит за пределы обзора, экран не переворачивается
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Обновляем значения вектора-прямо, вектора-вправо и вектора-вверх, используя обновленные значения углов Эйлера
        updateCameraVectors();
    }

    // Обрабатываем входные данные, полученные от события колеса прокрутки мыши. Интересуют только входные данные на вертикальную ось колесика 
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // Вычисляем вектор-прямо по (обновленным) углам Эйлера камеры
    void updateCameraVectors()
    {
        // Вычисляем новый вектор-прямо
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
		
        // Также пересчитываем вектор-вправо и вектор-вверх
        Right = glm::normalize(glm::cross(Front, WorldUp)); // нормализуем векторы, потому что их длина стремится к 0 тем больше, чем больше вы смотрите вверх или вниз, что приводит к более медленному движению
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif

