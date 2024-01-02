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

    // two triangles

    float ltVertices[] = {
        // first triangle (left)
        0.0f, 0.0f, 0.0f, // bottom right
        -0.5f, 0.0f, 0.0f, // bottom left
        -0.25, 0.5, 0.0f, // top
    };

    float rtVertices[] = {
        // second triangle (right)
        0.0f, 0.0f, 0.0f, // bottom left
        0.5f, 0.0f, 0.0f, // bottom right
        0.25f, 0.5f, 0.0f, // top
    };

    // managing a lot of vertex attributes in a lot of vertex buffer objects can be difficult.
    // for that reason we use vertex array objects in order to store our attribute handling data.
    // so basically VAOS are used to store state.
    unsigned lVAO;
    glGenVertexArrays(1, &lVAO);
    glBindVertexArray(lVAO);

    unsigned lVBO;
    glGenBuffers(1, &lVBO); // generate the buffer
    glBindBuffer(GL_ARRAY_BUFFER, lVBO); // bind the generated buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(ltVertices), ltVertices, GL_STATIC_DRAW); // pass our data to the buffer

    // above we have defined our vertex data. the vertex data consists of many vertex attributes. in our case
    // we have only one vertex attribute. the position. so now we need to let opengl know about this.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0); // enable the attribute at location = 0 (position of vertex)

    glBindVertexArray(0);

    unsigned rVAO;
    glGenVertexArrays(1, &rVAO);
    glBindVertexArray(rVAO);

    unsigned rVBO;
    glGenBuffers(1, &rVBO); // generate the buffer
    glBindBuffer(GL_ARRAY_BUFFER, rVBO); // bind the generated buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(rtVertices), rtVertices, GL_STATIC_DRAW); // pass our data to the buffer

    // above we have defined our vertex data. the vertex data consists of many vertex attributes. in our case
    // we have only one vertex attribute. the position. so now we need to let opengl know about this.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0); // enable the attribute at location = 0 (position of vertex)

    glBindVertexArray(0);

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
    const char* fsSrcYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";
    unsigned fsYellow;
    fsYellow = glCreateShader(GL_FRAGMENT_SHADER); // create shader of type fragment
    glShaderSource(fsYellow, 1, &fsSrcYellow, NULL); // pass source to fragment shader we created
    glCompileShader(fsYellow); // compile shader
    // check for errors in compilation of fragment shader
    glGetShaderiv(fsYellow, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fsYellow, 512, NULL, infoLog);
        std::cout << "Error in compilation of yellow fragment shader" << std::endl << infoLog << std::endl;
    }

    // shader program
    unsigned spYellow;
    spYellow = glCreateProgram(); // create the shader program
    glAttachShader(spYellow, vertexShader); // attach the vertex shader to our shader program
    glAttachShader(spYellow, fsYellow); // attach the fragment shader to our shader program
    glLinkProgram(spYellow); // link everything

    // check if the linking and everything with the shader program was successfull
    glGetProgramiv(spYellow, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(spYellow, 512, NULL, infoLog);
        std::cout << "Error in linking of shader program" << std::endl << infoLog << std::endl;
    }

    const char* fsSrcOrange = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    unsigned fsOrange;
    fsOrange = glCreateShader(GL_FRAGMENT_SHADER); // create shader of type fragment
    glShaderSource(fsOrange, 1, &fsSrcOrange, NULL); // pass source to fragment shader we created
    glCompileShader(fsOrange); // compile shader
    // check for errors in compilation of fragment shader
    glGetShaderiv(fsOrange, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fsOrange, 512, NULL, infoLog);
        std::cout << "Error in compilation of yellow fragment shader" << std::endl << infoLog << std::endl;
    }

    unsigned spOrange;
    spOrange = glCreateProgram();
    glAttachShader(spOrange, vertexShader);
    glAttachShader(spOrange, fsOrange);
    glLinkProgram(spOrange);

    // the shaders we made are inside our shader program right now so we dont need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fsYellow);
    glDeleteShader(fsOrange);

    while (!glfwWindowShouldClose(window))
    {
        handleInput(window);

        // set our clear color to greenish
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // clear our color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // render

        // left triangle yellow
        glUseProgram(spYellow);
        glBindVertexArray(lVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // right triangle orange
        glUseProgram(spOrange);
        glBindVertexArray(rVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // after we are done making our new image/buffer swap our buffers
        glfwSwapBuffers(window);

        // check for any OS events (keyboard input, mouse input etc)
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}