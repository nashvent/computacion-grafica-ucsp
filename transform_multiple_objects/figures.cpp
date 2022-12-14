#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "vector.cpp"
using namespace std;

class GLObject {
    public:
      // For draw
      unsigned int VBO, EBO;
      vector<float> points;
      vector<unsigned int> indices;
      int pointsSizeOf;
      int indicesSizeOf;
      int pointsLength;

      void generateBuffers(){
          glGenBuffers(1, &VBO);
          glGenBuffers(1, &EBO);
      }

      void bindBuffers(){
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, pointsSizeOf, &points[0], GL_STATIC_DRAW);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSizeOf, &indices[0], GL_STATIC_DRAW);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      }

      void reBindPoints(){
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, pointsSizeOf, &points[0], GL_STATIC_DRAW);
      }

      void deleteBuffers(){
          glDeleteBuffers(1, &VBO);
          glDeleteBuffers(1, &EBO);
      }

      void setInitialPosition(Vector *vect){
        vect->addition(&points[0], pointsLength);
      }


};


class Star: public GLObject {
  public:
    // float *points;
    // unsigned int *indices;
    // int pointsSizeOf;
    // int indicesSizeOf;
    // int pointsLength;
    Star(){
        points = {
            0.0f, 0.2f, 0.0f, // A 
            0.1f, 0.1f, 0.0f, // B
            0.05f, -0.05f, 0.0f, // C
            0.0f, 0.037f, 0.0f, // D
            -0.05f, -0.05f, 0.0f, // E
            -0.1f, 0.1f, 0.0f, // F
        };
        pointsSizeOf = sizeof(points[0]);
        pointsLength = points.size();
        indices = {
            0, 2, 3,
            0, 4, 3,   
            5, 3, 1,
        };
        indicesSizeOf = indices.size();
    }    
};


// class House: public GLObject {
//   public:
//     // float *points;
//     // unsigned int *indices;
//     // int pointsSizeOf;
//     // int indicesSizeOf;
//     // int pointsLength;
//     House(){
//         float startPoints[] = {
//             0.0f, 0.2f, 0.0f, // L 
//             0.1f, 0.1f, 0.0f, // M
//             -0.1f, -0.1f, 0.0f, // K
//             -0.05f, 0.1f, 0.0f, // H
//             0.05f, 0.1f, 0.0f, // J
//             0.05f, -0.1f, 0.0f, // I
//             -0.05f, -0.1f, 0.0f, // G
//         };
//         pointsSizeOf = sizeof(startPoints);
//         pointsLength = pointsSizeOf / sizeof(float);
//         points = startPoints;
//         unsigned int indexes[] = {
//             0, 1, 2,
//             3, 4, 5,   
//             3, 5, 6,
//         };
//         indices = indexes;
//         indicesSizeOf = sizeof(indexes);
//     }    
// };
