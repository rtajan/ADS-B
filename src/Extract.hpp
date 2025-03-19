#ifndef EXTRACT_HPP
#define EXTRACT_HPP

#include <streampu.hpp>

class Extract : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    Extract(const int n_elmts, const int rows, const int cols, const double seuil);
    virtual ~Extract() = default;
    void process(const double* intercorr, const int* indice, const double** sigs, double* tram);

private:
    double seuil;
    int rows;
    int cols;

    double* buffer;
    int voie;
    int size_buffer;

};

#endif // EXTRACT_HPP