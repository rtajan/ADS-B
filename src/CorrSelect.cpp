#include "CorrSelect.hpp"


CorrSelect::CorrSelect(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {

    const std::string name = "CorrSelect";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_num1 = this->template create_socket_in<double>(p, "num1", this->n_elmts);    // Create the input socket
    size_t ps_num2 = this->template create_socket_in<double>(p, "num2", this->n_elmts);

    size_t ps_denum1 = this->template create_socket_in<double>(p, "denum1", this->n_elmts);
    size_t ps_denum2 = this->template create_socket_int<double>(p, "denum2", this->n_elmts);

    size_t ps_intercorr = this->template create_socket_out<double>(p, "intercorr", this->n_elmts);  // Create the output socket
    size_t ps_indice = this->template create_socket_out<int>(p, "indice", 1);

    // create the codelet
    this->create_codelet(
      p,
      [ps_num1, ps_num2, ps_denum1, ps_denum2, ps_intercorr, ps_indice](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& corrSelect = static_cast<CorrSelect&>(m);

          double* num1 = (double*)(t[ps_num1].get_dataptr());
          double* num2 = (double*)(t[ps_num2].get_dataptr());
          double* denum1 = (double*)(t[ps_denum1].get_dataptr());
          double* denum2 = (double*)(t[ps_denum2].get_dataptr());

          double* intercorr = (double*)(t[ps_intercorr].get_dataptr());
          int* indice = (int*)(t[ps_indice].get_dataptr());

          // Process the data
          corrSelect.process(num1, num2, denum1, denum2, intercorr, indice);
          return spu::runtime::status_t::SUCCESS;
      });
}

void CorrSelect::process(const double* num1, const double* num2, const double* denum1, const double* denum2, double* intercorr, int* indice) {
    double* voie2 = new double[n_elmts];

    for (int i=0; i<n_elmts; i++){
        intercorr[i]=num1[i]/denum1[i];
        voie2[i]=num2[i]/denum2[i];
    }
    bool voie_max = *std::max_element(voie2, voie2 + n_elmts) > *std::max_element(intercorr, intercorr + n_elmts);

    if (voie_max){
        for (int i=0; i<n_elmts; i++){
            intercorr[i]=voie2[i];
        }
    }
    indice[0] = (int)voie_max;
}