#ifndef CORR_HPP
#define CORR_HPP

#include <streampu.hpp>

class Corr : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    Corr(const int n_elmts);
    virtual ~Corr() = default;
    void process(const double* num, const double* denum, double* intercorr);

};

#endif // CORR_HPP