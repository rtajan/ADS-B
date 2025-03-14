#include "Decimation_PM.hpp"


DecimationPM::DecimationPM(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts) {


    const std::string name = "DecimationPM";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket

    size_t ps_voie1 = this->template create_socket_out<double>(p, "voie1", this->n_elmts /10); // Create the output socket
    size_t ps_voie2 = this->template create_socket_out<double>(p, "voie2", this->n_elmts /10);
    size_t ps_voie3 = this->template create_socket_out<double>(p, "voie3", this->n_elmts /10);
    size_t ps_voie4 = this->template create_socket_out<double>(p, "voie4", this->n_elmts /10);
    size_t ps_voie5 = this->template create_socket_out<double>(p, "voie5", this->n_elmts /10);
    size_t ps_voie6 = this->template create_socket_out<double>(p, "voie6", this->n_elmts /10);
    size_t ps_voie7 = this->template create_socket_out<double>(p, "voie7", this->n_elmts /10);
    size_t ps_voie8 = this->template create_socket_out<double>(p, "voie8", this->n_elmts /10);
    size_t ps_voie9 = this->template create_socket_out<double>(p, "voie9", this->n_elmts /10);
    size_t ps_voie10 = this->template create_socket_out<double>(p, "voie10", this->n_elmts /10);

    // create the codelet
    this->create_codelet(
      p,
      [ps_input,
        ps_voie1,
        ps_voie2,
        ps_voie3,
        ps_voie4,
        ps_voie5,
        ps_voie6,
        ps_voie7,
        ps_voie8,
        ps_voie9,
        ps_voie10
      ](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& decimationPM = static_cast<DecimationPM&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());

          double* voie1 = (double*)(t[ps_voie1].get_dataptr());
          double* voie2 = (double*)(t[ps_voie2].get_dataptr());
          double* voie3 = (double*)(t[ps_voie3].get_dataptr());
          double* voie4 = (double*)(t[ps_voie4].get_dataptr());
          double* voie5 = (double*)(t[ps_voie5].get_dataptr());
          double* voie6 = (double*)(t[ps_voie6].get_dataptr());
          double* voie7 = (double*)(t[ps_voie7].get_dataptr());
          double* voie8 = (double*)(t[ps_voie8].get_dataptr());
          double* voie9 = (double*)(t[ps_voie9].get_dataptr());
          double* voie10 = (double*)(t[ps_voie10].get_dataptr());

          // Process the data
          decimationPM.process(input,
            voie1,
            voie2,
            voie3,
            voie4,
            voie5,
            voie6,
            voie7,
            voie8,
            voie9,
            voie10);
          return spu::runtime::status_t::SUCCESS;
      });

}

void DecimationPM::process(const double* input,
    double* voie1,
    double* voie2,
    double* voie3,
    double* voie4,
    double* voie5,
    double* voie6,
    double* voie7,
    double* voie8,
    double* voie9,
    double* voie10) {

    double* signaux[]={voie1,voie2,voie3,voie4,voie5,voie6,voie7,voie8,voie9,voie10};

    for (int i = 0; i < (n_elmts /10); ++i) {
        signaux[i % 10][i / 10] = input[i];
    }
}
