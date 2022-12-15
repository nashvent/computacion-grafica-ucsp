#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
// #define PI 3.14159
const double PI = atan(1) * 4;
#define COLS 4

float n_cos(float degree_value){
  if(abs(degree_value) == 90.0){
    return 0.0;
  }
  return (float) cos(degree_value*PI/180);
}

float n_sin(float degree_value){
  return (float) sin(degree_value*PI/180);  
}


template <typename T>
void print_vector(std::vector<T> vect){
  for(int i=0; i<vect.size(); i++){
    std::cout<<vect[i]<<",";
  }
  std::cout<<std::endl;
}

std::vector<float> sum_vector(std::vector<float> vect1, std::vector<float> vect2){
  std::vector<float> new_vect = {};
  for(int i=0; i<vect1.size(); i++){
    new_vect.push_back(vect1[i] + vect2[i]);
  }
  return new_vect;
}

std::vector<float> sum_matrix_vector(std::vector<float> matrix, std::vector<float> vector){
  std::vector<float> new_matrix = {};
  for(int i=0; i<matrix.size(); i+=3){
    new_matrix.push_back(matrix[i]+vector[i%3]);
    new_matrix.push_back(matrix[i+1]+vector[(i+1)%3]);
    new_matrix.push_back(matrix[i+2]+vector[(i+2)%3]);
  }
  return new_matrix;
}

std::vector<float> div_vector_by_num(std::vector<float> vect1, float n_div){
  std::vector<float> new_vect = {};
  for(int i=0; i<vect1.size(); i++){
    new_vect.push_back(vect1[i] / n_div);
  }
  return new_vect;
}

std::vector<float> mult_vector_by_num(std::vector<float> vect1, float n_mult){
  std::vector<float> new_vect = {};
  for(int i=0; i<vect1.size(); i++){
    new_vect.push_back(vect1[i] * n_mult);
  }
  return new_vect;
}

struct Matrix{
  // n == m -> 2x2, 3x3, 4x4...
  static std::vector<float> multipy(std::vector<float> vect_a, std::vector<float> vect_b, int n = 2){
    std::vector<float> result(vect_a.size(), 0.0);
    for(int i=0; i<result.size(); i++){
      int a = i%n;
      int step = (int) i/n;
      for(int b=0; b<n; b++){
        result[i] += (vect_a[(n*step)+b] * vect_b[(b*n)+a]);
      }
    }
    return result;
  }

  static void print(std::vector<float> vect, int n=2){
    for(int i=0; i<vect.size(); i++){
      if(i!=0 && i%n==0){
        std::cout<<std::endl;
      }
      std::cout<<vect[i]<<",";
    }
  }
};

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
      float sinValue = n_sin(value);  
      float cosValue = n_cos(value);

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


struct MatrixCubeRotate{
  static std::vector<float> rotate(float value, float axis, std::vector<float> center, std::vector<float> new_vertices){
    MatrixTransform* rotate_matrix = new MatrixTransform(value, axis, 'r');
    MatrixTransform* translate_back = new MatrixTransform(center, 't');
    MatrixTransform* translate_forward = new MatrixTransform(mult_vector_by_num(center, -1.0f), 't');


    std::vector<float> new_vertices_translated_forward = translate_forward->multiply(new_vertices);
    std::vector<float> new_vertices_rotated = rotate_matrix->multiply(new_vertices_translated_forward);
    std::vector<float> new_vertices_translated_back = translate_back->multiply(new_vertices_rotated);
    return new_vertices_translated_back;
  }

  static std::vector<float> separate(std::vector<float> n_values, std::vector<float> new_vertices, float per_value){
    std::vector<float> values = mult_vector_by_num(n_values, per_value);
    MatrixTransform* translate_matrix = new MatrixTransform(values, 't');
    return translate_matrix->multiply(new_vertices);
  }
};

#endif