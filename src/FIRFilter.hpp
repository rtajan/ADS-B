#ifndef FIRFILTER_HPP
#define FIRFILTER_HPP

#include <streampu.hpp>
#include <pybind11/numpy.h>
namespace py = pybind11;

class FIRFilter : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    FIRFilter(const int n_elmts,const std::vector<double>& b, int bufferSize);
    virtual ~FIRFilter() = default;
    void process(const double* input, double* output);
    void reset();

private:
    std::vector<double> b;
    std::vector<double> buffer;
    int head;
    int size_b;

};

#endif // FIRFILTER_HPP