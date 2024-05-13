#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Time.h"

const float width = 800;
const float height = 600;

Camera camera = Camera(width,height);
Time timer;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float plainCubeVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

void resizeEvent(GLFWwindow* window, int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
}

void mouseEvent(GLFWwindow* window, double xpos, double ypos)
{
    camera.rotate(xpos, ypos);
}

void handleInput(GLFWwindow* window)
{
    // CLOSE THE APPLICATION IF WE PRESS ESCAPE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.move(window);
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
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
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

    // MAKE SO THAT OUR MOUSE IS HIDDEN AND CAPTURED (GETS LOCKED TO CENTER OF OUR WINDOW)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // BIND MOUSE MOVEMENT EVENT
    glfwSetCursorPosCallback(window, mouseEvent);

    // SET OUR CLEAR COLOR
    glClearColor(0.f, 0.f, 0.f, 1.0f);

    // CREATE A VERTEX ARRAY
    unsigned VAO;
    glGenVertexArrays(1, &VAO);

    unsigned plainCubeVAO;
    glGenVertexArrays(1, &plainCubeVAO);

    // CREATE A VERTEX OBJECT
    unsigned VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // TELL OPENGL HOW TO INTERPRET OUR DATA. HAVING BOUND OUR VAO THIS WILL GET STORED
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glBindVertexArray(plainCubeVAO);
    unsigned plainCubeVBO;
    glGenBuffers(1, &plainCubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, plainCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plainCubeVertices), plainCubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader texturedShader = Shader("shaders/vertex/textured.vs", "shaders/fragment/textured.fs");
    Shader defaultShader = Shader("shaders/vertex/default.vs", "shaders/fragment/default.fs");

    texturedShader.use();

    // LOAD TEXTURES
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data1 = stbi_load("container2.png", &width, &height, &nrChannels, 0);

    unsigned texture1;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);

    unsigned char* data2 = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);

    unsigned texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data2);
    stbi_image_free(data1);

    // CREATE A PERSPECTIVE MATRIX
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height,0.1f, 100.0f);

    // CREATE A MODEL MATRIX
    glm::mat4 texturedCubeModelMatrix = glm::mat4(1.0f);
    glm::mat4 plainCubeModelMatrix = glm::mat4(1.0f);
    glm::vec3 lightPos = glm::vec3(2.0f, 3.0f, -2.0f);
    plainCubeModelMatrix = glm::translate(plainCubeModelMatrix, lightPos);

    // CREATE A VIEW MATRIX
    glm::mat4 view = glm::mat4(1.0f);

    texturedShader.setMatrix("projection", projection);
    texturedShader.setMatrix("model", texturedCubeModelMatrix);
    texturedShader.setVector3("viewPos", camera.getPos());

    texturedShader.setVector3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    texturedShader.setInt("material.diffuse", 0);
    texturedShader.setInt("material.specular", 1);
    texturedShader.setFloat("material.shininess", 32.0f);

    // directional light
    texturedShader.setVector3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    texturedShader.setVector3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    texturedShader.setVector3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    texturedShader.setVector3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    // point light 0
    texturedShader.setVector3("pointLights[0].position", lightPos);
    texturedShader.setVector3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    texturedShader.setVector3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    texturedShader.setVector3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    texturedShader.setFloat("pointLights[0].constant", 1.0f);
    texturedShader.setFloat("pointLights[0].linear", 0.09f);
    texturedShader.setFloat("pointLights[0].quadratic", 0.032f);
    
    defaultShader.use();
    defaultShader.setMatrix("projection", projection);
    defaultShader.setMatrix("model", plainCubeModelMatrix);

    glEnable(GL_DEPTH_TEST);

    // RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        timer.tick();

        handleInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // UPDATE VIEW MATRIX
        view = camera.getViewMatrix();

        texturedShader.use();
        texturedShader.setMatrix("view", view);
        texturedShader.setVector3("viewPos", camera.getPos());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        defaultShader.use();
        defaultShader.setMatrix("view", view);
        glBindVertexArray(plainCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents(); // CHECKS IF ANY EVENTS ARE TRIGGERED
    }

    glfwTerminate();
    return 0;
}