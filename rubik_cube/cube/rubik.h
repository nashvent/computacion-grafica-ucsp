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
#include "../lib/matrix.h"
#include <chrono>
#include <thread>
#include <map>

#define ROTATION_THRESHOLD 90
#define ROTATION_STEP 10

#define limit 50

class CubeSide{
  public:
  char axis;
  char notation;
  float_vector center = {0.0f, 0.0f, 0.0f};
  std::vector<float_vector> cubes_center;
  
  CubeSide(float_vector n_center, char n_notation){
    center = n_center;
    notation = n_notation;
    axis = get_axis(n_notation);
    std::cout<<"notation "<<n_notation<<std::endl;
    std::cout<<"axis "<<axis<<std::endl;
    generate_cubes_center(n_center, axis);
  }

  void generate_cubes_center(float_vector n_center, char n_axis){
    float_vector x_axis;
    float_vector y_axis;
    float_vector z_axis;

    print_vector(n_center);
    std::cout<<std::endl;
    switch(n_axis){
        case 'x': // l r
          y_axis = {n_center[1] + 1, n_center[1], n_center[1] - 1};
          if(notation == 'l') {
            z_axis = {n_center[2] + 1, n_center[2], n_center[2] - 1};          
          }
          else{
            z_axis = {n_center[2] - 1, n_center[2], n_center[2] + 1};
          }    
          for(int i_y=0; i_y < y_axis.size(); i_y++){
            for(int i_z=0; i_z < z_axis.size(); i_z++){
              float_vector temp_center = {n_center[0], y_axis[i_y], z_axis[i_z]};
              cubes_center.push_back(temp_center);
            }
          }
          
          break;
        case 'y': // u d
          x_axis = {n_center[0] + 1, n_center[0], n_center[0] - 1};
          if(notation== 'u'){
            z_axis = {n_center[2] + 1, n_center[2], n_center[2] - 1}; 
          } else{
            z_axis = {n_center[2] - 1, n_center[2], n_center[2] + 1};
          }

          for(int i_z=0; i_z < z_axis.size(); i_z++){
            for(int i_x=0; i_x < x_axis.size(); i_x++){
              float_vector temp_center = {x_axis[i_x], n_center[1], z_axis[i_z]};
              cubes_center.push_back(temp_center);
            }
          }
        
          
          break;
        case 'z': // f b
          y_axis = {n_center[1] + 1, n_center[1], n_center[1] - 1};
          if(notation=='f'){
            x_axis = {n_center[0] + 1, n_center[0], n_center[0] - 1};
          }
          else{
            x_axis = {n_center[0] - 1, n_center[0], n_center[0] + 1};
          }

          for(int i_y=0; i_y < y_axis.size(); i_y++){
            for(int i_x=0; i_x < x_axis.size(); i_x++){
              float_vector temp_center = {x_axis[i_x], y_axis[i_y], n_center[2]};
              cubes_center.push_back(temp_center);
            }
          }
        
          break;
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

  char get_axis(char n_notation){
    if(n_notation =='f' || n_notation =='b'){
      return 'z';
    }

    if(n_notation =='l' || n_notation =='r'){
      return 'x';
    }

    //if(n_notation =='u' || n_notation =='d'){
    // }
    return 'y';
  }
};

class RubikCube
{
public:
  std::vector<CubeSide*> sides;
  std::map<char, int> map_sides;
  std::vector<Cube*> all_cubes;
  std::vector<Cube*> cubes_to_move;
  // Locked movement vars
  int rotation_remaining = 0;
  int solution_in_progress = 0;
  std::vector<std::string> solution;
  float direction = 1.0f;
  int side_index = 0;
  std::map<char, char> side_by_color;
  unsigned int VBO;
  RubikCube(unsigned int &nVBO)
  {
    VBO = nVBO;
    //float_vector sides_center_raw = get_initial_sides_center();
    string_vector colors = {"g", "o", "b", "r", "w", "y"};
    // std::vector<char> axis = {'z','x','z','x','y','y'};
    std::vector<char> side_notation = {'f', 'l', 'b','r','u','d'};
    for(int i=0; i<side_notation.size(); i++){
      side_by_color[colors[i][0]] = side_notation[i];
    }

    int axis_index = 0;
    for(int i=0; i < side_notation.size(); i++){
      float_vector n_center_cube = get_center_by_notation(side_notation[i]);
      CubeSide* cube_side = new CubeSide(n_center_cube, side_notation[axis_index]);
      
      for(int cb_index = 0; cb_index < cube_side->cubes_center.size(); cb_index++){
        Cube* c_cube = get_cube_by_center(cube_side->cubes_center[cb_index]);
        if(c_cube == nullptr){
          c_cube = new Cube(cube_side->cubes_center[cb_index], colors, side_notation);
          all_cubes.push_back(c_cube);
        }
      }
      sides.push_back(cube_side);
      map_sides[side_notation[axis_index]] = axis_index;
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

  void re_draw(){
    float_vector all_vertices = get_all_vertices();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, all_vertices.size() * sizeof(all_vertices[0]),  all_vertices.data(), GL_STATIC_DRAW);
  }

  void update_draw(){
    if(rotation_remaining == 0 && solution.size() > 0) {
      std::string current_movement = solution[0];
      solution.erase(solution.begin());
      set_movement(current_movement);
    }
    
    // no movement
    if(rotation_remaining == 0) {
      return;
    }


    
    sides[side_index]->rotate_by_step(cubes_to_move, direction * ROTATION_STEP);
    rotation_remaining -= ROTATION_STEP;
    
    if(rotation_remaining == 0){
      for(int i=0; i< cubes_to_move.size(); i++){
        float_vector new_vertices = cubes_to_move[i]->center;
        cubes_to_move[i]->update_center(MatrixCubeRotate::rotate(ROTATION_THRESHOLD * direction, sides[side_index]->axis, sides[side_index]->center, new_vertices));
        cubes_to_move[i]->update_side_color(sides[side_index]->notation, direction);
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

  std::vector<Cube*> get_cubes_from_side(CubeSide* current_side){
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

  void set_movement(std::string move){
    if(rotation_remaining > 0) {
      std::cout<<"rotation in progress"<<std::endl;
      return;
    }
    side_index = map_sides[move[0]];
    
    direction = get_direction_by_move(move[0]);

    if(move.size()>1){
      direction = direction * -1.0;
    }
    cubes_to_move = get_cubes_from_side(side_index);
    rotation_remaining = ROTATION_THRESHOLD;
    // std::cout<<"side_index "<<side_index<<std::endl;
  }

  float_vector get_center_by_notation(char notation){
    switch (notation)
    {
    case 'f':
      return {0, 0, -1};
    case 'l':
      return {1, 0, 0};
    case 'b':
      return {0, 0, 1};
    case 'r':
      return {-1, 0, 0};
    case 'u':
      return {0, 1, 0};
    case 'd':
      return {0, -1, 0};
    }
    return {};
  }

  float get_direction_by_move(char notation){
    switch (notation){
      case 'f':
      case 'r':
      case 'd':
        return 1.0;
      case 'b':
      case 'l':
      case 'u':
        return -1.0;
    }
    return 1.0;
  }

  // float_vector get_initial_sides_center(std::string order="flbrud"){
  //   float_vector positions = {
      
  //     // 'f' (rotate z)
  //     0, 0, -1,
  //     // 'l' (rotate x)
  //     1, 0, 0,
  //     // 'b' (rotate z)
  //     0, 0, 1, 
  //     // 'r' (rotate x)
  //     -1, 0, 0, 

  //     // Vertical
  //     // 'u' | top (rotate y)
  //     0, 1, 0,
  //     // 'd' | bottom (rotate y)
  //     0, -1, 0
  //   };
  //   return positions;
  // }

  void move_from_center(float step, float direction){
    for(int i=0; i<all_cubes.size(); i++){
      Cube* current_cube = all_cubes[i];
      current_cube->update_vertices_without_texture(sum_matrix_vector(current_cube->get_vertices_without_texture(), mult_vector_by_num(current_cube->center, step * direction)));
    }
    re_draw();
  }

  std::vector<char> get_sides_status(std::string notations_order = "bflrdu"){
    std::vector<char> colors;
    for(int i=0; i<notations_order.size(); i++){
      CubeSide *current_side = get_cube_side(notations_order[i]);
      std::cout<<"notation "<<current_side->notation<<std::endl;
      std::vector<Cube*> cubes_from_side = get_cubes_from_side(current_side);
      for(int j=0; j<cubes_from_side.size(); j++){
        colors.push_back(cubes_from_side[j]->get_color(current_side->notation));
      }
    }
    return colors;
  }

  std::vector<char> get_sides_status_notation(std::string notations_order = "bflrdu"){
    std::vector<char> notation_status;
    for(int i=0; i<notations_order.size(); i++){
      CubeSide *current_side = get_cube_side(notations_order[i]);
      std::cout<<"notation "<<current_side->notation<<std::endl;
      std::vector<Cube*> cubes_from_side = get_cubes_from_side(current_side);
      for(int j=0; j<cubes_from_side.size(); j++){
        notation_status.push_back(side_by_color[cubes_from_side[j]->get_color(current_side->notation)]);
      }
    }
    return notation_status;
  }

  CubeSide* get_cube_side(char notation){
    for(int i=0; i<sides.size(); i++){
      if(sides[i]->notation == notation){
        return sides[i];
      }
    }
    return nullptr;
  }

  void set_solution(std::vector<std::string> n_solution){
    solution = n_solution;
  }
};

#endif