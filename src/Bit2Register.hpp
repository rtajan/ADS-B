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
};

using Register = struct Register;

class Bit2Register : public spu::module::Stateful{
protected:
    int n_elmts;

public:
    Bit2Register(const int n_elmts);
    virtual ~Bit2Register() = default;
    void process(double * input, Register * output);

private:
    int bin2dec(const std::vector<bool>& bits);
    std::string bin2hex(const std::vector<bool>& bits);
    char bin2carid(const std::vector<bool>& bits);
    double NL(double x, int Nz);
    double calcLAT(bool cprFlag, int LAT, double ref_latitude);
    double calcLON(bool cprFlag, int LON, double ref_longitude, double latitude);
    std::vector<bool> getRange(std::vector<bool>& vect, int start, int end);
};

#endif // BIT2REGISTER_HPP
