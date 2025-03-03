#include "Emetteur_pm.hpp"
#include <iostream>

EmetteurPM::EmetteurPM(double Fe, double Ts)
    : Stateful()
    , Fe(Fe), Ts(Ts) {
    Fse = static_cast<int>(Fe * Ts);
    len_p = Fse;

    std::vector<std::vector<double>> porte(2, std::vector<double>(len_p, 0));
    for (int i = 0; i < len_p / 2; ++i) {
        porte[0][i] = 0;
        porte[0][i + len_p / 2] = 1;
        porte[1][i] = 1;
        porte[1][i + len_p / 2] = 0;
    }

    const std::string name = "EmetteurPM";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<double>(p, "output", this->n_elmts * len_p); // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& emmeteurPM = static_cast<EmetteurPM&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          emmeteurPM.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });

}

void EmetteurPM::process(const double * input, double * output) {
    int len_input = input.size();
    //std::vector<double> sl(len_input * len_p, 0.0);

    for (int i = 0; i < len_input; ++i) {
        std::copy(porte[input[i]].begin(), porte[input[i]].end(), output.begin() + i * len_p);
    }

    //return sl;
}
