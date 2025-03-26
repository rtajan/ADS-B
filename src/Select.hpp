#ifndef SELECT_HPP
#define SELECT_HPP

#include <streampu.hpp>

class Select : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    Select(const int n_elmts);
    virtual ~Select() = default;
    void process(const double* num, const double* denum, int* decalage, int* max);

};

#endif // SELECT_HPP