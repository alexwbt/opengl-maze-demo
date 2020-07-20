#include "window.h"
#include "shader.h"
#include "maze.h"

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include <gtc/matrix_transform.hpp>

int main()
{
    if (Window::initWindow("OpenGL Maze Demo", 800, 600) != 0)
    {
        std::cout << "Failed to initialize GLFW Window" << std::endl;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Maze maze(100);

    const char* paths[] = { "2DSimpleShader.vs", "2DSimpleShader.fs" };
    int types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    Shader shader(paths, types, 2);
    shader.use();

    while (!glfwWindowShouldClose(Window::window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        maze.update();

        glm::mat4 projection(1.0f);
        // projection = glm::perspective(glm::radians(45.0f), (float)Window::width / (float)Window::height, 0.1f, 100.0f);
        projection = glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height, 0.1f, 100.0f);

        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0, 0, -3.0f));

        glm::mat4 model(1.0f);
        view = glm::translate(view, glm::vec3(maze.offsetX, maze.offsetY, 0));
        model = glm::scale(model, glm::vec3(maze.scale, maze.scale, 1));

        shader.setMat4("pvm", projection * view * model);

        maze.render();

        glfwSwapBuffers(Window::window);
        glfwPollEvents();
    }

    glfwTerminate();
}