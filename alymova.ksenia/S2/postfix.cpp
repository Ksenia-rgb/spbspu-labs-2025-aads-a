#include "postfix.hpp"
#include <cstddef>
#include <string>
#include <limits>
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

alymova::Queue< std::string > alymova::convert_postfix(const std::string& s)
{
  Stack< std::string > stack;
  Queue< std::string > queue;
  for (size_t i = 0; i < s.size(); i++)
  {
    if (s[i] == '(')
    {
      std::string token(1, s[i]);
      stack.push(token);
    }
    else if (s[i] == ')')
    {
      while (stack.top() != "(")
      {
        queue.push(stack.top());
        stack.pop();
        if (stack.empty())
        {
          throw std::logic_error("Incorrect expression");
        }
      }
      stack.pop();
    }
    if (std::isdigit(s[i]))
    {
      std::string token;
      while (isdigit(s[i]) && i < s.size())
      {
        token.push_back(s[i]);
        i++;
      }
      queue.push(token);
    }
    /*else if (is_second_priority(s[i]))
    {
      if (!stack.empty())
      {
        while (is_second_priority(stack.top()));
        {
          queue.push(stack.top());
          stack.pop();
          if (stack.empty())
          {
            break;
          }
        }
      }
      std::string token(1, s[i]);
      stack.push(token);
    }*/
    else if (is_first_priority(s[i]) || is_second_priority(s[i]))
    {
      if (!stack.empty())
      {
        //bool for_first_priority = (is_first_priority(s[i])) ? is_first_priority(stack.top()) : true;
        while (is_need_priority(s[i], stack.top()))
        {
          std::cout << "add\n";
          queue.push(stack.top());
          stack.pop();
          if (stack.empty())
          {
            break;
          }
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
  return queue;
}

long long int alymova::count_postfix(alymova::Queue< std::string >& queue)
{
  Stack< long long int > stack;
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
      if (is_overflow_addition(item1, item2))
      {
        throw std::overflow_error("Addition overflow");
      }
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
      if (is_overflow_multi(item1, item2))
      {
        throw std::overflow_error("Multiplication overflow");
      }
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
    else if (queue.front() == "%")
    {
      long long int item2 = stack.top();
      stack.pop();
      long long int item1 = stack.top();
      stack.pop();
      stack.push(my_mod(item1, item2));
    }
    queue.pop();
  }
  return stack.top();
}

bool alymova::my_isdigit(const std::string& s)
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

bool alymova::is_overflow_addition(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
  if (lhs > 0 && rhs > 0)
  {
    return ((max_sum - lhs) < rhs);
  }
  if (lhs < 0 && rhs < 0)
  {
    return ((min_sum - lhs) > rhs);
  }
  return false;
}

bool alymova::is_overflow_multi(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
  if (lhs > 0 && rhs > 0)
  {
    return ((max_sum / lhs) < rhs);
  }
  else if ((lhs > 0 && rhs < 0) || (lhs < 0 && rhs > 0))
  {
    return ((min_sum / lhs) < rhs);
  }
  else if (lhs < 0 && rhs < 0)
  {
    return is_overflow_multi(-lhs, -rhs);
  }
  return false;
}
long long int alymova::my_mod(long long int item1, long long int item2)
{
  long long int quot = item1 / item2;
  if (!is_overflow_multi(item1, item2))
  {
    if (item1 < 0 && item2 < 0)
    {
      quot++;
    }
    if (item1 * item2 < 0)
    {
      quot--;
    }
  }
  else
  {
    throw std::overflow_error("Multiplication overflow");
  }
  long long int res = item1 - quot * item2;
  return res;
}
bool alymova::is_first_priority(char item)
{
  return (item == '*' || item == '/' || item == '%');
}
bool alymova::is_first_priority(std::string token)
{
  return (token == "*" || token == "/" || token == "%");
}
bool alymova::is_second_priority(char item)
{
  return (item == '+' || item == '-');
}
bool alymova::is_second_priority(std::string token)
{
  return (token == "+" || token == "-");
}
bool alymova::is_need_priority(char item, std::string token)
{
  if (is_first_priority(item))
  {
    return (is_first_priority(token) || is_second_priority(token));
  }
  else
  {
    return is_second_priority(token);
  }
}
