#include "Filter.hpp"


Filter::Filter(const int n_elmts, double* h, const int size_h)
    : Stateful()
    , h(h), size_h(size_h), n_elmts(n_elmts) {

    const std::string name = "Filter";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<double>(p, "output", size_h + this->n_elmts - 1);   // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& filter = static_cast<Filter&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          filter.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Filter::process(const double* input, double* output) {
    for (size_t n = 0; n < n_elmts; ++n) {
        for (size_t k = 0; k < size_h; ++k) {
            output[n + k] += input[n] * h[k];
        }
    }
}