#include <iostream>
#include <cmath>
#include <vector>
#include "lab1.cpp"
#include "plot.h"

double sumOfPowersX(std::vector<double>& array, int k) {
  double sum = 0;

  for (double& x : array) {
    sum += std::pow(x, k);
  }

  return sum;
}

double sumOfPowersXY(std::vector<double>& x, std::vector<double>& y, int k) {
  if (x.size() != y.size()) {
    throw std::invalid_argument("the dimensions x and y must match.");
  }

  double sum = 0;

  for (int i = 0; i < x.size(); ++i) {
    sum += y[i] * std::pow(x[i], k);
  }

  return sum;
}

void fillInX(std::vector<double>& array, std::vector<double>& x) {
  for (int k = 0; k < array.size(); ++k) {
    array[k] = sumOfPowersX(x, k);
  }
}

void fillInXY(std::vector<double>& array, std::vector<double>& x, std::vector<double>& y) {
  for (int k = 0; k < array.size(); ++k) {
    array[k] = sumOfPowersXY(x, y, k);
  }
}

std::vector<double> approximation(int power, std::vector<double>& x, std::vector<double>& y) {
  std::vector<std::vector<double>> matrix(power + 1, std::vector<double>(power + 2));

  std::vector<double> X(2 * power + 1);
  std::vector<double> XY(power + 1);
  fillInX(X, x);
  fillInXY(XY, x, y);

  for (int i = 0; i <= power; ++i) {
    matrix[i][power + 1] = XY[power - i];
  }

  for (int i = 0; i <= power; ++i) {
    for (int j = 0; j <= power; ++j) {
      matrix[i][j] = X[2 * power - i - j];
    }
  }

  return IterativeMethods::Solve(matrix, 0.0001, 1000, "Seidel");
}

double function(std::vector<double>& coef, double x) {
  double y = 0;
  for (int i = 0; i < coef.size(); ++i) {
    y += pow(x, coef.size() - 1 - i) * coef[i];
  }
  return y;
}

int main() {
  // test
  signalsmith::plot::Plot2D plot;
  
  std::vector<double> x {-2, -1, 0, 1, 2};
  std::vector<double> y {0.9, 1.4, 1.1, 0.4, -1.2};

  int scale = 5;

  plot.x.major(0).minors(-scale, scale).label("x");
  plot.y.major(0).minors(-scale, scale).label("y");

  auto &line = plot.line().fillToY(0);
  line.label("initial points");

  for (int i = 0; i < x.size(); ++i) {
    line.marker(x[i], y[i]);
  }

  for (int i = 1; i <= 4; ++i) {
    std::vector<double> coef = approximation(i, x, y);

		auto &graph = plot.line();
		for (double p = -scale; p < scale; p += 0.1) {
			graph.add(p, function(coef, p));
		}

    graph.label(std::to_string(i) + "-th power");
  }

	plot.write("plot.svg");
}