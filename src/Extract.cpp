#include "Extract.hpp"


Extract::Extract(const int n_elmts, const int rows, const int cols, const double seuil)
    : Stateful()
    , n_elmts(n_elmts)
    , rows(rows)
    , cols(cols)
    , seuil(seuil)
     {

    buffer=new double[448];
    size_buffer=0;
    voie=-1;

    const std::string name = "Extract";
    this->set_name(name);

    auto& p = this->create_task("process");
    size_t ps_intercorr = this->template create_socket_in<double>(p, "intercorr", this->n_elmts);
    size_t ps_indice = this->template create_socket_in<int>(p, "indice", 1);

    size_t ps_sigs = this->template create_2d_socket_in<double>(p, "sigs", this->rows,this->cols);

    size_t ps_tram = this->template create_socket_out<double>(p, "tram", 448);

    // create the codelet
    this->create_codelet(
      p,
      [ps_intercorr, ps_indice, ps_sigs, ps_tram](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& extract = static_cast<Extract&>(m);

          double* intercorr = (double*)(t[ps_intercorr].get_dataptr());
          int* indice = (int*)(t[ps_indice].get_dataptr());

          double** sigs = (double**)(t[ps_sigs].get_dataptr());

          double* tram = (double*)(t[ps_tram].get_dataptr());


          // Process the data
          extract.process(intercorr, indice, sigs, tram);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Extract::process(const double* intercorr, const int* indice, const double** sigs, double* tram) {
    double* p_max = std::max_element(intercorr, intercorr + n_elmts);

    if (size_buffer > 0 and voie != -1){
        for (int j=0; j< size_buffer; j++){
            tram[j]=buffer[j];
        }

        for (int i=0; i <(448-size_buffer); i++){
            tram[size_buffer+i]=sigs[voie][i];
        }
    }

    if (*p_max > seuil){
        if((p_max - intercorr)<8){  //arithmetique des pointeurs, a verifier
            for (int t=0; t< 448; t++){
                tram[t]=sigs[indice][t+(p_max-intercorr)];
            }
        } else {
            size_buffer = 448-(p_max-intercorr);
            voie = indice;
            for (int k=0; k< size_buffer; k++){
                buffer[k]=sigs[indice][t+(p_max-intercorr)];
            }
        }
    } else{
        size_buffer=0;
        voie=-1;
    }


}