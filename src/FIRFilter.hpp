#ifndef FIRFILTER_HPP
#define FIRFILTER_HPP

#include <streampu.hpp>
#include "mipp.h"
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>


class FIRFilter : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    FIRFilter(const int n_elmts,const std::vector<double>& b, int size_b);
    virtual ~FIRFilter() = default;
    void process(const double* input, double* output);
    inline void step(const double* x_elt, double* y_elt);
    void reset();

private:
    std::vector<double> b;
    std::vector<double> buffer;
    int head;
    int size_b;
    int M;
    int P;

};

void FIRFilter::step(const double* x_elt, double* y_elt)
{
	this->buffer[this->head] = *x_elt;
	this->buffer[this->head + this->size_b] = *x_elt;

	*y_elt = this->buffer[this->head+1] * this->b[0];
	for (auto i = 1; i < this->size_b ; i++)
		*y_elt += this->buffer[this->head + 1 + i] * this->b[i];

	this->head++;
	this->head %= this->size_b;
}


#endif // FIRFILTER_HPP