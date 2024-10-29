#include <iostream>
#include <vector>
#include <random>
#include <vector>
#include <algorithm>
#include "CSimple_timer.hpp"
#include <omp.h>

void grid_maker(std::vector<double>& random_grid, double x0, double x1){
  std::default_random_engine engine(std::random_device{}());
  std::uniform_real_distribution<double> unif(x0, x1);
  auto random_numbers = [&]() { return unif(engine); };
  std::generate(std::begin(random_grid), std::end(random_grid),
                  random_numbers);
  random_grid[0] = x0;
  random_grid[random_grid.size() - 1] = x1;
  std::sort(random_grid.begin(), random_grid.end());

}//grid_maker

void matmul(const std::vector<double>& A, const std::vector<double>& B, std::vector<double>& C, const int N){
    {
        CSimple_timer t{"matmul"};
        int i,j;
        //collaps(2) is okay because the 
        #pragma omp parallel for collapse(2) private(i ,j) //reduction(+: C)
            for (int i = 0; i < N ; i++) {
                for (int j = 0; j < N; j++) {
                    for (int k = 0; k < N; ++k) {
                        C[i * N + j] += A[i * N + k] * B[k * N + j];
                    }
                }
            }
    }

    
}//matmul

int main(int argc, char** argv){
    if (argc != 2){
        std::cout << "Usage: input the rows of the matrix as a command line argument." << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    double x0{1.0};
    double x1{100.0};
    std::vector<double> A(N*N);
    std::vector<double> B(N*N);
    std::vector<double> C(N*N);
    grid_maker(A, x0, x1);
    grid_maker(B, x0, x1);

    matmul(A, B, C, N);

    CSimple_timer::print_timing_results();

}//main
