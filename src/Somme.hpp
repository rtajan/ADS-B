#ifndef SOMME_HPP
#define SOMME_HPP

#include <streampu.hpp>

class Somme : public spu::module::Stateful {
protected:
    int n_elmts;

public:
    Somme(const int n_elmts);
    virtual ~Somme() = default;
    void process(const double* input, double* output);

};

#endif // SOMME_HPP
