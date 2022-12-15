#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
#include "utils.h"

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
  return result;
}

std::vector<std::string> get_solution(std::string state)
{
  // ffbfffrbbdrlrrrddbudbuuuuulddrbdlldurllulldlfurfbbbrff
  std::string command = "node solver/app.js " + state;
  std::cout << "command " << command << std::endl;
  std::string output = exec(command);
  return parse_solution(output);
}

#endif