#include "Emetteur_PM.hpp"
#include <iostream>

EmetteurPM::EmetteurPM(const int n_elmts, double Fe, double Ts)
    : Stateful()
    , n_elmts(n_elmts), Fe(Fe), Ts(Ts) {
    Fse = static_cast<int>(Fe * Ts);
    len_p = Fse;

    //std::vector<std::vector<double>> porte(2, std::vector<double>(len_p, 0));
    porte = new double*[2];
    porte[0] = new double[len_p];
    porte[1] = new double[len_p];

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

void EmetteurPM::process(const double* input, double* output) {
    for (int i = 0; i < n_elmts; ++i) {
        // Assurez-vous que input[i] est un index valide pour porte
        int index = static_cast<int>(input[i]);

        // Utilisation correcte de std::copy avec des pointeurs
        std::copy(porte[index], porte[index]+len_p, output + i * len_p);
    }
}
