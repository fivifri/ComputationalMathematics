#include <iostream>
#include <cmath>

// Newton's method and fixed-point iteration method for solving nonlinear equations

double f(double x) {
  return 2.2 * x - std::pow(2, x);
}

double df(double x) {
  return 2.2 - std::pow(2, x) * std::log(2);
}

double g(double x) {
  return std::pow(x, 3) - 0.2 * x * x + 0.5 * x - 1;
}

double dg(double x) {
  return 3 * x * x - 0.4 * x + 0.5;
}


double Newton(double (*function)(double), double (*derivative)(double), double a, double b, double eps=0.001) {
  double x = (a + b) / 2;

  while (std::abs(function(x)) > eps) {
    x = x - function(x) / derivative(x);
  }

  return x;
}

double FixedPointIteration(double (*fix)(double), double a, double b, double eps=0.001) {
  double x = (a + b) / 2;
  
  while(std::abs(fix(x) - x) > eps) {
    x = fix(x);
  }

  return x;
}

int main() {
  // test
  std::cout << Newton(&f, &df, 0.5, 1.5) << ' ' << Newton(&f, &df, 1.5, 2.5) << '\n';
  std::cout << Newton(&g, &dg, 0, 1) << '\n';

  std::cout << FixedPointIteration([](double x) {return x - f(x) / 2;}, 0.5, 1.5) << ' ' << FixedPointIteration([](double x) {return x + f(x) / 2;}, 1.5, 2.5) << '\n';
  std::cout << FixedPointIteration([](double x) {return x - g(x) / 2;}, 0, 1) << '\n';
}