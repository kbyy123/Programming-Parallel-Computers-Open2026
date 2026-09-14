/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- the correlation between rows i and j has to be stored in result[i + j*ny]
- only elements with 0 <= j <= i < ny need to be filled
*/
#include <cmath>
#include <vector>

void correlate(int ny, int nx, const float *data, float *result) {
    std::vector<double> sum(ny);
    std::vector<double> square_sum(ny);

    std::vector<double> data_v(ny * nx);

    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            double d = data[i * nx + j];
            sum[i] += d;
            square_sum[i] += d * d;
        }
    }

    for (int i = 0; i < ny; i++) {
        double mean = sum[i] / nx;
        double inv_norm = 1.0 / std::sqrt(square_sum[i] - sum[i] * sum[i] / nx);
        for (int j = 0; j < nx; j++) {
            data_v[i * nx + j] = (data[i * nx + j] - mean) * inv_norm;
        }   
    }

    for (int i = 0; i < ny; i++) {
        for (int j = i; j < ny; j++) {
            double dot_sum = 0;
            for (int k = 0; k < nx; k++) {
                dot_sum += data_v[i * nx + k] * data_v[j * nx + k];
            }
            result[i * ny + j] = (float)dot_sum;
        }
    }
}
