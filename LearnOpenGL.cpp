#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";


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

    // CREATE A VERTEX ARRAY
    unsigned VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // CREATE A VERTEX OBJECT
    unsigned VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // CREATE A ELEMENT BUFFER OBJECT
    unsigned EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // TELL OPENGL HOW TO INTERPRET OUR DATA. HAVING BOUND OUR VAO THIS WILL GET STORED
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // CREATE A VERTEX SHADER
    unsigned vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int successVertexShader;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertexShader);
    if (!successVertexShader)
    {
        char infolog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        
        std::cout << "Error in vertex shader\n" << infolog << std::endl;
    }

    // CREATE A FRAGMENT SHADER
    unsigned fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    int successFragmentShader;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFragmentShader);
    if (!successFragmentShader)
    {
        char infolog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);

        std::cout << "Error in fragment shader\n" << infolog << std::endl;
    }

    // MERGE THE VERTEX AND FRAGMENT SHADER INTO A SHADER PROGRAM
    unsigned shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int successLink;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successLink);
    if (!successLink)
    {
        char infolog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);

        std::cout << "Error in linking\n" << infolog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    // RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        // CLOSE THE APPLICATION IF WE PRESS ESCAPE
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents(); // CHECKS IF ANY EVENTS ARE TRIGGERED
    }

    glfwTerminate();
    return 0;
}