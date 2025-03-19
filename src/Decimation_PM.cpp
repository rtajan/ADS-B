#include "Decimation_PM.hpp"


DecimationPM::DecimationPM(const int n_elmts,int isComplex)
    : Stateful()
    , n_elmts(n_elmts)
    , isComplex(isComplex) {


    const std::string name = "DecimationPM";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket

    size_t ps_voie1 = this->template create_socket_out<double>(p, "voie1", this->n_elmts /2); // Create the output socket
    size_t ps_voie2 = this->template create_socket_out<double>(p, "voie2", this->n_elmts /2);

    // create the codelet
    this->create_codelet(
      p,
      [ps_input,
        ps_voie1,
        ps_voie2,
      ](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& decimationPM = static_cast<DecimationPM&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());

          double* voie1 = (double*)(t[ps_voie1].get_dataptr());
          double* voie2 = (double*)(t[ps_voie2].get_dataptr());

          // Process the data
          decimationPM.process(input,
            voie1,
            voie2,);
          return spu::runtime::status_t::SUCCESS;
      });

}

void DecimationPM::process(const double* input,
    double* voie1,
    double* voie2) {

    double* signaux[]={voie1,voie2};

    if (isComplex){
        for (int i=0; i<n_elmts; i++){
          int indice = (i%4)/2;
          signaux[indice][(i+(1-(2*indice)))/2]=input[i]
        }
    }
    else {
      for (int i = 0; i < n_elmts; i++) {
        signaux[i % 2][i / 2] = input[i];
      }
    }
}
