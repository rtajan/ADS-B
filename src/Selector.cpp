#include "Selector.hpp"

Selector::Selector(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {

    const std::string name = "Selector";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_2d_socket_in<double>(p, "input",2,this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<int>(p, "output", 1);   // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& selector = static_cast<Selector&>(m);
          double** input = (double**)(t[ps_input].get_dataptr());
          int* output = (int*)(t[ps_output].get_dataptr());

          // Process the data
          selector.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Selector::process(double* input[], int* output) {
    double max = input[0][0]/input[1][0];
    int indice = 0;
    for (int i=1; i < n_elmts; i++){
        if (max < (input[0][i]/input[1][i])){
            max = input[0][i]/input[1][i];
            indice = i;
        }
    }
    output[0]=indice;
}