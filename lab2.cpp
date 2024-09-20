#include <iostream>
#include <cmath>

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

  if (function(a) > 0) {
    std::swap(a, b);
  }
}

double BisectionMethod(double (*function)(double), double a, double b, double eps) {
  CheckInput(function, a, b);

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

double SecantMethod(double (*function)(double), double a, double b, double eps) {
  CheckInput(function, a, b);

  while (std::abs(function(a)) > eps) {
    a = b - function(b) * (a - b) / (function(a) - function(b));

    if (function(b) * function(a) < 0) {
      std::swap(a, b);
    }
  }

  return a;
}

int main() {
  // test

  std::cout << BisectionMethod(&f, -10, -2, 0.001) << ' ' << BisectionMethod(&f, -2, 5, 0.001) << ' ' << BisectionMethod(&f, 5, 10, 0.001) << '\n';
  std::cout << SecantMethod(&f, -10, -2, 0.001) << ' ' << SecantMethod(&f, -2, 5, 0.001) << ' ' << SecantMethod(&f, 5, 10, 0.001) << '\n';

  std::cout << BisectionMethod(&f2, -3, -2, 0.001) << ' ' << BisectionMethod(&f2, -1, 0, 0.001) << ' ' << BisectionMethod(&f2, 2, 3, 0.001) << '\n';
  std::cout << SecantMethod(&f2, -3, -2, 0.001) << ' ' << SecantMethod(&f2, -1, 0, 0.001) << ' ' << SecantMethod(&f2, 2, 3, 0.001) << '\n';
}