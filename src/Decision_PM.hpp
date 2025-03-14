#ifndef DECISION_PM_HPP
#define DECISION_PM_HPP

#include <streampu.hpp>

class DecisionPM : public spu::module::Stateful {
protected:
    int n_elmts;

public:
    DecisionPM(const int n_elmts, double v0);
    virtual ~DecisionPM() = default;
    void process(const double* input, double* output);

private:
    double v0;
    double dist_eucl(const double* vec);
};

#endif // DECISION_PM_HPP
