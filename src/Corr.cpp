#include "Corr.hpp"


Corr::Corr(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {

    const std::string name = "Corr";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_num = this->template create_socket_in<double>(p, "num1", this->n_elmts);    // Create the input socket
    size_t ps_denum = this->template create_socket_in<double>(p, "denum1", this->n_elmts);
    size_t ps_intercorr = this->template create_socket_out<double>(p, "intercorr", this->n_elmts);  // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_num, ps_denum, ps_intercorr](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& corr = static_cast<Corr&>(m);

          double* num = (double*)(t[ps_num].get_dataptr());
          double* denum = (double*)(t[ps_denum].get_dataptr());
          double* intercorr = (double*)(t[ps_intercorr].get_dataptr());

          // Process the data
          corr.process(num,denum,intercorr);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Corr::process(const double* num, const double* denum, double* intercorr) {
    for (int i=0; i<n_elmts; i++){
        intercorr[i]=num[i]/denum[i];
    }
}