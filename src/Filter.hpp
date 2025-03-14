#ifndef FILTER_HPP
#define FILTER_HPP

#include <streampu.hpp>

class Filter : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    Filter(const int n_elmts, double* h, const int size_h);
    virtual ~Filter() = default;
    void process(const double* input, double* output);

private:
    double* h;
    const int size_h;

};

#endif // FILTER_HPP