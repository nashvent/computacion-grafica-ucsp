#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<char> char_vector_to_upper(std::vector<char> vect)
{
  std::vector<char> format_input;
  for (int i = 0; i < vect.size(); i++)
  {
    format_input.push_back(toupper(vect[i]));
  }
  return format_input;
}

std::vector<char> char_vector_to_lower(std::vector<char> vect)
{
  std::vector<char> format_input;
  for (int i = 0; i < vect.size(); i++)
  {
    format_input.push_back(tolower(vect[i]));
  }
  return format_input;
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

std::string trim(const std::string &s)
{
  return rtrim(ltrim(s));
}

float random_float(float min, float max)
{
    // this  function assumes max > min, you may want 
    // more robust error checking for a non-debug build
    assert(max > min); 
    float random = ((float) rand()) / (float) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;  
    return (random*range) + min;
}

std::vector<std::string> split_string(std::string s, std::string delimiter = " "){
  std::vector<std::string> splited;
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
      token = s.substr(0, pos);
      splited.push_back(token);
      s.erase(0, pos + delimiter.length());
  }
  splited.push_back(s);
  return splited; 
}

std::vector<std::string> parse_solution_fridrich(std::string solution)
{
  size_t pos = 0;
  std::vector<std::string> parsed_sol;
  std::vector<std::string> splited_solution = split_string(solution);

  for(int i=0; i<splited_solution.size(); i++){
    std::string token = splited_solution[i];
    std::string parsed_token;

    if (token[0] == 'm')
    {
      std::string first_move = "li";
      std::string second_move = "r";
      if (token[1] == 'p')
      {
        first_move = "l";
        second_move = "ri";
      }
      else if (token[1] == '2')
      {
        parsed_sol.push_back(first_move);
        parsed_sol.push_back(second_move);
      }
      parsed_sol.push_back(first_move);
      parsed_sol.push_back(second_move);
    }
    else
    {
      parsed_token = "";
      parsed_token += tolower(token[0]);
      if (token[1] == 'p')
      {
        parsed_token += "i";
      }
      else if (token[1] == '2')
      {
        parsed_sol.push_back(parsed_token);
      }
      parsed_sol.push_back(parsed_token);
    }
  }
  return parsed_sol;
}

std::vector<std::string> parse_solution_kociemba(std::string solution)
{
  size_t pos = 0;
  std::vector<std::string> parsed_sol;
  std::vector<std::string> splited_solution = split_string(solution);

  for(int i=0; i<splited_solution.size(); i++){
    std::string token = splited_solution[i];
    std::string parsed_token;
    parsed_token = "";
    parsed_token += tolower(token[0]);
    if (token[1] == '\'')
    {
      parsed_token += "i";
    }
    else if (token[1] == '2')
    {
      parsed_sol.push_back(parsed_token);
    }
    parsed_sol.push_back(parsed_token);
  }
  return parsed_sol;
}

float get_random_difference(float current_value, float limit_value){
  if(current_value + 0.5 >= limit_value){
    return 0.05;
  }
  return random_float(0.001, 0.499);
}

#endif