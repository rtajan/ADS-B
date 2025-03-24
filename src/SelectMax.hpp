#ifndef SELECTMAX_HPP
#define SELECTMAX_HPP

#include <streampu.hpp>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

class SelectMax : public spu::module::Stateful {
protected:
    int n_elmts;

public:

    SelectMax(const int n_elmts, const std::vector<double>& init);
    virtual ~SelectMax() = default;
    void process(const double* intercorr, int* indice_in, double* max, int* indice_out);

private:
    std::vector<double> init;
    int indice_state;

};

#endif // SELECTMAX_HPP