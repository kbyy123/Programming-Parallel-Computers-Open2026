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
    constexpr int nb = 4;
    int na = (nx + nb - 1) / nb;
    int nab =  na * nb;


    std::vector<double> sum(ny);
    std::vector<double> square_sum(ny);
    // 把数组一行变为 4 的倍数，方便并行
    std::vector<double> data_v(ny * nab, 0.0);

    for (int i = 0; i < ny; i++) {
        double sv[nb] = {0}, ssv[nb] = {0};
        for (int ka = 0; ka < na; ka++) {
            for (int kb = 0; kb < nb; kb++) {
                int x = ka * nb + kb;
                if (x < nx) {
                    double d = data[i * nx + x];
                    sv[kb] += d;
                    ssv[kb] += d * d;
                }
            }
        }
        for (int kb = 0; kb < nb; kb++) {
            sum[i] += sv[kb];
            square_sum[i] += ssv[kb];
        }
    }

    for (int i = 0; i < ny; i++) {
        double mean = sum[i] / nx;
        double inv_norm = 1.0 / std::sqrt(square_sum[i] - sum[i] * sum[i] / nx);
        for (int j = 0; j < nx; j++) {
            data_v[i * nab + j] = (data[i * nx + j] - mean) * inv_norm;
        }   
    }

    for (int i = 0; i < ny; i++) {
        for (int j = i; j < ny; j++) {
            double sv[nb] = {0}, v = 0;
            for (int ka = 0; ka < na; ka++) {
                for (int kb = 0; kb < nb; kb++) {
                    int x = ka * nb + kb;
                    sv[kb] += data_v[i * nab + x] * data_v[j * nab + x];
                }
            }
            for (int kb = 0; kb < nb; kb++) {
                v += sv[kb];
            }
            result[i * ny + j] = v;
        }
    }
}
