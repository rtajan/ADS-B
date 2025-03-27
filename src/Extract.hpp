#ifndef EXTRACT_HPP
#define EXTRACT_HPP

#include <streampu.hpp>

class Extract : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    Extract(const int n_elmts, const int Fse, const double seuil);
    virtual ~Extract() = default;
    void process(const int* decalage, const double* max, double* sigs, double* tram);
    bool isEmpty(double* input, int taille);

private:
    double seuil;
    int Fse;

    double* buffer;
    int voie;
    int size_buffer;

};

#endif // EXTRACT_HPP