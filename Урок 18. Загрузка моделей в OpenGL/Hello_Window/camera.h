#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// РћРїСЂРµРґРµР»СЏРµС‚ РЅРµСЃРєРѕР»СЊРєРѕ РІРѕР·РјРѕР¶РЅС‹С… РІР°СЂРёР°РЅС‚РѕРІ РґРІРёР¶РµРЅРёСЏ РєР°РјРµСЂС‹. Р�СЃРїРѕР»СЊР·СѓРµС‚СЃСЏ РІ РєР°С‡РµСЃС‚РІРµ Р°Р±СЃС‚СЂР°РєС†РёРё, С‡С‚РѕР±С‹ РґРµСЂР¶Р°С‚СЊСЃСЏ РїРѕРґР°Р»СЊС€Рµ РѕС‚ СЃРїРµС†РёС„РёС‡РЅС‹С… РґР»СЏ РѕРєРѕРЅРЅРѕР№ СЃРёСЃС‚РµРјС‹ РјРµС‚РѕРґРѕРІ РІРІРѕРґР°
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// РџР°СЂР°РјРµС‚СЂС‹ РєР°РјРµСЂС‹ РїРѕ СѓРјРѕР»С‡Р°РЅРёСЋ
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// РђР±СЃС‚СЂР°РєС‚РЅС‹Р№ РєР»Р°СЃСЃ РєР°РјРµСЂС‹, РєРѕС‚РѕСЂС‹Р№ РѕР±СЂР°Р±Р°С‚С‹РІР°РµС‚ РІС…РѕРґРЅС‹Рµ РґР°РЅРЅС‹Рµ Рё РІС‹С‡РёСЃР»СЏРµС‚ СЃРѕРѕС‚РІРµС‚СЃС‚РІСѓСЋС‰РёРµ Р­Р№Р»РµСЂРѕРІС‹ СѓРіР»С‹, РІРµРєС‚РѕСЂС‹ Рё РјР°С‚СЂРёС†С‹ РґР»СЏ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ РІ OpenGL
class Camera
{
public:
    // РђС‚СЂРёР±СѓС‚С‹ РєР°РјРµСЂС‹
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // СѓРіР»С‹ Р­Р№Р»РµСЂР°
    float Yaw;
    float Pitch;
    // РќР°СЃС‚СЂРѕР№РєРё РєР°РјРµСЂС‹
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ, РёСЃРїРѕР»СЊР·СѓСЋС‰РёР№ РІРµРєС‚РѕСЂС‹
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ, РёСЃРїРѕР»СЊР·СѓСЋС‰РёРµ СЃРєР°Р»СЏСЂС‹
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Р’РѕР·РІСЂР°С‰Р°РµС‚ РјР°С‚СЂРёС†Сѓ РІРёРґР°, РІС‹С‡РёСЃР»РµРЅРЅСѓСЋ СЃ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёРµРј СѓРіР»РѕРІ Р­Р№Р»РµСЂР° Рё LookAt-РјР°С‚СЂРёС†С‹ 
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    //РћР±СЂР°Р±Р°С‚С‹РІР°РµРј РІС…РѕРґРЅС‹Рµ РґР°РЅРЅС‹Рµ, РїРѕР»СѓС‡РµРЅРЅС‹Рµ РѕС‚ Р»СЋР±РѕР№ РєР»Р°РІРёР°С‚СѓСЂРѕРїРѕРґРѕР±РЅРѕР№ СЃРёСЃС‚РµРјС‹ РІРІРѕРґР°. РџСЂРёРЅРёРјР°РµРј РІС…РѕРґРЅРѕР№ РїР°СЂР°РјРµС‚СЂ РІ РІРёРґРµ РѕРїСЂРµРґРµР»РµРЅРЅРѕРіРѕ РєР°РјРµСЂРѕР№ РїРµСЂРµС‡РёСЃР»РµРЅРёСЏ (РґР»СЏ Р°Р±СЃС‚СЂР°РіРёСЂРѕРІР°РЅРёСЏ РµРіРѕ РѕС‚ РѕРєРѕРЅРЅС‹С… СЃРёСЃС‚РµРј)
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

