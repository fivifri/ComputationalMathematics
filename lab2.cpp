#include <iostream>
#include <cmath>

// Bisection method and false position method for solving nonlinear equations

double f(double x) {
  return 2 * std::pow(x, 3) - 9 * std::pow(x, 2) - 60 * x + 1;
}

double f2(double x) {
  return 5 * std::sin(x) + 1 - x;
}

void CheckInput(double (*function)(double), double& a, double& b) {
  if (function(a) * function(b) > 0) {
    throw std::invalid_argument("The function must take different signs at the ends of the interval");
  }
}

double BisectionMethod(double (*function)(double), double a, double b, double eps=0.001) {
  CheckInput(function, a, b);

  if (function(a) > 0) {
    std::swap(a, b);
  }

  while (std::abs(a - b) > eps) {
    double mid = (a + b) / 2;
    if (function(mid) > 0) {
      b = mid;
    } else {
      a = mid;
    }
  }

 return a;
}

double FalsePositionMethod(double (*function)(double), double a, double b, double eps=0.001) {
  CheckInput(function, a, b);
  if (a > b) {
    std::swap(a, b);
  }

  double c = a;

  while (std::abs(function(c)) > eps) {
    c = a - (b - a) * function(a) / (function(b) - function(a));

    if (function(a) * function(c) < 0) {
      b = c;
    } else {
      a = c;
    }
  }

  return c;
}

int main() {
  // test
  std::cout << BisectionMethod(&f, -10, -2) << ' ' << BisectionMethod(&f, -2, 5) << ' ' << BisectionMethod(&f, 5, 10) << '\n';
  std::cout << FalsePositionMethod(&f, -10, -2) << ' ' << FalsePositionMethod(&f, -2, 5) << ' ' << FalsePositionMethod(&f, 5, 10) << '\n';

  std::cout << BisectionMethod(&f2, -3, -2) << ' ' << BisectionMethod(&f2, -1, 0) << ' ' << BisectionMethod(&f2, 2, 3) << '\n';
  std::cout << FalsePositionMethod(&f2, -3, -2) << ' ' << FalsePositionMethod(&f2, -1, 0) << ' ' << FalsePositionMethod(&f2, 2, 3) << '\n';
}