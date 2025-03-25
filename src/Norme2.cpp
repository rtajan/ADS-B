#include "Norme2.hpp"
#include <cmath>

Norme2::Norme2(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {

    const std::string name = "Norme2";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<double>(p, "output", this->n_elmts /2);   // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& norme2 = static_cast<Norme2&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          norme2.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Norme2::process(const double* input, double* output) {
    for (size_t i = 0; i < n_elmts/2; ++i) {
        output[i]=std::sqrt((input[i*2]*input[i*2])+(input[i*2 +1]*input[i*2 +1]));
    }
}