    //РћР±СЂР°Р±Р°С‚С‹РІР°РµРј РІС…РѕРґРЅС‹Рµ РґР°РЅРЅС‹Рµ, РїРѕР»СѓС‡РµРЅРЅС‹Рµ РѕС‚ СЃРёСЃС‚РµРјС‹ РІРІРѕРґР° СЃ РїРѕРјРѕС‰СЊСЋ РјС‹С€Рё. РћР¶РёРґР°РµРј РІ РєР°С‡РµСЃС‚РІРµ РїР°СЂР°РјРµС‚СЂРѕРІ Р·РЅР°С‡РµРЅРёСЏ СЃРјРµС‰РµРЅРёСЏ РєР°Рє РІ РЅР°РїСЂР°РІР»РµРЅРёРё X, С‚Р°Рє Рё РІ РЅР°РїСЂР°РІР»РµРЅРёРё Y.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // РЈР±РµР¶РґР°РµРјСЃСЏ, С‡С‚Рѕ РєРѕРіРґР° С‚Р°РЅРіР°Р¶ РІС‹С…РѕРґРёС‚ Р·Р° РїСЂРµРґРµР»С‹ РѕР±Р·РѕСЂР°, СЌРєСЂР°РЅ РЅРµ РїРµСЂРµРІРѕСЂР°С‡РёРІР°РµС‚СЃСЏ
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // РћР±РЅРѕРІР»СЏРµРј Р·РЅР°С‡РµРЅРёСЏ РІРµРєС‚РѕСЂР°-РїСЂСЏРјРѕ, РІРµРєС‚РѕСЂР°-РІРїСЂР°РІРѕ Рё РІРµРєС‚РѕСЂР°-РІРІРµСЂС…, РёСЃРїРѕР»СЊР·СѓСЏ РѕР±РЅРѕРІР»РµРЅРЅС‹Рµ Р·РЅР°С‡РµРЅРёСЏ СѓРіР»РѕРІ Р­Р№Р»РµСЂР°
        updateCameraVectors();
    }

    // РћР±СЂР°Р±Р°С‚С‹РІР°РµС‚ РІС…РѕРґРЅС‹Рµ РґР°РЅРЅС‹Рµ, РїРѕР»СѓС‡РµРЅРЅС‹Рµ РѕС‚ СЃРѕР±С‹С‚РёСЏ РєРѕР»РµСЃР° РїСЂРѕРєСЂСѓС‚РєРё РјС‹С€Рё. Р�РЅС‚РµСЂРµСЃСѓСЋС‚ С‚РѕР»СЊРєРѕ РІС…РѕРґРЅС‹Рµ РґР°РЅРЅС‹Рµ РЅР° РІРµСЂС‚РёРєР°Р»СЊРЅСѓСЋ РѕСЃСЊ РєРѕР»РµСЃРёРєР° 
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
    // Р’С‹С‡РёСЃР»СЏРµС‚ РІРµРєС‚РѕСЂ-РїСЂСЏРјРѕ РїРѕ (РѕР±РЅРѕРІР»РµРЅРЅС‹Рј) СѓРіР»Р°Рј Р­Р№Р»РµСЂР° РєР°РјРµСЂС‹
    void updateCameraVectors()
    {
        // Р’С‹С‡РёСЃР»СЏРµРј РЅРѕРІС‹Р№ РІРµРєС‚РѕСЂ-РїСЂСЏРјРѕ
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // РўР°РєР¶Рµ РїРµСЂРµСЃС‡РёС‚С‹РІР°РµРј РІРµРєС‚РѕСЂ-РІРїСЂР°РІРѕ Рё РІРµРєС‚РѕСЂ-РІРІРµСЂС…
        Right = glm::normalize(glm::cross(Front, WorldUp));  // РќРѕСЂРјР°Р»РёР·СѓРµРј РІРµРєС‚РѕСЂС‹, РїРѕС‚РѕРјСѓ С‡С‚Рѕ РёС… РґР»РёРЅР° СЃС‚Р°РЅРѕРІРёС‚СЃСЏ СЃС‚СЂРµРјРёС‚СЃСЏ Рє 0 С‚РµРј Р±РѕР»СЊС€Рµ, С‡РµРј Р±РѕР»СЊС€Рµ РІС‹ СЃРјРѕС‚СЂРёС‚Рµ РІРІРµСЂС… РёР»Рё РІРЅРёР·, С‡С‚Рѕ РїСЂРёРІРѕРґРёС‚ Рє Р±РѕР»РµРµ РјРµРґР»РµРЅРЅРѕРјСѓ РґРІРёР¶РµРЅРёСЋ.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif

