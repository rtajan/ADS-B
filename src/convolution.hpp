#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <vector>

// Effectue la convolution entre deux signaux
std::vector<double> convolve(const std::vector<double>& signal, const std::vector<double>& kernel);

#endif // CONVOLUTION_H
