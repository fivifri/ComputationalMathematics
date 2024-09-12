#include <vector>
#include <iostream>
#include <cmath>

// it's a sketch without checks for correctness of input, convergence of the method and uniqueness of the solution
// a matrix must be of size n*(n+1) where the last column is the column of the constant terms

// Jacobian method for solving systems of linear equations
std::vector<double> JacobianMethod(std::vector<std::vector<double>> matrix, double eps) { 
  int N = matrix.size();
  std::vector<double> x_old(N);
  std::vector<double> x(N);
  double max_dif = 2 * eps;

  for (int i = 0; i < N; ++i) {
    x_old[i] = matrix[i][N];
  }

  while (max_dif > eps) {
    for (int i = 0; i < N; ++i) {
      x[i] = matrix[i][N];

      for (int j = 0; j < N; ++j) {
        if (j != i) {
          x[i] -= matrix[i][j] * x_old[j];
        }
      }

      x[i] /= matrix[i][i];

      if (i == 0) {
        max_dif = std::abs(x_old[0] - x[0]);
      }

      max_dif = std::max(max_dif, std::abs(x_old[i] - x[i]));
    }

    std::swap(x, x_old);
  }

  return x_old;
}

int main() {
  // test
  std::vector<std::vector<double>> matrix = {{1, -0.22, 0.11, -0.31, 2.7}, 
                                             {-0.38, 1, 0.12, -0.22, -1.5},
                                             {-0.11, -0.23, 1, 0.51, 1.2},
                                             {-0.17, 0.21, -0.31, 1, -0.17}};

  std::vector<double> ans = JacobianMethod(matrix, 0.001);

  for (double x : ans) {
    std::cout << x << ' ';
  }
}