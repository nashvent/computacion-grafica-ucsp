#include <iostream>
using namespace std;

class Star {
  public:
    float *points;
    int *indexes;
    Star(){
        float starPoints[] = {
            0.0f, 0.5f, 0.0f, 
            0.5f, 0.2f, 0.0f,
            0.35f, -0.5f, 0.0f,
            0.0f, -0.2f, 0.0f,
            -0.35f, -0.5f, 0.0f,
            -0.5f, 0.2f, 0.0f, 
        };
        points = starPoints;
        unsigned int indices[] = {
            0, 2, 3,
            0, 4, 3,   
            5, 3, 1,
        };
        indexes = indexes;
    }
};
