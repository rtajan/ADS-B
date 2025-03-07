#ifndef BIT2REGISTER_HPP
#define BIT2REGISTER_HPP

#include <vector>
#include <string>
#include <streampu.hpp>

class Register{
public:
    std::string adresse;
    int format;
    int type;
    std::string nom;
    int altitude;
    bool timeFlag;
    bool cprFlag;
    double latitude;
    double longitude;

    Register() : adresse("000000"), nom("Unknow"), type(0) {
        std::cerr << "adresse.capacity()=" <<adresse.capacity()<<std::endl;
    }
};

class Bit2Register : public spu::module::Stateful{
protected:
    int n_elmts;

public:
    Bit2Register(const int n_elmts);
    virtual ~Bit2Register() = default;
    void process(double * input, Register * output);

private:
    int bin2dec(const std::vector<double>& bits);
    void bin2hex(const std::vector<double>& bits, std::string& out);
    char bin2carid(const std::vector<double>& bits);
    double NL(double x, int Nz);
    double calcLAT(bool cprFlag, int LAT, double ref_latitude);
    double calcLON(bool cprFlag, int LON, double ref_longitude, double latitude);
    std::vector<double> getRange(std::vector<double>& vect, int start, int end);
};

#endif // BIT2REGISTER_HPP
