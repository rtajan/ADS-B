#include "Recepteur_PM.hpp"
#include <iostream>
#include <cmath>
#include <numeric> // Pour std::accumulate

RecepteurPM::RecepteurPM(const int n_elmts, double Fe, double Ts)
    : Stateful()
    , Fe(Fe), Ts(Ts), n_elmts(n_elmts) {
    Fse = static_cast<int>(Fe * Ts);
    len_p = Fse;

    std::vector<std::vector<double>> porte(2, std::vector<double>(len_p, 0));
    for (int i = 0; i < len_p / 2; ++i) {
        porte[0][i] = 0;
        porte[0][i + len_p / 2] = 1;
        porte[1][i] = 1;
        porte[1][i + len_p / 2] = 0;
    }

    //porte = {
    //    std::vector<double>(len_p / 2, 0.0),
    //    std::vector<double>(len_p / 2, 1.0),
    //    std::vector<double>(len_p / 2, 1.0),
    //    std::vector<double>(len_p / 2, 0.0)
    //};

    const std::string name = "RecepteurPM";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<double>(p, "output", ((2*Fse + this->n_elmts) - 1) / Fse); // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& recepteurPM = static_cast<RecepteurPM&>(m);
          double* yl = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          recepteurPM.process(yl, output);
          return spu::runtime::status_t::SUCCESS;
      });


}

void RecepteurPM::process(const double* yl, double* output) {
    std::vector<double> rl = convolution(yl, porte[0]);
    std::vector<double> rk;
    for (size_t i = len_p; i < rl.size(); i += len_p / 2) {
        rk.push_back(rl[i]);
    }

    double v0 = std::accumulate(porte[1].begin(), porte[1].end(), 0.0,
                                [](double acc, double val) { return acc + val * val; });

    std::vector<double> output(rk.size() / 2, 0);  //problem for the length of rk (we don't know it)
    for (size_t i = 0; i < rk.size() / 2; ++i) {
        std::vector<double> point = {rk[i * 2], rk[i * 2 + 1]};
        if (dist_eucl(point) > dist_eucl(point)) {
            output[i] = 1;
        } else {
            output[i] = 0;
        }
    }

    #return sortie;
}

double* RecepteurPM::convolution(const double* signal, const double* filtre) {
    std::vector<double> result(signal.size() + filtre.size() - 1, 0.0);
    for (size_t n = 0; n < signal.size(); ++n) {
        for (size_t k = 0; k < filtre.size(); ++k) {
            result[n + k] += signal[n] * filtre[k];
        }
    }
    return result;
}

double RecepteurPM::dist_eucl(const double* vec) {
    return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}
