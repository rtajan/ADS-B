#include "DecodNom.hpp"


DecodNom::DecodNom(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts)
     {


    const std::string name = "DecodNom";
    this->set_name(name);

    auto& p = this->create_task("process");
    size_t ps_tram = this->template create_socket_in<double>(p, "tram", 112);
    size_t ps_nom = this->template create_socket_out<int8_t>(p, "nom", 6*sizeof(int8_t));


    // create the codelet
    this->create_codelet(
      p,
      [ps_tram, ps_nom](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& decodNom = static_cast<DecodNom&>(m);
          double* tram = (double*)(t[ps_tram].get_dataptr());
          char* nom = (char*)(t[ps_nom].get_dataptr());

          // Process the data
          decodNom.process(tram, nom);
          return spu::runtime::status_t::SUCCESS;
      });
}

void DecodNom::process(const double* tram, char nom[8]) {
    std::vector<double> vectbin(tram, tram + 112);

    for (int i = 0; i < 8; ++i) {
        auto charBits = getRange(vectbin, 40 + i * 6, 46 + i * 6);
        nom[i] = bin2carid(charBits);
    }
}

int DecodNom::bin2dec(const std::vector<double>& bits) {
    int value = 0;
    for (double bit : bits) {
        value = (value << 1) | static_cast<int>(bit);
    }
    return value;
}

char DecodNom::bin2carid(const std::vector<double>& bits) {

    if (bits.size() == 6) {
        const std::string majA_O = "ABCDEFGHIJKLMNO";
        const std::string majP_Z = "PQRSTUVWXYZ     ";

        std::vector<double> flipped_vect = bits;
        std::reverse(flipped_vect.begin(), flipped_vect.end());
        std::vector<double> extract = getRange(flipped_vect,0,4);
        std::reverse(extract.begin(), extract.end());

        int value = bin2dec(extract);
        //std::cout<< "valeur : "<<value<<std::endl;

        if (flipped_vect[5] == 0) {
            if (flipped_vect[4] == 0) {
                return majA_O[value-1];
            } else {
                return majP_Z[value];
            }
        } else {
            if (flipped_vect[4] == 1) {
                return std::to_string(value)[0];
            } else {
                return ' ';
            }
        }
    } else {
        throw std::invalid_argument("La suite de bits n'est pas de taille 6");
    }
}


std::vector<double> DecodNom::getRange(std::vector<double>& vect, int start, int end) {
    return std::vector<double>(vect.begin() + start, vect.begin() + end);
}