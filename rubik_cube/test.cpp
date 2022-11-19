#include <iostream>
#include <vector>
#include "matrix.h"
using namespace std;

int main(){
    vector<float> cube_center = {-1, 0, -1};
    vector<float> side_center = {0, 0, -1};
    float value = 90;
    char axis = 'z';
    /*
     value 90
    axis z
    side center 0,0,-1,
    cube center 0,1,-1,
    value 90    
    */

    vector<float> result = MatrixCubeRotate::rotate(90, 'z', side_center, cube_center);
    print_vector(result);
    //MatrixTransform* rotate_matrix = new MatrixTransform(value, axis, 'r');
    //rotate_matrix->print();


    return 0;
}