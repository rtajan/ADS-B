#include "Bit2Register.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iterator> // Pour std::next


Bit2Register::Bit2Register(const int n_elmts)
    : Stateful()
    ,n_elmts(n_elmts) {

        const std::string name = "Bit2Register";
        this->set_name(name);

        auto& p = this->create_task("process");                                                  // Create the task
        size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
        size_t ps_output = this->template create_socket_out<int8_t>(p, "output", sizeof(Register)); // Create the output socket

        // create the codelet
        this->create_codelet(
          p,
          [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
          {
              // Recover the Module and Sockets in the codelet
              auto& bit2register = static_cast<Bit2Register&>(m);
              double* input = (double*)(t[ps_input].get_dataptr());
              Register * output = t[ps_output].get_dataptr<Register>();

              // Process the data
              bit2register.process(input, output);
              return spu::runtime::status_t::SUCCESS;
          });

}

void Bit2Register::process(double * input, Register * registre) {

    Register * out = new(registre) Register; //delete à faire

    std::vector<double> vectbin(input, input + n_elmts);

    auto formatBits = getRange(vectbin, 0, 5);
    auto adresseBits = getRange(vectbin, 8, 32);
    auto typeBits = getRange(vectbin, 32, 37);

    out->format = bin2dec(formatBits);
    bin2hex(adresseBits,out->adresse);
    out->type = bin2dec(typeBits);

    if ((registre->type >= 9 && registre->type <= 18) || (registre->type >= 20 && registre->type <= 22)) {
        auto altitudeBits = getRange(vectbin, 40, 47);

        //afficher les paramètres d'insert


        altitudeBits.insert(altitudeBits.end(), getRange(vectbin, 48, 52).begin(), getRange(vectbin, 48, 52).end()); //ligne a veirfier, paramètre d'insert
        registre->altitude = bin2dec(altitudeBits) * 25 - 1000;
        registre->timeFlag = vectbin[52];
        registre->cprFlag = vectbin[53];
        int LAT = bin2dec(getRange(vectbin, 54, 71));
        int LON = bin2dec(getRange(vectbin, 71, 88));
        registre->latitude = calcLAT(registre->cprFlag, LAT, 44.806884);
        registre->longitude = calcLON(registre->cprFlag, LON, -0.606629, registre->latitude);
    }
    if (registre->type >= 1 && registre->type <= 4) {
        std::string nom = "";
        for (int i = 0; i < 8; ++i) {
            auto charBits = getRange(vectbin, 40 + i * 6, 46 + i * 6);
            nom += bin2carid(charBits);
        }
        registre->nom = nom;
        std::cout << nom << std::endl;
    }

    //return registre;
}



int Bit2Register::bin2dec(const std::vector<double>& bits) {
    int value = 0;
    for (double bit : bits) {
        value = (value << 1) | static_cast<int>(bit);
    }
    return value;
}

void Bit2Register::bin2hex(const std::vector<double>& bits, std::string& out) {

    std::cout<<"dans la fct"<<std::endl;
    int value = bin2dec(bits);
    std::cout<<"après bin2dec"<<value<<std::endl;
    std::stringstream ss;
    std::cout<<"après stringstream"<<std::endl;
    ss << std::hex << std::setw(6) << std::setfill('0') << value;
    std::cout<<"après fleche "<< ss.str() <<std::endl;

    auto longu = ss.str().length();

    out.reserve(longu);
    out = ss.str();

    //return new std::string(ss.str());
}

char Bit2Register::bin2carid(const std::vector<double>& bits) {
    return static_cast<char>(bin2dec(bits));
}

double Bit2Register::NL(double x, int Nz) {
    if (x == 0) {
        return 59;
    } else if (x == 87) {
        return 2;
    } else if (std::abs(x) > 87) {
        return 1;
    } else {
        return std::floor(2 * M_PI *
                          std::pow(std::acos(1 - (1 - std::cos(M_PI / (2 * Nz))) /
                                             std::pow(std::cos(M_PI * std::abs(x) / 180), 2)), -1));
    }
}

double Bit2Register::calcLAT(bool cprFlag, int LAT, double latref) {
    const int Nz = 15; // Nombre de latitudes géographiques considérées entre l'équateur et un pôle
    const double Dlati = 360.0 / (4 * Nz - cprFlag);
    const int Nb = 17; // Nombre de bits constituant le registre de latitude

    // Calcul de l'indice j
    double j = std::floor(latref / Dlati) +
               std::floor(0.5 + std::fmod(latref, Dlati) / Dlati - static_cast<double>(LAT) / (1 << Nb));

    // Calcul de la latitude
    double lat = Dlati * (j + static_cast<double>(LAT) / (1 << Nb));

    return lat;
}

double Bit2Register::calcLON(bool cprFlag, int LON, double lonref, double lat) {
    const int Nb = 17; // Nombre de bits utilisés pour représenter la longitude
    const int Nz = 15; // Nombre de latitudes géographiques considérées entre l'équateur et un pôle

    // Calcul de Dloni
    double Dloni;
    if (NL(lat, Nz) - cprFlag > 0) {
        Dloni = 360.0 / (NL(lat, Nz) - cprFlag);
    } else {
        Dloni = 360.0; // degrés
    }

    // Calcul de l'indice m
    double m = std::floor(lonref / Dloni) +
               std::floor(0.5 + std::fmod(lonref, Dloni) / Dloni - static_cast<double>(LON) / (1 << Nb));

    // Calcul de la longitude
    double lon = Dloni * (m + static_cast<double>(LON) / (1 << Nb));

    return lon;
}

std::vector<double> Bit2Register::getRange(std::vector<double>& vect, int start, int end) {
    return std::vector<double>(vect.begin() + start, vect.begin() + end);
}
