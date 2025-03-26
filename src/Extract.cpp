#include "Extract.hpp"


Extract::Extract(const int n_elmts, const int Fse, const double seuil)
    : Stateful()
    , n_elmts(n_elmts)
    , Fse(Fse)
    , seuil(seuil)
     {

    buffer=new double[224];
    size_buffer=0;
    voie=-1;

    const std::string name = "Extract";
    this->set_name(name);

    auto& p = this->create_task("process");
    size_t ps_decal_max = this->template create_socket_in<int>(p, "decal_max", 2);
    size_t ps_sigs = this->template create_socket_in<double>(p, "sigs", this->n_elmts);

    size_t ps_tram = this->template create_socket_out<double>(p, "tram", 224);

    // create the codelet
    this->create_codelet(
      p,
      [ps_decal_max, ps_sigs, ps_tram](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& extract = static_cast<Extract&>(m);
          int* decal_max = (int*)(t[ps_decal_max].get_dataptr());
          double* sigs = (double*)(t[ps_sigs].get_dataptr());
          double* tram = (double*)(t[ps_tram].get_dataptr());


          // Process the data
          extract.process(decal_max, sigs, tram);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Extract::process(const int* decal_max, double* sigs, double* tram) {
    int voie_sig =  decal_max[0] % (this->Fse /2);

    if (size_buffer > 0 and voie != -1){
        for (int j=0; j< size_buffer; j++){
            tram[j]=buffer[j];
        }

        for (int i=0; i <(224-size_buffer); i++){
            tram[size_buffer+i]=sigs[i*(Fse/2)+voie];
        }
    }

    if (decal_max[1] > seuil){
        if(decal_max[0]<8){
            for (int t=0; t< 224; t++){
                tram[t]=sigs[decal_max[0] + (t*(Fse/2)+voie_sig)];
            }
        } else {
            size_buffer = 224-decal_max[0];
            voie = voie_sig;
            for (int k=0; k< size_buffer; k++){
                buffer[k]=sigs[decal_max[0] + (k*(Fse/2)+voie_sig)];
            }
        }
    } else{
        size_buffer=0;
        voie=-1;
        throw spu::tools::processing_aborted(); // a verfier
    }


}