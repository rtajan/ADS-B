#ifndef CANAL_PM_H
#define CANAL_PM_H

#include <streampu.hpp>
#include <vector>

class Canal : public spu::module::Stateful{
protected:
    int n_elmts;

public:
    Canal(const int n_elmts,const double SNR, const int len_p);
    virtual ~Canal() = default;
    void process(const double * sl, double * output);

private:
    const double SNR;
    const int len_p;

    double calculateEb(const double * sl);
    double * generateNoise();
};

#endif // CANAL_PM_H
