#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>

// Jacobian method for solving systems of linear equations
class JacobianMethod 
{
private:
  static bool CheckInput(const std::vector<std::vector<double>>& matrix) {
    if (matrix.size() == 0) {
      return false;
    }

    if (matrix.size() + 1 != matrix[0].size()) {
      return false;
    }

    return true;
  }

  static bool MakeNonZeroDiag(std::vector<std::vector<double>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
      if (matrix[i][i] == 0) {
        int id = -1;

        for (int j = 0; j < matrix.size(); ++j) {
          if (matrix[j][i] != 0) {
            id = j;
            break;
          }
        }

        if (id == -1) {
          return false;
        }

        for (int j = 0; j < matrix.size(); ++j) {
          matrix[i][j] += matrix[id][j];
        }
      }
    }

    return true;
  }

  static std::vector<double> Solver(const std::vector<std::vector<double>> &matrix, double eps, int limit) {
    int N = matrix.size();

    std::vector<double> x_old(N);
    std::vector<double> x(N);
    double max_dif = 2 * eps;

    for (int i = 0; i < N; ++i) {
      x_old[i] = matrix[i][N];
    }

    while (max_dif > eps) {
      if (--limit == 0) {
        return {};
      }

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

public:
  static std::vector<double> Solve(std::vector<std::vector<double>> matrix, double eps, int limit) {
    if (!CheckInput(matrix)) {
      throw std::invalid_argument("A matrix must be of size n*(n+1) where the last column is the column of the constant terms and n != 0.");
    }

    if (!MakeNonZeroDiag(matrix)) {
      throw std::invalid_argument("The matrix contains a zero column. Jacobi's method is not applicable.");
    }

    std::vector<double> ans = Solver(matrix, eps, limit);

    for (double x : ans) {
      if (std::isnan(x) || std::isinf(x)) {
        throw std::invalid_argument("The method didn't converge :(\nTry to convert the matrix to a diagonally dominant form."); 
      }
    }

    if (ans.empty()) {
      throw std::invalid_argument("Iteration limit exceeded."); 
    }

    return ans;
  }
};

int main() {
  // test
  std::vector<std::vector<double>> matrix = {{1, -0.22, 0.11, -0.31, 2.7}, 
                                             {-0.38, 1, 0.12, -0.22, -1.5},
                                             {-0.11, -0.23, 1, 0.51, 1.2},
                                             {-0.17, 0.21, -0.31, 1, -0.17}};

  std::vector<double> ans = JacobianMethod::Solve(matrix, 0.001, 10000);

  for (double x : ans) {
    std::cout << x << ' ';
  }
}