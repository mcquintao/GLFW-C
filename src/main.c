#include <stdio.h>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>


int main(void) {
    printf("Hello World\n");

    GLFWwindow* window;

    if (!glfwInit()){
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Window!", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Não foi possivel carregar o OPENGL");
        glfwTerminate();
        return -1;
    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}