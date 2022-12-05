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
#include "lib/matrix.h"
#include "classes/Sphere.h"
#include "classes/Cubesphere.h"
#include "classes/Icosphere.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
const int limit_counter = 100;
int counter = 0;
const int max_bounce = 11;
int counter_bounce;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Icosphere icosphere(1.0f, 2);

int cube_selected = 0;
int solid = 1;
float lineColor[] = {0.2f, 0.2f, 0.2f};
const float* getVertices(){
  return icosphere.getVertices();
}

const unsigned int* getIndices(){
  return icosphere.getIndices();
}

const unsigned int* getLineIndices(){
  return icosphere.getLineIndices();
}

unsigned int getVertexSize(){
  return icosphere.getVertexSize();
}

unsigned int getIndexSize(){
  return icosphere.getIndexSize();
}

unsigned int getLineIndexSize(){
  return icosphere.getLineIndexSize();
}

unsigned int getIndexCount(){
  return icosphere.getLineIndexCount();
}

float rad_plane = 0.4f;
float plane_vertices[] = {
  -1.0f*rad_plane,  0.0f, -1.0f*rad_plane,  
  rad_plane, -0.0f, -1.0f*rad_plane, 
  rad_plane, -0.0f, rad_plane,
  -1.0f*rad_plane,  0.0f, rad_plane,
};

unsigned int plane_indices[] = {
    0, 1, 2,
    0, 2, 3,
};

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


  std::vector<float> origin = {0,1,-8};
  MatrixTransform translate = MatrixTransform(origin, 't');
  icosphere.setVertices(translate.multiply(icosphere.getVerticesVector()));
  
  std::vector<float> move_down = {0,-0.1f,0.01f};
  MatrixTransform translate_down = MatrixTransform(move_down, 't');
  
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
    model = glm::rotate(model, 1.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 4.0f, -10.0f));
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

    if(max_bounce > counter_bounce){
      
      if(counter>limit_counter){
        counter=0;
        move_down[1] = move_down[1] * -0.9f; 
        translate_down = MatrixTransform(move_down, 't');
        counter_bounce++;
      }
      else{
        counter++;
        icosphere.setVertices(translate_down.multiply(icosphere.getVerticesVector()));
      }
    }
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getVertexSize(), getVertices(), GL_STATIC_DRAW);


    GLint color_location = glGetUniformLocation(ourShader.ID, "my_color");
    if(solid){
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
