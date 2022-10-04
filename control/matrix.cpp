#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#define PI 3.14159265
#define COLS 4

class MatrixTransform{
  public:
    float value;
    std::vector <float> values;
    char axis; // x, y, z
    char type; // r: rotation | t: translation | s: scale 
    std::vector <float> matrix = {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };
    MatrixTransform(float n_value, char n_axis, char n_type){
      value = n_value;
      axis = n_axis;
      type = n_type;

      switch (type){
      case 'r': // rotation
        createRotationMatrix();
        break;
      case 't': // translation
        createTranslationMatrix();
        break;
      case 's': // scale
        createScaleMatrix();
        break;
      default:
        break;
      }
    };

    MatrixTransform(std::vector<float> n_values, char n_type){
      values = n_values;
      type = n_type;

      switch (type){
      case 't': // translation
        createTranslationMatrixFromValues();
        break;
      case 's': // scale
        createScaleMatrixFromValues();
        break;
      default:
        break;
      }
    };


    void createRotationMatrix(){
      float sinValue = (float) sin(value*PI/180);  
      float cosValue = (float) cos(value*PI/180);

      switch(axis){
          case 'x': 
            matrix[5] = cosValue;
            matrix[6] = (-1) * sinValue;
            matrix[9] = sinValue;
            matrix[10] = cosValue;
            break;
          case 'y': 
            matrix[0] = cosValue;
            matrix[2] = sinValue;
            matrix[8] = (-1) * sinValue;
            matrix[10] = cosValue;
            break;
          case 'z':
            matrix[0] = cosValue;
            matrix[1] = (-1) * sinValue;
            matrix[4] = sinValue;
            matrix[5] = cosValue;
            break;
          default: break;
      }
    }

    void createTranslationMatrix(){
      switch(axis){
          case 'x': 
            matrix[3] = value;
            break;
          case 'y': 
            matrix[7] = value;
            break;
          case 'z':
            matrix[11] = value;
            break;
          default: break;
      }
    }

    void createScaleMatrix(){
      switch(axis){
          case 'x': 
            matrix[0] = value;
            break;
          case 'y': 
            matrix[5] = value;
            break;
          case 'z':
            matrix[10] = value;
            break;
          default: break;
      }
    }

    void createTranslationMatrixFromValues(){
      matrix[3] = values[0];
      matrix[7] = values[1];
      matrix[11] = values[2];
    }

    void createScaleMatrixFromValues(){
      matrix[0] = values[0];
      matrix[5] = values[1];
      matrix[10] = values[2];
    }

    void print(){
      for(int i=0; i<matrix.size(); i+=4){
        std::cout<<matrix[i]<<" "<<matrix[i+1]<<" "<<matrix[i+2]<<" "<<matrix[i+3]<<std::endl;
      }
    }

    std::vector<float> multiply(std::vector<float> vertices){
      std::vector<float> new_vertices;
      for(int row = 0; row < vertices.size() / 3; row++){
        std::vector<float> vert_row(vertices.begin() + (row * 3), vertices.begin() + (row*3+3));
        std::vector<float> new_row = matrixMultiplyVector(vert_row);
        new_vertices.insert(new_vertices.end(), new_row.begin(), new_row.end());  
      }
      return new_vertices;
    }

    std::vector<float> matrixMultiplyVector(std::vector<float> vertices_row){
      std::vector<float> vertices_cp(vertices_row);
      std::vector<float> result;
      vertices_cp.push_back(1);
      for(int curr_row=0; curr_row < 4; curr_row++){
        float calc_value = 0.0;
        for(int curr_col=0; curr_col<COLS; curr_col++){
            calc_value = calc_value + ( matrix[(curr_row * COLS) + curr_col ] * vertices_cp[curr_col]);
        }
        result.push_back(calc_value);
      }
      result.pop_back();
      return result;
    }

    void printVector(std::vector<float> vect){
      for(int i=0; i<vect.size(); i++){
        std::cout<<vect[i]<<" ";
      }
      std::cout<<std::endl;
    }

};
