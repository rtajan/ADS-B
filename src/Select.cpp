#include "Select.hpp"


Select::Select(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {

    const std::string name = "Select";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_num = this->template create_socket_in<double>(p, "num1", this->n_elmts);    // Create the input socket
    size_t ps_denum = this->template create_socket_in<double>(p, "denum1", this->n_elmts);
    size_t ps_decal_max = this->template create_socket_out<int>(p, "decal_max", 2);  // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_num, ps_denum, ps_decal_max](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& select = static_cast<Select&>(m);

          double* num = (double*)(t[ps_num].get_dataptr());
          double* denum = (double*)(t[ps_denum].get_dataptr());
          int* decal_max = (int*)(t[ps_decal_max].get_dataptr());

          // Process the data
          select.process(num,denum,decal_max);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Select::process(const double* num, const double* denum, int* decal_max) {
    double intercorr[n_elmts];
    for (int i=0; i<n_elmts; i++){
        intercorr[i]=num[i]/denum[i];
    }
    double* p_max = std::max_element(intercorr, intercorr + n_elmts);
    decal_max[0] = (p_max - intercorr);
    decal_max[1] = *p_max;
}