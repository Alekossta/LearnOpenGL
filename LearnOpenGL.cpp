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

glm::vec3 lightPos = glm::vec3(2.0f, 3.0f, -2.0f);

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

void addLights(Shader& shader)
{
    // directional light
    shader.setVector3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader.setVector3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader.setVector3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    shader.setVector3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    // point light 0
    shader.setVector3("pointLights[0].position", lightPos);
    shader.setVector3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader.setVector3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader.setVector3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09f);
    shader.setFloat("pointLights[0].quadratic", 0.032f);

    // spotlight
    shader.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader.setFloat("spotlight.constant", 1.0f);
    shader.setFloat("spotlight.linear", 0.09f);
    shader.setFloat("spotlight.quadratic", 0.032f);
    shader.setVector3("spotlight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader.setVector3("spotlight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
    shader.setVector3("spotlight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
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

    //Shader texturedShader = Shader("shaders/vertex/textured.vs", "shaders/fragment/textured.fs");
    Shader colorShader = Shader("shaders/vertex/unlit_color.vert", "shaders/fragment/unlit_color.frag");
    Shader shadedShader = Shader("shaders/vertex/lit_color.vert", "shaders/fragment/lit_color.frag");

    // LOAD TEXTURES
    stbi_set_flip_vertically_on_load(true);

    // CREATE A PERSPECTIVE MATRIX
    glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)width / (float)height,0.1f, 100.0f);

    // CREATE A MODEL MATRIX
    glm::mat4 texturedCubeModelMatrix = glm::mat4(1.0f);

    // CREATE A VIEW MATRIX
    glm::mat4 view = glm::mat4(1.0f);

    // Add projection matrices to shaders
    shadedShader.setMatrix("projection", projection);
    colorShader.setMatrix("projection", projection);

    addLights(shadedShader);

    // material
    colorShader.setVector3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    colorShader.setVector3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    colorShader.setVector3("material.specular", glm::vec3(1.0f, 0.5f, 0.31f));
    colorShader.setFloat("material.shininess", 32.0f);    

    glEnable(GL_DEPTH_TEST);

    // Load and Create Model
    Model backpackModel("./backpack/backpack.obj");
    Model cubeModel("./cube.obj");
    Model planeModel("./plane.obj");

    // Make matrix for light position
    glm::mat4 lightCubeModelMatrix = glm::mat4(1.0f);
    lightCubeModelMatrix = glm::translate(lightCubeModelMatrix, lightPos);

    // Make matrix for outline
    glm::mat4 outlineCubeModelMatrix = glm::mat4(1.0f);
    glm::vec3 outlineCubePosition = -lightPos;
    outlineCubePosition.y = 1.1;
    outlineCubeModelMatrix = glm::translate(outlineCubeModelMatrix, outlineCubePosition);

    // Make matrix for 
    glm::mat4 planeModelMatrix = glm::mat4(1.0f);
    glm::vec3 planePos = glm::vec3(0.f);
    planeModelMatrix = glm::translate(planeModelMatrix, planePos);
    planeModelMatrix = glm::scale(planeModelMatrix, glm::vec3(10.f));

    // STENCIL SETUP
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        timer.tick();

        handleInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        view = camera.getViewMatrix();
        colorShader.setMatrix("view", view);

        shadedShader.setMatrix("view", view);
        shadedShader.setVector3("spotlight.position", camera.getPos());
        shadedShader.setVector3("spotlight.direction", camera.getFront());

        // draw light cube
        colorShader.setMatrix("model", lightCubeModelMatrix);
        colorShader.setVector3("color", glm::vec3(1.0f,1.0f,1.0f));
        cubeModel.Draw(colorShader);

        // draw outline cube
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        shadedShader.setMatrix("model", outlineCubeModelMatrix);
        shadedShader.setVector3("material.color", glm::vec3(0.0f, 0.5f, 0.0f));
        cubeModel.Draw(shadedShader);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glm::mat4 scaledCube = glm::scale(outlineCubeModelMatrix, glm::vec3(1.05f));
        colorShader.setMatrix("model", scaledCube);
        colorShader.setVector3("color", glm::vec3(1.0f, 0.0f, 0.0f));
        cubeModel.Draw(colorShader);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        // draw ground
        shadedShader.setMatrix("model", planeModelMatrix);
        shadedShader.setVector3("material.color", glm::vec3(0.2f, 0.5f, 0.6f));
        planeModel.Draw(shadedShader);


        glfwSwapBuffers(window);
        glfwPollEvents(); // CHECKS IF ANY EVENTS ARE TRIGGERED
    }

    glfwTerminate();
    return 0;
}