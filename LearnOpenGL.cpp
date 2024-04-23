#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// TRIANGLE POINTS IN NORMALIZED DEVICE COORDINATES
float triangle[] = {
    -0.5,0.0f,0.0f, // LEFT POINT
    0.5,0.0f,0.0f, // RIGHT POINT
    0.0f,0.5f, 0.0f, // TOP MIDDLE POINT
};

void resizeEvent(GLFWwindow* window, int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
}

int main()
{
    // INIT GLFW
    glfwInit();

    // SET THE OPENGL VERSION AND PROFILE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // CREATE THE WINDOW
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GET THE OPENGL IMPLEMENTATIONS USING GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TELL OPENGL THE SIZE OF OUR WINDOW
    glViewport(0,0,800,600);

    // ASSIGN A FUNCTION TO THE "RESIZE" EVENT
    glfwSetFramebufferSizeCallback(window, resizeEvent);

    // SET OUR CLEAR COLOR
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        // CLOSE THE APPLICATION IF WE PRESS ESCAPE
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents(); // CHECKS IF ANY EVENTS ARE TRIGGERED
    }

    glfwTerminate();
    return 0;
}