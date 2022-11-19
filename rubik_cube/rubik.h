#ifndef RUBIK_H
#define RUBIK_H
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "cube.h"
#include "matrix.h"
#include <chrono>
#include <thread>

#define ROTATION_THRESHOLD 90
#define ROTATION_STEP 2
class CubeSide{
  public:
  char axis;
  float_vector center = {0.0f, 0.0f, 0.0f};
  std::vector<float_vector> cubes_center;
  
  CubeSide(float_vector n_center, char n_axis){
    center = n_center;
    axis = n_axis;
    generate_cubes_center(n_center, n_axis);
  }

  void generate_cubes_center(float_vector n_center, char n_axis){
    float_vector x_axis;
    float_vector y_axis;
    float_vector z_axis;

    switch(n_axis){
        case 'x':
          x_axis = {n_center[0]};
          y_axis = {n_center[1] - 1, n_center[1], n_center[1] + 1};
          z_axis = {n_center[2] - 1, n_center[2], n_center[2] + 1};
          break;
        case 'y':
          x_axis = {n_center[0] - 1, n_center[0], n_center[0] + 1};
          y_axis = {n_center[1]};
          z_axis = {n_center[2] - 1, n_center[2], n_center[2] + 1}; 
          break;
        case 'z': 
          x_axis = {n_center[0] - 1, n_center[0], n_center[0] + 1};
          y_axis = {n_center[1] - 1, n_center[1], n_center[1] + 1};
          z_axis = {n_center[2]};
          break;
    }

    for(int i_x=0; i_x < x_axis.size(); i_x++){
      for(int i_y=0; i_y < y_axis.size(); i_y++){
        for(int i_z=0; i_z < z_axis.size(); i_z++){
          float_vector temp_center = {x_axis[i_x], y_axis[i_y], z_axis[i_z]};
          cubes_center.push_back(temp_center);
        }
      }
    }
    
    std::cout<<"Center"<<std::endl;
    print_vector(n_center);
    std::cout<<"axis "<<n_axis<<std::endl;
    for(int i=0; i<cubes_center.size(); i++){
      print_vector(cubes_center[i]);
      std::cout<<std::endl;
    }
  }

  void rotate_by_step(std::vector<Cube*> c_cubes, float value){
    for(int i=0; i< c_cubes.size(); i++){
      // Move vertices
      float_vector new_vertices = c_cubes[i]->get_vertices_without_texture();
      c_cubes[i]->update_vertices_without_texture(MatrixCubeRotate::rotate(value, axis, center, new_vertices));
    }
  }

  void print_center(){
    return print_vector(center);
  }
};

class RubikCube
{
public:
  std::vector<CubeSide*> sides;
  std::vector<Cube*> all_cubes;
  std::vector<Cube*> cubes_to_move;
  // Locked movement vars
  int rotation_remaining = 0;
  float direction = 1.0f;
  int side_index = 0;

  unsigned int VBO;
  RubikCube(unsigned int &nVBO)
  {
    VBO = nVBO;
    float_vector sides_center_raw = get_initial_sides_center();
    string_vector colors = {"g", "r", "b", "o", "w", "y"};
    std::vector<char> axis = {'z','x','z','x','y','y'};
    int axis_index = 0;
    for(int i=0; i < sides_center_raw.size(); i+=3){
      float_vector n_center_cube = {sides_center_raw[i], sides_center_raw[i+1], sides_center_raw[i+2]};
      CubeSide* cube_side = new CubeSide(n_center_cube, axis[axis_index]);
      
      for(int cb_index = 0; cb_index < cube_side->cubes_center.size(); cb_index++){
        Cube* c_cube = get_cube_by_center(cube_side->cubes_center[cb_index]);
        if(c_cube == nullptr){
          c_cube = new Cube(cube_side->cubes_center[cb_index], colors);
          all_cubes.push_back(c_cube);
        }
      }
      sides.push_back(cube_side);
      axis_index++;
    }
  }

  void draw(){
    float_vector all_vertices = get_all_vertices();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, all_vertices.size() * sizeof(all_vertices[0]),  all_vertices.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }

  void update_draw(){
    if(rotation_remaining == 0) {
      return;
    }
    sides[side_index]->rotate_by_step(cubes_to_move, direction * ROTATION_STEP);
    rotation_remaining -= ROTATION_STEP;
    
    if(rotation_remaining == 0){
      for(int i=0; i< cubes_to_move.size(); i++){
        float_vector new_vertices = cubes_to_move[i]->center;
        cubes_to_move[i]->update_center(MatrixCubeRotate::rotate(ROTATION_THRESHOLD * direction, sides[side_index]->axis, sides[side_index]->center, new_vertices));
      }
    }

    float_vector all_vertices = get_all_vertices();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, all_vertices.size() * sizeof(all_vertices[0]),  all_vertices.data(), GL_STATIC_DRAW);
  }

  Cube* get_cube_by_center(float_vector n_center){
    for(int i=0; i<all_cubes.size(); i++){
      if(all_cubes[i]->center == n_center){
        return all_cubes[i];
      }
    }
    return nullptr;
  }

  std::vector<Cube*> get_cubes_from_side(int s_index){
    CubeSide * current_side = sides[s_index];  
    std::vector<Cube*> cubes_from_side;
    for(int i=0; i < current_side->cubes_center.size(); i++){
      Cube* c_cube = get_cube_by_center(current_side->cubes_center[i]);
      if(c_cube != nullptr){
        cubes_from_side.push_back(c_cube);
      }
    }
    return cubes_from_side;
  }

  float_vector get_all_vertices(){
    float_vector all_vertices = {};
    for(int i=0; i<all_cubes.size(); i++){
      all_vertices.insert(all_vertices.end(), all_cubes[i]->vertices.begin(),all_cubes[i]->vertices.end());
    }
    return all_vertices;
  }

  void set_rotation(int n_side_index, float n_direction){
    if(rotation_remaining > 0) {
      std::cout<<"rotation in progress"<<std::endl;
      return;
    }

    direction = n_direction;
    side_index = n_side_index;
    rotation_remaining = ROTATION_THRESHOLD;
    cubes_to_move = get_cubes_from_side(side_index);
  }

  float_vector get_initial_sides_center(){
    float_vector positions = {
      // Horizontal
      // CAMADA 1 (rotate z)
      0, 0, -1,
      // CAMADA 2 (rotate x)
      1, 0, 0,
      // CAMADA 3 (rotate z)
      0, 0, 1, 
      // CAMADA 4 (rotate x)
      -1, 0, 0, 

      // Vertical
      // CAMADA 5 | top (rotate y)
      0, 1, 0,
      // CAMADA 6 | bottom (rotate y)
      0, -1, 0
    };
    return positions;
  }
};

#endif