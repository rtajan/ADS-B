#ifndef REDIRIG_HPP
#define REDIRIG_HPP

#include <streampu.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iterator>

class Redirig : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    Redirig(const int n_elmts);
    virtual ~Redirig() = default;
    void process(const double* tram, char adresse[16], int* indic);

private:
    int bin2dec(const std::vector<double>& bits);
    void bin2hex(const std::vector<double>& bits, char* out);
    std::vector<double> getRange(std::vector<double>& vect, int start, int end);

};

#endif // REDIRIG_HPP