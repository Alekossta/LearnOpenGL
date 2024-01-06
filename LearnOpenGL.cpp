#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"

#include <iostream>

void window_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    // init glfw
    glfwInit();

    // set opengl version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // use the opengl core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    // check if window creation was successfull
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // make our current context the window we made
    glfwMakeContextCurrent(window);

    // init glad and check if it worked
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // every time our window changes size call our function
    glfwSetFramebufferSizeCallback(window, window_resize_callback);

    // two triangles

    float vertices[] = {
        // first triangle (left)
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right red
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left green
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top blue
    };

    unsigned VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned VBO;
    glGenBuffers(1, &VBO); // generate the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the generated buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // pass our data to the buffer

    // above we have defined our vertex data. the vertex data consists of many vertex attributes

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0); // enable the attribute at location = 0 (position of vertex)

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // enable the attribute at location = 0 (position of vertex)

    glBindVertexArray(0);

    Shader defaultShader("shaders/vertex/default.vs", "shaders/fragment/default.fs");

    unsigned texture;
    glGenTextures(1, &texture); // generate a texture object
    glBindTexture(GL_TEXTURE_2D, texture); // bind the texture

    // load the image
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    while (!glfwWindowShouldClose(window))
    {
        handleInput(window);

        // set our clear color to greenish
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // clear our color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // render
        defaultShader.use();
        defaultShader.setFloat("offsetRight", 0.25f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // after we are done making our new image/buffer swap our buffers
        glfwSwapBuffers(window);

        // check for any OS events (keyboard input, mouse input etc)
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}