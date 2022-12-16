#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "lib/matrix.h"
#include "lib/shader_m.h"
#include "lib/utils.h"
#include "lib/solver.h"
#include "cube/cube.h"
#include "cube/rubik.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// custom vars
float camera_step_x = 0.1;
float camera_step_y = 0.1;
float camera_step_z = 0.1;
RubikCube * rubikCube = nullptr;
int camada_selected = 0;


// HEARTBEAT
const float THRESHOLD_HEARTBEAT = 0.5;
float direction_heartbeat = 0;
float step_heartbeat = 0.01;
float hearbeat_counter = 0.0;

void heartbeat_monitor(){
    if(hearbeat_counter<= 0.0){
        direction_heartbeat = 1.0;
        step_heartbeat = 0.01;
    }
    if(hearbeat_counter >= THRESHOLD_HEARTBEAT){
        direction_heartbeat = -1.0;
        step_heartbeat = 0.001;
    }

    hearbeat_counter+= (direction_heartbeat * step_heartbeat);
    rubikCube->move_from_center(step_heartbeat, direction_heartbeat);
}


void print_instructions(){
    std::cout<<"Instructions para manejar"<<std::endl;
    std::cout<<"Para mover camada presionar las letras de F, B, U, D, R, L"<<std::endl;
    std::cout<<"Para resolver el cubo presionar la barra espaciadora"<<std::endl;
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    Shader ourShader("rubik_cube.vs", "rubik_cube.fs");

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    rubikCube = new RubikCube(VBO);
    rubikCube->draw();
    rubikCube->set_init_animation();

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, cube->vertices.size() * sizeof(cube->vertices[0]),  cube->vertices.data(), GL_STATIC_DRAW);

    // // position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // texture coord attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    std::string container_path = "./resources/textures/rubikscubepalette.png";
    unsigned char *data = stbi_load(container_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);

    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    // -----------------------------------------------------------------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection); 

    print_instructions();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // activate shader
        ourShader.use();

        // camera/view transformation
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        float radius = 10.0f;
        float camX = static_cast<float>(sin(camera_step_x) * radius);
        float camY = static_cast<float>(glm::radians(camera_step_y));
        float camZ = static_cast<float>(cos(camera_step_z) * radius);
        view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 4.0f, 0.0f));
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        float time_value = (float)glfwGetTime() * 0.1f;
        model = glm::rotate(model, time_value, glm::vec3(0.2f, 1.0f, 0.0f));
        // float angle = 0.0f;
        // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        ourShader.setMat4("model", model);
        // heartbeat_monitor();
        rubikCube->update_draw();
        glDrawArrays(GL_TRIANGLES, 0, 1620);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    if (key == GLFW_KEY_LEFT){
        camera_step_x = camera_step_x + 0.1;
        camera_step_z = camera_step_z + 0.1;
        // x_direction = 1.0;
    }

    if (key == GLFW_KEY_RIGHT){
        camera_step_x = camera_step_x - 0.1;
        camera_step_z = camera_step_z - 0.1;
    }

    if (key == GLFW_KEY_UP){
        camera_step_y = camera_step_y + 10.0f;
        if (camera_step_y > 270.0f)
          camera_step_y = 270.0f;
    }

    if (key == GLFW_KEY_DOWN){
        camera_step_y = camera_step_y - 10.0f;
        if (camera_step_y < -270.0f)
          camera_step_y = -270.0f;
    }

    if(action != GLFW_PRESS){
      return;
    }

    if(key == GLFW_KEY_U || key == GLFW_KEY_D || key == GLFW_KEY_R || key == GLFW_KEY_L || key == GLFW_KEY_F || key == GLFW_KEY_B){
        std::string key_pressed = glfwGetKeyName(key, scancode);   
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            key_pressed += "i"; 
        }
        // std::cout<<"key_pressed "<<key_pressed<<std::endl;
        std::cout<<"key_pressed "<<key_pressed<<std::endl;
        rubikCube->set_movement(key_pressed);
    }

    if(key == GLFW_KEY_SPACE){
      std::string order_notation = "urfdlb"; //"frudlb";
      std::vector<char> colors_notation = rubikCube->get_sides_status_notation(order_notation);
      std::string color_notation_str = "";
      for(int i=0;i<colors_notation.size();i++){
        color_notation_str += colors_notation[i];
	    }
      std::cout<<color_notation_str<<std::endl;
      std::vector<std::string> solution = get_solution_kociemba(color_notation_str);
      print_vector(solution);
      rubikCube->set_solution(solution);
    }

    if ( key == GLFW_KEY_ENTER ){
      rubikCube->set_out_animation();
    }
}
