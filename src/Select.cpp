#include "Select.hpp"


Select::Select(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {

    const std::string name = "Select";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_num = this->template create_socket_in<double>(p, "num1", this->n_elmts);    // Create the input socket
    size_t ps_denum = this->template create_socket_in<double>(p, "denum1", this->n_elmts);
    size_t ps_decalage = this->template create_socket_out<int>(p, "decalage", 1);  // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_num, ps_denum, ps_decalage](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& select = static_cast<Select&>(m);

          double* num = (double*)(t[ps_num].get_dataptr());
          double* denum = (double*)(t[ps_denum].get_dataptr());
          int* decalage = (int*)(t[ps_decalage].get_dataptr());

          // Process the data
          select.process(num,denum,decalage);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Select::process(const double* num, const double* denum, int* decalage) {
    double intercorr[n_elmts];
    for (int i=0; i<n_elmts; i++){
        intercorr[i]=num[i]/denum[i];
    }
    double* p_max = std::max_element(intercorr, intercorr + n_elmts);
    decalage[0] = (p_max - intercorr);
}