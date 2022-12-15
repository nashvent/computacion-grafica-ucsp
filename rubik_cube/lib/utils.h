#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
      if(token[0] == 'm'){
        std::string first_move = "li";
        std::string second_move = "r";
        if(token[1] == 'p'){
          first_move = "l";
          second_move = "ri";
        }
        else if(token[1] == '2') {
          parsed_sol.push_back(first_move);
          parsed_sol.push_back(second_move);
        }
        parsed_sol.push_back(first_move);
        parsed_sol.push_back(second_move);
      }else{
        parsed_token = "";
        parsed_token += tolower(token[0]);
        if(token[1] == 'p'){
          parsed_token += "i";
        }
        else if(token[1] == '2'){
          parsed_sol.push_back(parsed_token);
        }
        parsed_sol.push_back(parsed_token);
      }
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

const std::string WHITESPACE = " \n\r\t\f\v";
 
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}


#endif