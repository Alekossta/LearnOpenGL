#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    // simple 2d triangle located in centre of screen/viewport
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left vertex
        0.f, 0.5f, 0.0f, // top middle vertex   
        0.5f, -0.5f, 0.0f // bottom right vertex
    };

    // managing a lot of vertex attributes in a lot of vertex buffer objects can be difficult.
    // for that reason we use vertex array objects in order to store our attribute handling data.
    // so basically VAOS are used to store state.
    unsigned VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned VBO;
    glGenBuffers(1, &VBO); // generate the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the generated buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // pass our data to the buffer

    // above we have defined our vertex data. the vertex data consists of many vertex attributes. in our case
    // we have only one vertex attribute. the position. so now we need to let opengl know about this.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0); // enable the attribute at location = 0 (position of vertex)


    // vertex shader
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    unsigned vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // create shader of type vertex
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // pass our source code to the created shader
    glCompileShader(vertexShader); // compile the source code we gave our shader

    // check for errors in compilation of vertex shader
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error in compilation of vertex shader" << std::endl << infoLog << std::endl;
    }

    // fragment shader
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
    unsigned fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create shader of type fragment
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // pass source to fragment shader we created
    glCompileShader(fragmentShader); // compile shader
    // check for errors in compilation of fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error in compilation of fragment shader" << std::endl << infoLog << std::endl;
    }

    // shader program
    unsigned shaderProgram;
    shaderProgram = glCreateProgram(); // create the shader program
    glAttachShader(shaderProgram, vertexShader); // attach the vertex shader to our shader program
    glAttachShader(shaderProgram, fragmentShader); // attach the fragment shader to our shader program
    glLinkProgram(shaderProgram); // link everything

    // check if the linking and everything with the shader program was successfull
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error in linking of shader program" << std::endl << infoLog << std::endl;
    }

    // for all next renders use the shader program we created
    glUseProgram(shaderProgram);

    // the shaders we made are inside our shader program right now so we dont need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window))
    {
        handleInput(window);

        // set our clear color to greenish
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // clear our color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // render
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // after we are done making our new image/buffer swap our buffers
        glfwSwapBuffers(window);

        // check for any OS events (keyboard input, mouse input etc)
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}