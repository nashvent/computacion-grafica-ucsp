#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "lib/shader_m.h"
#include "classes/Sphere.h"
#include "classes/Cubesphere.h"
#include "classes/Icosphere.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Sphere sphere(1.0f, 36, 18, false);
Cubesphere cubesphere(1.0f, 2);
Icosphere icosphere(1.0f, 2);

int cube_selected = 0;
int solid = 0;
const float* getVertices(){
  switch(cube_selected){
        case 1: return cubesphere.getVertices();
        case 2: return icosphere.getVertices();
        default: return sphere.getVertices();
  }
}

const unsigned int* getIndices(){
  switch(cube_selected){
    case 1: return cubesphere.getIndices();
    case 2: return icosphere.getIndices();
    default: return sphere.getIndices();
  }
}

const unsigned int* getLineIndices(){
  switch(cube_selected){
    case 1: return cubesphere.getLineIndices();
    case 2: return icosphere.getLineIndices();
    default: return sphere.getLineIndices();
  }
}

unsigned int getVertexSize(){
  switch(cube_selected){
    case 1: return cubesphere.getVertexSize();
    case 2: return icosphere.getVertexSize();
    default: return sphere.getVertexSize();
  }
}

unsigned int getIndexSize(){
  switch(cube_selected){
    case 1: return cubesphere.getIndexSize();
    case 2: return icosphere.getIndexSize();
    default: return sphere.getIndexSize();
  }
}

unsigned int getLineIndexSize(){
  switch(cube_selected){
    case 1: return cubesphere.getLineIndexSize();
    case 2: return icosphere.getLineIndexSize();
    default: return sphere.getLineIndexSize();
  }
}

unsigned int getIndexCount(){
  switch(cube_selected){
    case 1: return cubesphere.getLineIndexCount();
    case 2: return icosphere.getLineIndexCount();
    default: return sphere.getLineIndexCount();
  }
}


int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("shader_cube.vs", "shader_cube.fs");

  unsigned int VBO, VAO, EBO, TBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &TBO);
  
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, getVertexSize(), getVertices(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLineIndexSize(), getLineIndices(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndices(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  ourShader.setMat4("projection", projection);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // activate shader
    ourShader.use();

    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    ourShader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getVertexSize(), getVertices(), GL_STATIC_DRAW);


    GLint color_location = glGetUniformLocation(ourShader.ID, "my_color");
    if(solid){
      float lineColor[] = {0.2f, 0.2f, 0.2f};
      glUniform3fv(color_location, 1, lineColor);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndices(), GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, getIndexCount() * 2, GL_UNSIGNED_INT, 0);
      
    }
    
    float lineColor2[] = {0.1f, 0.1f, 0.1f};
    glUniform3fv(color_location, 1, lineColor2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLineIndexSize(), getLineIndices(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, getLineIndexSize() * 2, GL_UNSIGNED_INT, 0);
    
    
    /*
    float lineColor[] = {0.1f, 0.3f, 0.3f, 1};
    glColor4fv(lineColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   lineColor);
    */
    /*
    */
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if(action != GLFW_PRESS){
      return;
  }

  if (key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3)
  {
    cube_selected = key - 49;
    std::cout << "cube_selected " << cube_selected<< std::endl;
  }

  if (key == GLFW_KEY_S){
    solid= (solid+1)%2;
  }
}
