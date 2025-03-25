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

bool my_isdigit(const std::string& s)
{
  for (size_t i = 0; i < s.size(); i++)
  {
    if (!isdigit(s[i]))
    {
      return false;
    }
  }
  return true;
}
void convert_postfix(std::string s, alymova::Queue< std::string >& queue)
{
  alymova::Stack< std::string > stack;
  for (size_t i = 0; s[i] != '\0'; i++)
  {
    if (s[i] == '(')
    {
      std::string token(1, s[i]);
      stack.push(token);
    }
    if (std::isdigit(s[i]))
    {
      std::string token(1, s[i]);
      i++;
      while (isdigit(s[i]) && s[i] != '\0')
      {
        token.push_back(s[i]);
        i++;
      }
      queue.push(token);
    }
    if (s[i] == '+' || s[i] == '-')
    {
      if (!stack.empty())
      {
        while (stack.top() == "+" || stack.top() == "-")
        {
          queue.push(stack.top());
          stack.pop();
        }
      }
      std::string token(1, s[i]);
      stack.push(token);
    }
    if (s[i] == ')')
    {
      while (stack.top() != "(")
      {
        queue.push(stack.top());
        stack.pop();
      }
      stack.pop();
    }
    if (s[i] == '/' || s[i] == '*')
    {
      if (!stack.empty())
      {
        while (stack.top() == "+" || stack.top() == "-" || stack.top() == "*" || stack.top() == "/")
        {
          queue.push(stack.top());
          stack.pop();
        }
      }
      std::string token(1, s[i]);
      stack.push(token);
    }
  }
  while (!stack.empty())
  {
    queue.push(stack.top());
    stack.pop();
  }
}
long long int count_postfix(alymova::Queue< std::string >& queue)
{
  std::stack< long long int > stack;
  while (!queue.empty())
  {
    if (my_isdigit(queue.front()))
    {
      stack.push(std::stoll(queue.front()));
    }
    else if (queue.front() == "+")
    {
      long long int item2 = stack.top();
      stack.pop();
      long long int item1 = stack.top();
      stack.pop();
      stack.push(item1 + item2);
    }
    else if (queue.front() == "-")
    {
      long long int item2 = stack.top();
      stack.pop();
      long long int item1 = stack.top();
      stack.pop();
      stack.push(item1 - item2);
    }
    else if (queue.front() == "*")
    {
      long long int item2 = stack.top();
      stack.pop();
      long long int item1 = stack.top();
      stack.pop();
      stack.push(item1 * item2);
    }
    else if (queue.front() == "/")
    {
      long long int item2 = stack.top();
      stack.pop();
      long long int item1 = stack.top();
      stack.pop();
      stack.push(item1 / item2);
    }
    queue.pop();
  }
  return stack.top();
}
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
      alymova::Queue< std::string > queue;
      alymova::Stack< std::string > stack;
      std::string s;
      std::getline(*input, s);
      if (s.empty())
      {
        continue;
      }
      convert_postfix(s, queue);
      res.push(count_postfix(queue));
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  while (!res.empty())
  {
    std::cout << res.top() << " ";
    res.pop();
  }
  if (argc == 2)
  {
    file.close();
  }
}


/*int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Incorrect arguments\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  while (input.is_open() && !input.eof())
  {
    std::string s;
    std::getline(input, s);
    std::cout << s << "\n";
  }
  input.close();
}*/
