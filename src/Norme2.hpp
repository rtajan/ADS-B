#ifndef NORME2_HPP
#define NORME2_HPP

#include <streampu.hpp>

class Norme2 : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    Norme2(const int n_elmts);
    virtual ~Norme2() = default;
    void process(const double* input, double* output);

};

#endif // NORME2_HPP