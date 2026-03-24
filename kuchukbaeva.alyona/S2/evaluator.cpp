#include "evaluator.hpp"
#include <stdexcept>
#include <limits>
#include <cctype>
#include "stack.hpp"

namespace {
  long long safeMod(long long a, long long b) {
    if (b == 0) {
      throw std::logic_error("Division by zero");
    }
    long long res = a % b;
    if (res < 0) {
      res += (b < 0) ? -b : b;
    }
    return res;
  }

  int getPrecedence(const std::string& op) {
    if (op == "*" || op == "/" || op == "%") {
      return 3;
    }
    if (op == "+" || op == "-") {
      return 2;
    }
    if (op == "##") {
      return 1;
    }
    return 0;
  }

  long long safeAdd(long long a, long long b) {
    if ((b > 0 && a > std::numeric_limits< long long >::max() - b) ||
        (b < 0 && a < std::numeric_limits< long long >::min() - b)) {
      throw std::overflow_error("Addition overflow");
    }
    return a + b;
  }

  long long safeSub(long long a, long long b) {
    if ((b < 0 && a > std::numeric_limits< long long >::max() + b) ||
        (b > 0 && a < std::numeric_limits< long long >::min() + b)) {
      throw std::overflow_error("Subtraction overflow");
    }
    return a - b;
  }

  long long safeMul(long long a, long long b) {
    if (a > 0) {
      if (b > 0) {
        if (a > std::numeric_limits< long long >::max() / b) {
          throw std::overflow_error("Multiplication overflow");
        }
      } else {
        if (b < std::numeric_limits< long long >::min() / a) {
          throw std::overflow_error("Multiplication overflow");
        }
      }
    } else {
      if (b > 0) {
        if (a < std::numeric_limits< long long >::min() / b) {
          throw std::overflow_error("Multiplication overflow");
        }
      } else {
        if (a != 0 && b < std::numeric_limits< long long >::max() / a) {
          throw std::overflow_error("Multiplication overflow");
        }
      }
    }
    return a * b;
  }
  void processOperation(kuchukbaeva::Stack< long long >& values, kuchukbaeva::Stack< std::string >& ops) {
    std::string op = ops.top();
    ops.drop();
    long long v2 = values.top();
    values.drop();
    long long v1 = values.top();
    values.drop();

    if (op == "+") {
      values.push(safeAdd(v1, v2));
    } else if (op == "-") {
      values.push(safeSub(v1, v2));
    } else if (op == "*") {
      values.push(safeMul(v1, v2));
    } else if (op == "##") {
      values.push(kuchukbaeva::concatenateNumbers(v1, v2));
    } else if (op == "/") {
      if (v2 == 0) {
        throw std::logic_error("Division by zero");
      }
      values.push(v1 / v2);
    } else if (op == "%") {
      values.push(safeMod(v1, v2));
    }
  }
}

long long kuchukbaeva::concatenateNumbers(long long a, long long b) {
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

long long kuchukbaeva::evaluateExpression(const std::string& expression) {
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
      while (!ops.isEmpty() && ops.top() != "(") {
        processOperation(values, ops);
      }
      if (!ops.isEmpty()) {
        ops.drop();
      }
    } else {
      std::string op = "";
      if (expression[i] == '#' && i + 1 < expression.length() && expression[i + 1] == '#') {
        op = "##";
        ++i;
      } else {
        op = std::string(1, expression[i]);
      }

      while (!ops.isEmpty() && ops.top() != "(" && getPrecedence(ops.top()) >= getPrecedence(op)) {
          processOperation(values, ops);
      }
      ops.push(op);
    }
  }

  while (!ops.isEmpty()) {
    processOperation(values, ops);
  }
  long long result = values.top();
  values.drop();
  return result;
}
