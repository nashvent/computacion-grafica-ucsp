#ifndef CUBE_H
#define CUBE_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "seed_data.h"

class Cube{
public:
  float_vector vertices;
  float_vector center;
  string_vector colors;
  std::map<char, char> side_color;

  Cube(float_vector n_center, string_vector n_colors, std::vector<char> n_side_notation){
    colors = n_colors;
    center = n_center;
    vertices = create_vertices(center);
    //                        colors  {"b", "r", "g", "o", "y", "w"};
    std::vector<char> side_notation = {'f', 'r', 'b','l','u','d'};
    for(int i=0; i<n_colors.size(); i++){
      side_color[side_notation[i]] = n_colors[i][0];
    }
  }

  float_vector create_vertices(float_vector n_center){
    float_vector new_vertices;
    int color_count = 0;
    int color_index = 0;
    float_vector current_color = source_colors[colors[color_index]];
    int_vector colors_index = {0, 1, 2, 2, 3, 0};
    for(int i=0; i<source_cube_vertices.size(); i+=3){
      // Vertices
      new_vertices.push_back(source_cube_vertices[i] + n_center[0]);
      new_vertices.push_back(source_cube_vertices[i+1] + n_center[1]);
      new_vertices.push_back(source_cube_vertices[i+2] + n_center[2]);

      // Colors
      if(color_count >= colors_index.size()){
        color_count = 0;
        color_index += 1;
        current_color = source_colors[colors[color_index]];
      }
      new_vertices.push_back(current_color[ colors_index[color_count] * 2 ]);
      new_vertices.push_back(current_color[ colors_index[color_count] * 2 + 1 ]);
      color_count += 1;
    }
    return new_vertices;
  }

  float_vector create_vertices_without_texture(float_vector n_center){
    float_vector new_vertices;
    for(int i=0; i<source_cube_vertices.size(); i+=3){
      // Vertices
      new_vertices.push_back(source_cube_vertices[i] + n_center[0]);
      new_vertices.push_back(source_cube_vertices[i+1] + n_center[1]);
      new_vertices.push_back(source_cube_vertices[i+2] + n_center[2]);
    }
    return new_vertices;
  }

  void print_center(){
    std::cout<<center[0]<<" "<<center[1]<<" "<<center[2]<<std::endl;
  }

  float_vector get_vertices_without_texture(){
    float_vector vert = {};
    for(int i=0; i<vertices.size(); i+=5){
      vert.push_back(vertices[i]);
      vert.push_back(vertices[i+1]);
      vert.push_back(vertices[i+2]);
    }
    return vert;
  }

  void update_vertices_without_texture(float_vector n_vertices){
    int n_i = 0;
    for(int i=0; i<vertices.size(); i+=5){
      vertices[i] = n_vertices[n_i];
      vertices[i+1] = n_vertices[n_i+1];
      vertices[i+2] = n_vertices[n_i+2];
      n_i+=3;
    }
  }

  void update_center(float_vector n_center){
    center = n_center;
  }

  void print_color(char notation){
    std::cout<<"color "<<side_color[notation]<<std::endl;
  }

  void update_side_color(char side_notation, float direction){
    std::cout<<"update_side_color "<<side_notation<<std::endl;
    std::map<char, char> temp_side_color(side_color);
    std::vector<char> swaps_moves(8);
    if(side_notation=='l' || side_notation=='r'){
      if(direction > 0){
        swaps_moves = {'u','f',  'f', 'd',  'd', 'b',  'b', 'u'};
      } else {
        swaps_moves = {'u','b',  'b', 'd',  'd', 'f',  'f', 'u'};
      }
    }

    if(side_notation=='u' || side_notation=='d'){
      if(direction > 0){
        swaps_moves = {'f','l',  'l', 'b',  'b', 'r',  'r', 'f'};
      } else {
        swaps_moves = {'f','r',  'r', 'b',  'b', 'l',  'l', 'f'};
      }
    }
    
    if(side_notation=='f' || side_notation=='b'){
      if(direction > 0){
        swaps_moves = {'l','d',  'd', 'r',  'r', 'u',  'u', 'l'};
      } else {
        swaps_moves = {'l','u',  'u', 'r',  'r', 'd',  'd', 'l'};
      }
    }
    for(int i=0; i < swaps_moves.size(); i=i+2){
      side_color[swaps_moves[i]] = temp_side_color[swaps_moves[i+1]];
    }
  }

};
#endif