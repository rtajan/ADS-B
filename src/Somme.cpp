#include "Somme.hpp"

Somme::Somme(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {

    const std::string name = "Somme";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<double>(p, "output", 1);   // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& somme = static_cast<Somme&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          somme.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Somme::process(const double* input, double* output) {
    for (int i = 0; i < n_elmts; ++i) {
        output[0] += input[i];
    }
}