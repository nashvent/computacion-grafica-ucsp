#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

std::vector<char> char_vector_to_upper(std::vector<char> vect){
  std::vector<char> format_input;
  for(int i=0; i<vect.size(); i++){
    format_input.push_back(toupper(vect[i]));
  }
  return format_input;
}

std::vector<char> char_vector_to_lower(std::vector<char> vect){
  std::vector<char> format_input;
  for(int i=0; i<vect.size(); i++){
    format_input.push_back(tolower(vect[i]));
  }
  return format_input;
}

std::vector<std::string> parse_solution(std::string solution){
  // node app.js ffbfffrbbdrlrrrddbudbuuuuulddrbdlldurllulldlfurfbbbrff
  // Bprime D Lprime Dprime L D R D2 Rprime D Bprime Dprime B D2 Rprime D R D Rprime D R D R Fprime Rprime F L Dprime Lprime Dprime L D L U Lprime Dprime L Uprime Lprime D2 Lprime D2
  // 
  size_t pos = 0;
  std::vector<std::string> parsed_sol;
  std::cout << solution << std::endl;
  std::string token;
  std::string parsed_token;
  while ((pos = solution.find(" ")) != std::string::npos) {
      token = solution.substr(0, pos);
      parsed_token = "";
      parsed_token += tolower(token[0]);
      if(token[1] == 'p'){
        parsed_token += "i";
      }
      else if(token[1] == '2'){
        parsed_sol.push_back(parsed_token);
      }
      parsed_sol.push_back(parsed_token);
      solution.erase(0, pos + 1);
  }

  parsed_token = "";
  parsed_token += tolower(solution[0]);
  if(solution[1] == 'p'){
    parsed_token += "i";
  }
  else if(solution[1] == '2'){
    parsed_sol.push_back(parsed_token);
  }
  parsed_sol.push_back(parsed_token);
  return parsed_sol;
}

#endif