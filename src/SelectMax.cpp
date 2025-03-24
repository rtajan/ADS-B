#include "SelectMax.hpp"


SelectMax::SelectMax(const int n_elmts, const std::vector<double>& init)
    : Stateful()
    , init(n_elmts,0.0)
    , n_elmts(n_elmts) {

    this->indice_state=-1;

    for (size_t i = 0; i < n_elmts; i++)
		this->init[i] = init[i];

    const std::string name = "SelectMax";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_intercorr = this->template create_socket_in<double>(p, "intercorr", this->n_elmts);    // Create the input socket
    size_t ps_indice_in = this->template create_socket_in<int>(p, "indice_in", 1);

    size_t ps_max = this->template create_socket_out<double>(p, "max", this->n_elmts);  // Create the output socket
    size_t ps_indice_out = this->template create_socket_out<int>(p, "indice_out", 1);

    // create the codelet
    this->create_codelet(
      p,
      [ps_intercorr, ps_indice_in, ps_max, ps_indice_out](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& selectMax = static_cast<SelectMax&>(m);

          double* intercorr = (double*)(t[ps_intercorr].get_dataptr());
          int* indice_in = (int*)(t[ps_indice_in].get_dataptr());

          double* max = (double*)(t[ps_max].get_dataptr());
          int* indice_out = (int*)(t[ps_indice_out].get_dataptr());

          // Process the data
          selectMax.process(intercorr, indice_in, max, indice_out);
          return spu::runtime::status_t::SUCCESS;
      });
}

void SelectMax::process(const double* intercorr, int* indice_in, double* max, int* indice_out) {
    bool voie_max = *std::max_element(intercorr, intercorr + n_elmts) > *std::max_element(this->init.begin(), this->init.end());

    if (voie_max){
        for (int i=0; i<n_elmts; i++){
            init[i]=intercorr[i];
            max[i]=intercorr[i]; //remplacer par une copie de la première adresse ?
        }
        indice_out[0]=indice_in[0];
    } else {
        for (int i=0; i<n_elmts; i++){
            max[i]=init[i];
        }
        this->indice_state=indice_in[0];
    }

}