#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "plot.h"
#include "lab1.cpp"

using namespace std;

// Piecewise Polynomial Interpolation
class PiecewiseInterpolation {
private:
  vector<vector<double>> coefficients;
  vector<pair<double, double>> xy;

  void CreateXY(vector<double>& x, vector<double>& y, vector<pair<double, double>>& xy) {
    for (int i = 0; i < x.size(); ++i) {
      xy.push_back({x[i], y[i]});
    }

    sort(xy.begin(), xy.end());   
  }

  void CheckInput(vector<double>& x, vector<double>& y, int k) {
    if (x.size() != y.size()) {
      throw invalid_argument("the dimensions of vectors x and y must match.");
    }

    if (x.size() < k + 1) {
      throw invalid_argument("the vector size must be no less than k+1.");
    }
  }

  void FillInMatrix(vector<pair<double, double>>& xy, int start, int k, vector<vector<double>>& matrix) {
    for (int i = start; i <= start + k; ++i) {
      matrix[i - start][k + 1] = xy[i].second;
      for (int j = 0; j <= k; ++j) {
        matrix[i - start][j] = pow(xy[i].first, j);
      }
    }
  }

  void FindCoefficients(vector<pair<double, double>>& xy, int k) {
    vector<vector<double>> matrix(k + 1, vector<double>(k + 2));
    for (int i = 0; i + k < xy.size(); i += k) {
      FillInMatrix(xy, i, k, matrix);
      coefficients.push_back({});
      coefficients.back() = IterativeMethods::Solve(matrix, 0.001, 10000, "Seidel");
      coefficients.back().push_back(xy[i].first);
      coefficients.back().push_back(xy[i + k].first);
    }
  }

  double Value(vector<double>& interval, double x) {
    double result = 0;

    for (int i = 0; i < interval.size() - 2; ++i) {
      result += interval[i] * pow(x, i);
    }

    return result;
  }

public:
  // x and y are interpolation nodes, k is the degree of the polynomial
  PiecewiseInterpolation(vector<double>& x, vector<double>& y, int k) {
    CheckInput(x, y, k);
    xy = {};
    CreateXY(x, y, xy);
    coefficients = {};
    FindCoefficients(xy, k);
  }

  /* i-th element of the vector is the vector of coefficients of the polynomial 
     on the i-th interval, where the penultimate element is the beginning 
     of the interval, the last element is the end of the interval */
  vector<vector<double>> GetCoefficients() {
    return coefficients;
  }

  // saves svg plot with the given name
  void Plot(string file_name) {
    signalsmith::plot::Plot2D plot;
    int scale = 4;

    plot.x.major(0).minors(-scale, scale).label("x");
    plot.y.major(0).minors(-scale, scale).label("y");

    auto &line = plot.line().fillToY(0);
    line.label("initial points");
    auto &graph = plot.line();

    for (int i = 0; i < xy.size(); ++i) {
      line.marker(xy[i].first, xy[i].second);
    }

    for (vector<double>& interval : coefficients) {
      for (double p = interval[interval.size() - 2]; p <= interval.back(); p += 0.1) {
        graph.add(p, Value(interval, p));
      }
    }

    graph.label(std::to_string(coefficients[0].size() - 3) + "-th power");
    plot.write(file_name + ".svg");
  }

  double FindValue(double x) {
    for (vector<double>& interval : coefficients) {
      if (x >= interval[interval.size() - 2] && x <= interval.back()) {
        return Value(interval, x);
      }
    }

    return 0;
  }
};


int main() {
  // test

  vector<double> x {1, 1.2, 1.5, 2.1, 2.3, 2.9, 3.1, 3.4, 4.0};
  vector<double> y = {-12, -10, -9.8, -7.9, -6.2, -5.7, -4.5, -3.1, -4.2};
  PiecewiseInterpolation test = PiecewiseInterpolation(x, y, 2);
  test.Plot("test_lab_5");
  cout << test.FindValue(3.3) << ' ' << test.FindValue(1.7) << '\n';
}