#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <queue>
#include <stack>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"
#include "array.hpp"
#include "postfix.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Incorrect arguments\n";
    return 1;
  }
  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Incorrect file\n";
      return 1;
    }
    input = &file;
  }
  alymova::Stack< long long int > res;
  while (!(*input).eof())
  {
    try
    {
      std::string s;
      std::getline(*input, s);
      if (s.empty())
      {
        continue;
      }
      alymova::Queue< std::string > queue = alymova::convert_postfix(s);
      res.push(alymova::count_postfix(queue));
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  if (!res.empty())
  {
    std::cout << res.top();
    res.pop();
  }
  while (!res.empty())
  {
    std::cout << " " << res.top();
    res.pop();
  }
  std::cout << '\n';
  if (argc == 2)
  {
    file.close();
  }
}
