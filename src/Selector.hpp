#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include <streampu.hpp>

class Selector : public spu::module::Stateful {
protected:
    int n_elmts;

public:
    Selector(const int n_elmts);
    virtual ~Selector() = default;
    void process(double* input[], int* output);

};

#endif // SELECTOR_HPP