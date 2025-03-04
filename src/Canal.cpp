#include "Canal.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <numeric> // for std::accumulate

Canal::Canal(const int n_elmts,const double SNR, const int len_p)
    : Stateful()
    ,n_elmts(n_elmts), SNR(SNR), len_p(len_p) {

        const std::string name = "Canal";
        this->set_name(name);

        auto& p = this->create_task("process");                                                  // Create the task
        size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
        size_t ps_output = this->template create_socket_out<double>(p, "output", this->n_elmts); // Create the output socket

        // create the codelet
        this->create_codelet(
          p,
          [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
          {
              // Recover the Module and Sockets in the codelet
              auto& canal = static_cast<Canal&>(m);
              double* sl = (double*)(t[ps_input].get_dataptr());
              double* output = (double*)(t[ps_output].get_dataptr());

              // Process the data
              canal.process(sl, output);
              return spu::runtime::status_t::SUCCESS;
          });


}

void Canal::process(const double * sl, double * output) {
    double Eb = calculateEb(sl);
    std::cout << "Eb " << Eb << std::endl;
    double * noise = generateNoise();

    double sigma = std::sqrt(len_p * (Eb / SNR) / 2);
    std::cout << "sigma  " << sigma << std::endl;

    //std::vector<double> yl(sl.size());
    for (size_t i = 0; i < n_elmts; ++i) {
        output[i] = sl[i] + noise[i] * sigma;
    }


    //return yl;
}

double Canal::calculateEb(const double * sl) {
    double sum = std::accumulate(sl, std::next(sl, n_elmts), 0.0, [](double acc, double val) {
        return acc + val * val;
    });
    return sum / n_elmts;
}

double * Canal::generateNoise() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);

    //std::vector<double> noise(len_p * n_elmts);
    double * noise = new double[n_elmts];
    for (int i = 0; i < n_elmts; ++i) {
        noise[i] = d(gen);
    }
    return noise;
}
