#include "DecodCoord.hpp"


DecodCoord::DecodCoord(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts)
     {


    const std::string name = "DecodCoord";
    this->set_name(name);

    auto& p = this->create_task("process");
    size_t ps_tram = this->template create_socket_in<double>(p, "tram", 112);

    size_t ps_altitude = this->template create_socket_out<int>(p, "altitude", 1);
    size_t ps_latitude = this->template create_socket_out<double>(p, "latitude", 1);
    size_t ps_longitude = this->template create_socket_out<double>(p, "longitude", 1);

    // create the codelet
    this->create_codelet(
      p,
      [ps_tram, ps_altitude, ps_latitude,ps_longitude](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& decodCoord = static_cast<DecodCoord&>(m);
          double* tram = (double*)(t[ps_tram].get_dataptr());
          int* altitude = (int*)(t[ps_altitude].get_dataptr());
          double* latitude = (double*)(t[ps_latitude].get_dataptr());
          double* longitude = (double*)(t[ps_longitude].get_dataptr());

          // Process the data
          decodCoord.process(tram, altitude,longitude,latitude);
          return spu::runtime::status_t::SUCCESS;
      });
}

void DecodCoord::process(const double* tram, int* altitude, double* longitude, double* latitude) {
    std::vector<double> vectbin(tram, tram + 112);

    auto altitudeBits = getRange(vectbin, 40, 47);
    auto tempVect = getRange(vectbin, 48, 52);
    altitudeBits.insert(altitudeBits.end(), tempVect.begin(), tempVect.end());
    *altitude = bin2dec(altitudeBits) * 25 - 1000;
    double timeFlag = vectbin[52];
    double cprFlag = vectbin[53];
    int LAT = bin2dec(getRange(vectbin, 54, 71));
    int LON = bin2dec(getRange(vectbin, 71, 88));
    *latitude = calcLAT(cprFlag, LAT, 44.806884);
    *longitude = calcLON(cprFlag, LON, -0.606629, *latitude);

}

int DecodCoord::bin2dec(const std::vector<double>& bits) {
    int value = 0;
    for (double bit : bits) {
        value = (value << 1) | static_cast<int>(bit);
    }
    return value;
}

double DecodCoord::NL(double x, int Nz) {
    if (x == 0) {
        return 59;
    } else if (x == 87) {
        return 2;
    } else if (std::abs(x) > 87) {
        return 1;
    } else {
        //return std::floor(2 * M_PI *
        //                  std::pow(std::acos(1 - (1 - std::cos(M_PI / (2 * Nz))) /
        //                                     std::pow(std::cos(M_PI * std::abs(x) / 180), 2)), -1));
        return std::floor(2*M_PI / (std::acos(1- ( (1-std::cos(M_PI/(2*Nz))) / (std::pow(std::cos(M_PI*x/180),2)) ))));
    }
}

double DecodCoord::calcLAT(int cprFlag, int LAT, double latref) {
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

double DecodCoord::calcLON(int cprFlag, int LON, double lonref, double lat) {
    const int Nb = 17; // Nombre de bits utilisés pour représenter la longitude
    const int Nz = 15; // Nombre de latitudes géographiques considérées entre l'équateur et un pôle

    // Calcul de Dloni
    double Dloni;
    if (NL(lat, Nz) - cprFlag > 0) {
        Dloni = 360.0 / (NL(lat, Nz)); //NL(lat,Nz) - cprFlag
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


std::vector<double> DecodCoord::getRange(std::vector<double>& vect, int start, int end) {
    return std::vector<double>(vect.begin() + start, vect.begin() + end);
}