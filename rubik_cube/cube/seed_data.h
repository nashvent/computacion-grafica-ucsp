#ifndef SEED_H
#define SEED_H
#include <vector>
#include <map>

typedef std::vector<float> float_vector;
typedef std::vector<std::string> string_vector;
typedef std::vector<int> int_vector;
typedef std::string string;
typedef std::map<std::string, float_vector> float_vector_map;

float_vector source_cube_vertices =  {
  // 1
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f,  0.5f, -0.5f,
  0.5f,  0.5f, -0.5f,
  -0.5f,  0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,

  // 2
  -0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f,  0.5f,
  -0.5f,  0.5f,  0.5f,

  // 3
  -0.5f, -0.5f,  0.5f,
  0.5f, -0.5f,  0.5f,
  0.5f,  0.5f,  0.5f,
  0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f,  0.5f,
  -0.5f, -0.5f,  0.5f,

  // 4
  0.5f,  0.5f,  0.5f,
  0.5f,  0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f, -0.5f,  0.5f,
  0.5f,  0.5f,  0.5f,

  // up
  -0.5f,  0.5f, -0.5f,
  0.5f,  0.5f, -0.5f,
  0.5f,  0.5f,  0.5f,
  0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f, -0.5f,    

  // down 
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f, -0.5f,  0.5f,
  0.5f, -0.5f,  0.5f,
  -0.5f, -0.5f,  0.5f,
  -0.5f, -0.5f, -0.5f,
};

float_vector green = {
  0.0f, 0.0f,
  0.166f, 0.0f,
  0.166f, 1.0f,
  0.0f, 1.0f,
};

float_vector red = {
  0.167f, 0.0f,
  0.333f, 0.0f,
  0.333f, 1.0f,
  0.167f, 1.0f,
};

float_vector blue = {
  0.334f, 0.0f,
  0.499f, 0.0f,
  0.499f, 1.0f,
  0.334f, 1.0f,
};

float_vector orange = {
  0.501f, 0.0f,
  0.666f, 0.0f,
  0.666f, 1.0f,
  0.501f, 1.0f,
};

float_vector white = {
  0.668f, 0.0f,
  0.833f, 0.0f,
  0.833f, 1.0f,
  0.668f, 1.0f,
};

float_vector yellow = {
  0.834f, 0.0f,
  1.0f, 0.0f,
  1.0f, 1.0f,
  0.834f, 1.0f,
};

float_vector_map source_colors = { 
  {"g", green},
  {"r", red},
  {"b", blue},
  {"o", orange},
  {"w", white},
  {"y", yellow},
};
#endif