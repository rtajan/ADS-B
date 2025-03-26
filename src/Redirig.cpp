#include "Redirig.hpp"


Redirig::Redirig(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts)
     {


    const std::string name = "Redirig";
    this->set_name(name);

    auto& p = this->create_task("process");
    size_t ps_tram = this->template create_socket_in<double>(p, "tram", 112);

    size_t ps_adresse = this->template create_socket_out<int8_t>(p, "adresse", 6*sizeof(int8_t));
    size_t ps_indic = this->template create_socket_out<int>(p,"indic",1);

    // create the codelet
    this->create_codelet(
      p,
      [ps_tram, ps_adresse, ps_indic](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& redirig = static_cast<Redirig&>(m);
          double* tram = (double*)(t[ps_tram].get_dataptr());
          char* adresse = (char*)(t[ps_adresse].get_dataptr());
          int* indic = (int*)(t[ps_indic].get_dataptr());

          // Process the data
          redirig.process(tram, adresse,indic);
          return spu::runtime::status_t::SUCCESS;
      });
}

void Redirig::process(const double* tram, char adresse[16], int* indic) {
    std::vector<double> vectbin(tram, tram + 112);

    auto formatBits = getRange(vectbin, 0, 5);
    auto adresseBits = getRange(vectbin, 8, 32);
    auto typeBits = getRange(vectbin, 32, 37);

    auto format = bin2dec(formatBits);
    bin2hex(adresseBits, adresse);
    auto type = bin2dec(typeBits);

    if ((type >= 9 && type <= 18) || (type >= 20 && type <= 22)) {
        indic[0]=1; //the data of the tram contains long, lat and altitud
    } else if (type >= 1 && type <= 4) {
        indic[0]=0; //the data of the tram contains the name of the flight
    }

    //for (int i=0; i<6; i++){
    //    std::cerr<<"adresse car : "<<adresse[i]<<std::endl;
    //}

}

int Redirig::bin2dec(const std::vector<double>& bits) {
    int value = 0;
    for (double bit : bits) {
        value = (value << 1) | static_cast<int>(bit);
    }
    return value;
}

void Redirig::bin2hex(const std::vector<double>& bits, char* out) {
    int value = bin2dec(bits);
    std::stringstream ss;
    ss << std::hex << std::setw(6) << std::setfill('0') << value;
    auto longu = ss.str().length();
    std::string inter = ss.str();

    for (int i=0; i<longu; i++){
        out[i] = inter[i];
    }
}


std::vector<double> Redirig::getRange(std::vector<double>& vect, int start, int end) {
    return std::vector<double>(vect.begin() + start, vect.begin() + end);
}