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
#include "Model.h"

const float width = 800;
const float height = 600;

Camera camera = Camera(width,height);
Time timer;

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

    // material
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

    // spotlight
    texturedShader.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    texturedShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));


    texturedShader.setVector3("spotlight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    texturedShader.setVector3("spotlight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
    texturedShader.setVector3("spotlight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
    
    defaultShader.use();
    defaultShader.setMatrix("projection", projection);
    defaultShader.setMatrix("model", plainCubeModelMatrix);

    glEnable(GL_DEPTH_TEST);

    // Create Model
    Model model("./backpack/backpack.obj");

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
        texturedShader.setVector3("spotlight.position", camera.getPos());
        texturedShader.setVector3("spotlight.direction", camera.getFront());
        texturedShader.setVector3("viewPos", camera.getPos());
        model.Draw(texturedShader);

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