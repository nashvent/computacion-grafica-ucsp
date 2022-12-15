#include <iostream>
#include <vector>
//#include "matrix.h"
// #include "lib/solver.h"
#include "lib/matrix.h"
#include "lib/utils.h"
#include "lib/solver.h"

using namespace std;

int main(){
    //                  F         B         L         R         U         D
    //             "RRWGGWGGW YOOYBBYBB GGGOOOOOO BRRBRRBRR WWBWWBWWO YYRYYGYYG";
                   //BWRRWWYG OYYWG BGOBBGRORYO
    
    // GBYRGYGYY OGYOROOGB RRRYWOYOB WGGYYROGO WRRWOBGBR BWBWBBWWW
    // string input = "OOOGGGGGG RRRBBBBBB BBBOOOOOO GGGRRRRRR WWWWWWWWW YYYYYYYYY";
    
    // char rubic[6][10];
    // for(int i=0;i<6;i++){
		//   for(int j=0;j<9;j++)
		// 	  rubic[i][j]=input[ (i*10) + j ];
	  // }
    // vector<string> moves = get_moves(rubic);
    // print_vector(moves);
    // vector<float> cube_center = {-1, 0, -1};
    // vector<float> side_center = {0, 0, -1};
    // float value = 90;
    // char axis = 'z';
    // /*
    //  value 90
    // axis z
    // side center 0,0,-1,
    // cube center 0,1,-1,
    // value 90    
    // */

    // vector<float> result = MatrixCubeRotate::rotate(90, 'z', side_center, cube_center);
    // print_vector(result);
    //MatrixTransform* rotate_matrix = new MatrixTransform(value, axis, 'r');
    //rotate_matrix->print();
    // std::vector<std::string> solution = parse_solution("Bprime D Lprime Dprime L D R D2 Rprime D Bprime Dprime B D2 Rprime D R D Rprime D R D R Fprime Rprime F L Dprime Lprime Dprime L D L U Lprime Dprime L Uprime Lprime D2 Lprime D2");    
    // print_vector(solution);
    std::vector<std::string> solution = get_solution("ffbfffrbbdrlrrrddbudbuuuuulddrbdlldurllulldlfurfbbbrff");
    print_vector(solution);
    return 0;
}