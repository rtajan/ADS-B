#ifndef DECODNOM_HPP
#define DECODNOM_HPP

#include <streampu.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iterator>

class DecodNom : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    DecodNom(const int n_elmts);
    virtual ~DecodNom() = default;
    void process(const double* tram, char nom[8]);

private:
    int bin2dec(const std::vector<double>& bits);
    char bin2carid(const std::vector<double>& bits);
    std::vector<double> getRange(std::vector<double>& vect, int start, int end);

};

#endif // DECODNOM_HPP