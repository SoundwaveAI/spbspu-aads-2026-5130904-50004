#include "evaluator.hpp"
#include <stdexcept>
#include <limits>
#include <cctype>
#include <string>
#include "stack.hpp"

namespace
{
  using lim_t = std::numeric_limits< long long >;
  struct OpLess
  {
    static int getPrecedence(const std::string& op)
    {
      if (op == "*" || op == "/" || op == "%")
      {
        return 3;
      }
      if (op == "+" || op == "-")
      {
        return 2;
      }
      if (op == "##")
      {
        return 1;
      }
      return 0;
    }

    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
      return getPrecedence(lhs) < getPrecedence(rhs);
    }
  };

  long long safeMod(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("Division by zero");
    }
    long long res = a % b;
    if (res < 0)
    {
      res += (b < 0) ? -b : b;
    }
    return res;
  }

  long long safeAdd(long long a, long long b)
  {
    if ((b > 0 && a > lim_t::max() - b) || (b < 0 && a < lim_t::min() - b))
    {
      throw std::overflow_error("Addition overflow");
    }
    return a + b;
  }

  long long safeSub(long long a, long long b)
  {
    if ((b < 0 && a > lim_t::max() + b) || (b > 0 && a < lim_t::min() + b))
    {
      throw std::overflow_error("Subtraction overflow");
    }
    return a - b;
  }

  long long safeMul(long long a, long long b)
  {
    if (a > 0)
    {
      if (b > 0)
      {
        if (a > lim_t::max() / b)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
      else
      {
        if (b < lim_t::min() / a)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
    }
    else
    {
      if (b > 0)
      {
        if (a < lim_t::min() / b)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
      else
      {
        if (a != 0 && b < lim_t::max() / a)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
    }
    return a * b;
  }

  void processOperation(kuchukbaeva::Stack< long long >& values, kuchukbaeva::Stack< std::string >& ops)
  {
    if (ops.empty() || values.getSize() < 2)
    {
      throw std::logic_error("Invalid expression format");
    }
    std::string op = ops.top();
    ops.pop();
    long long v2 = values.top();
    values.pop();
    long long v1 = values.top();
    values.pop();

    if (op == "+")
    {
      values.push(safeAdd(v1, v2));
    }
    else if (op == "-")
    {
      values.push(safeSub(v1, v2));
    }
    else if (op == "*")
    {
      values.push(safeMul(v1, v2));
    }
    else if (op == "##")
    {
      values.push(kuchukbaeva::concatenateNumbers(v1, v2));
    }
    else if (op == "/")
    {
      if (v2 == 0)
      {
        throw std::logic_error("Division by zero");
      }
      values.push(v1 / v2);
    }
    else if (op == "%")
    {
      values.push(safeMod(v1, v2));
    }
  }
}

long long kuchukbaeva::concatenateNumbers(long long a, long long b)
{
  if (b < 0)
  {
    throw std::invalid_argument("Cannot concatenate negative number");
  }
  long long multiplier = 1;
  if (b == 0)
  {
    multiplier = 10;
  }
  else
  {
    long long temp = b;
    while (temp > 0)
    {
      multiplier = safeMul(multiplier, 10);
      temp /= 10;
    }
  }
  return safeAdd(safeMul(a, multiplier), b);
}

long long kuchukbaeva::evaluateExpression(const std::string& expression)
{
  Stack< long long > values;
  Stack< std::string > ops;

  size_t start = 0;
  while (start < expression.length())
  {
    while (start < expression.length() && std::isspace(expression[start]))
    {
      ++start;
    }
    if (start >= expression.length())
    {
      break;
    }

    size_t end = start;
    while (end < expression.length() && !std::isspace(expression[end]))
    {
      ++end;
    }

    std::string token = expression.substr(start, end - start);
    start = end;

    if (token == "(")
    {
      ops.push("(");
    }
    else if (token == ")")
    {
      while (!ops.empty() && ops.top() != "(")
      {
        processOperation(values, ops);
      }
      if (!ops.empty())
      {
        ops.pop();
      }
      else
      {
        throw std::logic_error("Mismatched parentheses");
      }
    }
    else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "##")
    {
      OpLess isLess;
      while (!ops.empty() && ops.top() != "(" && !isLess(ops.top(), token))
      {
        processOperation(values, ops);
      }
      ops.push(token);
    }
    else
    {
      size_t parsedChars = 0;
      long long val = std::stoll(token, &parsedChars);
      if (parsedChars != token.length())
      {
        throw std::invalid_argument("Invalid number format");
      }
      values.push(val);
    }
  }

  while (!ops.empty())
  {
    processOperation(values, ops);
  }

  if (values.getSize() != 1)
  {
    throw std::logic_error("Invalid expression format");
  }
  long long result = values.top();
  values.pop();
  return result;
}
