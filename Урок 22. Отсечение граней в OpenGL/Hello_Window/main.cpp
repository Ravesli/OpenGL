#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// настройки
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

// камера
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// тайминги
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: инициализация и конфигурирование
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw создание окна
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // говорим GLFW захватить нашу мышку
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: загрузка всех указателей на OpenGL-функции
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // конфигурирование глобального состояния OpenGL
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // тест глубины всегда проходит успешно (аналогично glDisable(GL_DEPTH_TEST))
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    // компилирование нашей шейдерной программы
    // -------------------------
    Shader shader("../4.1.face_culling.vs", "../4.1.face_culling.fs");

    // установка вершинных данных (буффера(-ов)) и настройка вершинных атрибутов
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // задняя грань
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // нижняя-левая
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // нижняя-правая    
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // верхняя-правая              
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // верхняя-правая
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // верхняя-левая
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // нижняя-левая                
        // передняя грань
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // нижняя-левая
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // верхняя-правая
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // нижняя-правая        
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // верхняя-правая
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // нижняя-левая
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // верхняя-левая        
        // left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // верхняя-правая
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // нижняя-левая
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // верхняя-левая       
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // нижняя-левая
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // верхняя-правая
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // нижняя-правая
        // грань справа
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // верхняя-левая
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // верхняя-правая      
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // нижняя-правая          
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // нижняя-правая
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // нижняя-левая
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // верхняя-левая
        // нижняя грань          
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // верхняя-правая
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // нижняя-левая
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // верхняя-левая        
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // нижняя-левая
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // верхняя-правая
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // нижняя-правая
        // грань слева
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // верхняя-левая
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // верхняя-правая
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // нижняя-правая                 
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // нижняя-правая
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // нижняя-левая  
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // верхняя-левая              
    };


    /*
    Также не забудьте добавить вызов glFrontFace(), чтобы указать, что треугольники определяются порядком обхода вершин по часовой стрелке.
    Теперь это "фронтальные" треугольники, так что куб визуализируется как обычный:
       glFrontFace(GL_CW);
    */


    // VAO куба
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // Загрузка текстур
    // -------------
    unsigned int cubeTexture = loadTexture("../resources/textures/cube.jpg");
    

    // настройка шейдера
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    // цикл рендеринга
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // логическая часть работы со временем для каждого кадра
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // обработка ввода
        // -----
        processInput(window);

        // рендеринг
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // кубы
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // glfw: обмен содержимым переднего и заднего буферов. Опрос событий Ввода\Ввывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // опционально: освобеждение памяти, выделенной под ресурсы
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    

    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или опер. системой), вызывается данная функция
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // перевернуто, так как Y-координаты идут снизу вверх

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// вспомогательная функция загрузки 2D-текстур из файла
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

