#ifndef DECODCOORD_HPP
#define DECODCOORD_HPP

#include <streampu.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iterator>

class DecodCoord : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    DecodCoord(const int n_elmts);
    virtual ~DecodCoord() = default;
    void process(const double* tram, int* altitude, double* longitude, double* latitude);

private:
    int bin2dec(const std::vector<double>& bits);
    double NL(double x, int Nz);
    double calcLAT(int cprFlag, int LAT, double ref_latitude);
    double calcLON(int cprFlag, int LON, double ref_longitude, double latitude);
    std::vector<double> getRange(std::vector<double>& vect, int start, int end);

};

#endif // DECODCOORD_HPP