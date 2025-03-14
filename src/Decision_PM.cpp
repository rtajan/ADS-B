#include "Decision_PM.hpp"


DecisionPM::DecisionPM(const int n_elmts, double v0)
    : Stateful()
    , n_elmts(n_elmts), v0(v0) {

    const std::string name = "DecisionPM";
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
          auto& decisionPM = static_cast<DecisionPM&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          decisionPM.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });
}

void DecisionPM::process(const double* input, double* output) {
    for (size_t i = 0; i < n_elmts / 2; ++i) {
        double point[2] = {input[i * 2], input[i * 2 + 1]};
        double compare_l[2] = {point[0], point[1]-v0};
        double compare_r[2] = {point[0]-v0, point[1]};

        if (dist_eucl(compare_l) > dist_eucl(compare_r)) {
            output[i] = 1;
        } else {
            output[i] = 0;
        }
    }
}

double DecisionPM::dist_eucl(const double* vec) {
    return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}