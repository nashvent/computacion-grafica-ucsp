#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
#include "utils.h"
// Used https://www.npmjs.com/package/rubiks-cube-solver
std::string exec(std::string command)
{
  char buffer[128];
  std::string result = "";

  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe)
  {
    return "popen failed!";
  }

  while (!feof(pipe))
  {
    if (fgets(buffer, 128, pipe) != NULL)
      result += buffer;
  }
  pclose(pipe);
  return trim(result);
}

// order "frudlb"
std::vector<std::string> get_solution_fridrich(std::string state)
{
  std::string command = "node solver/fridrich.js " + state;
  // std::cout << "command " << command << std::endl;
  std::string output = exec(command);
  if(output.empty()){
    return {};
  }
  return parse_solution_fridrich(output);
}

// order urfdlb
std::vector<std::string> get_solution_kociemba(std::string state){
  std::string command = "node solver/kociemba.js " + state;
  std::cout << "command " << command << std::endl;
  std::string output = exec(command);
  std::cout<<"output "<<output<<std::endl;
  if(output.empty()){
    return {};
  }
  return parse_solution_kociemba(output);
}

#endif