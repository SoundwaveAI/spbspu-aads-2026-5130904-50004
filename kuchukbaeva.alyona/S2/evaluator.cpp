#include "evaluator.hpp"
#include <stdexcept>
#include <limits>
#include <cctype>
#include "stack.hpp"

long long kuchukbaeva::concatenateNumbers(long long a, long long b)
{
  if (b < 0) {
    throw std::invalid_argument("Cannot concatenate negative number");
  }
  long long multiplier = 1;
  if (b == 0) {
    multiplier = 10;
  } else {
    long long temp = b;
    while (temp > 0) {
      multiplier = safeMul(multiplier, 10);
      temp /= 10;
    }
  }
  return safeAdd(safeMul(a, multiplier), b);
}

long long kuchukbaeva::evaluateExpression(const std::string& expression)
{
  kuchukbaeva::Stack< long long > values;
  kuchukbaeva::Stack< std::string > ops;

  for (size_t i = 0; i < expression.length(); ++i) {
    if (std::isspace(expression[i])) {
      continue;
    }

    if (std::isdigit(expression[i])) {
      long long val = 0;
      while (i < expression.length() && std::isdigit(expression[i])) {
        val = safeAdd(safeMul(val, 10), (expression[i] - '0'));
        ++i;
      }
      --i;
      values.push(val);
    } else if (expression[i] == '(') {
      ops.push("(");
    } else if (expression[i] == ')') {
      while (!ops.isEmpty()) {
        std::string top = ops.drop();
        if (top == "(") {
          break;
        }
        long long v2 = values.drop();
        long long v1 = values.drop();
        if (top == "+") {
          values.push(safeAdd(v1, v2));
        } else if (top == "-") {
          values.push(safeSub(v1, v2));
        } else if (top == "*") {
          values.push(safeMul(v1, v2));
        } else if (top == "«") {
          values.push(kuchukbaeva::concatenateNumbers(v1, v2));
        } else if (top == "/") {
          if (v2 == 0) {
            throw std::logic_error("Division by zero");
          }
          values.push(v1 / v2);
        } else if (top == "%") {
          if (v2 == 0) {
            throw std::logic_error("Division by zero");
          }
          values.push(v1 % v2);
        }
      }
    } else {
      std::string op = "";
      if (expression[i] == '<' && i + 1 < expression.length() && expression[i + 1] == '<') {
        op = "«";
        ++i;
      } else {
        op = std::string(1, expression[i]);
      }

      while (!ops.isEmpty()) {
        std::string top = ops.drop();
        if (top == "(" || getPrecedence(top) < getPrecedence(op)) {
          ops.push(top);
          break;
        }
        long long v2 = values.drop();
        long long v1 = values.drop();
        if (top == "+") {
          values.push(safeAdd(v1, v2));
        } else if (top == "-") {
          values.push(safeSub(v1, v2));
        } else if (top == "*") {
          values.push(safeMul(v1, v2));
        } else if (top == "«") {
          values.push(kuchukbaeva::concatenateNumbers(v1, v2));
        } else if (top == "/") {
          if (v2 == 0) {
            throw std::logic_error("Division by zero");
          }
          values.push(v1 / v2);
        } else if (top == "%") {
          if (v2 == 0) {
            throw std::logic_error("Division by zero");
          }
          values.push(v1 % v2);
        }
      }
      ops.push(op);
    }
  }

  while (!ops.isEmpty()) {
    std::string top = ops.drop();
    long long v2 = values.drop();
    long long v1 = values.drop();
    if (top == "+") {
      values.push(safeAdd(v1, v2));
    } else if (top == "-") {
      values.push(safeSub(v1, v2));
    } else if (top == "*") {
      values.push(safeMul(v1, v2));
    } else if (top == "«") {
      values.push(kuchukbaeva::concatenateNumbers(v1, v2));
    } else if (top == "/") {
      if (v2 == 0) {
        throw std::logic_error("Division by zero");
      }
      values.push(v1 / v2);
    } else if (top == "%") {
      if (v2 == 0) {
        throw std::logic_error("Division by zero");
      }
      values.push(v1 % v2);
    }
  }
  return values.drop();
}
