#ifndef AbsolueCarre_HPP
#define AbsolueCarre_HPP

#include <streampu.hpp>

class AbsolueCarre : public spu::module::Stateful {
protected:
    int n_elmts;

public:

AbsolueCarre(const int n_elmts);
    virtual ~AbsolueCarre() = default;
    void process(const double* input, double* output);

};

#endif // AbsolueCarre_HPP