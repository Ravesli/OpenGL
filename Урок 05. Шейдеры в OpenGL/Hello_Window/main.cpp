

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: инициализация и конфигурирование
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Расскоментировать строчку в случае использования MacOS X
#endif

    // glfw создание окна
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: загрузка всех указателей на OpenGL-функции
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // компилирование нашей шейдерной программы
    // ------------------------------------
    Shader ourShader("../shader.vs", "../shader.fs");  // путь к файлам шейдеров

    // задание вершин (и буфера(ов)) и настройка вершинных атрибутов
    // ------------------------------------------------------------------
    float vertices[] = {
        // координаты         // цвета
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // нижняя правая вершина
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // нижняя левая вершина
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // верхняя вершина
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // координатные артибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // цветовые атрибуты
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // После этого вы можете отменить привязку VАО, чтобы другие вызовы VАО случайно не изменили этот VAO, но это редко происходит.
    // Изменение других значений VAO требует вызова glBindVertexArray в любом случае, поэтому мы обычно не снимаем привязку VAO (или VBO), когда это непосредственно не требуется.
    // glBindVertexArray(0);


    // цикл рендеринга
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Обработка ввода
        // -----
        processInput(window);

        // Рендеринг
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Рендеринг треугольника
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: обмен содержимым переднего и заднего буферов. Опрос событий Ввода\Ввывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
   // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // опционально: освобождаем все ресурсы, как только они изжили своё назначение:
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или опер. системой), вызывается данная функция
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // убеждаемся, что вьюпорт соответствует новым размерам окна; обратите внимание,
    // что ширина и высота будут значительно больше, чем указано на retina -дисплеях.
    glViewport(0, 0, width, height);
}

