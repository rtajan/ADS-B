#ifndef CORRSELECT_HPP
#define CORRSELECT_HPP

#include <streampu.hpp>

class CorrSelect : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    CorrSelect(const int n_elmts);
    virtual ~CorrSelect() = default;
    void process(const double* num1, const double* num2, const double* denum1, const double* denum2, double* intercorr, int* indice);

};

#endif // CORRSELECT_HPP