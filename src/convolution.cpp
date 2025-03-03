#include "convolution.h"
#include <vector>
#include <stdexcept>

std::vector<double> convolve(const std::vector<double>& signal, const std::vector<double>& kernel) {
    // Vérifie que les entrées ne sont pas vides
    if (signal.empty() || kernel.empty()) {
        throw std::invalid_argument("Les signaux d'entrée ne doivent pas être vides.");
    }

    // Taille du signal de sortie
    size_t n = signal.size() + kernel.size() - 1;
    std::vector<double> result(n, 0.0);

    // Effectue la convolution
    for (size_t i = 0; i < signal.size(); ++i) {
        for (size_t j = 0; j < kernel.size(); ++j) {
            result[i + j] += signal[i] * kernel[j];
        }
    }

    return result;
}
