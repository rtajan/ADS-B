#ifndef RECEPTEUR_PM_H
#define RECEPTEUR_PM_H

#include <streampu.hpp>
#include <vector>
#include <string>

class RecepteurPM : public spu::module::Stateful{
protected:
    int n_elmts;

public:
    RecepteurPM(const int n_elmts, double Fe, double Ts);
    virtual ~RecepteurPM() = default;
    void process(const double* yl, double* output);

private:
    double Fe;
    double Ts;
    int Fse;
    int len_p;
    double* porte;

    double* convolution(const double* signal, const double* filtre);
    double dist_eucl(const double vec);
};

#endif // RECEPTEUR_PM_H
