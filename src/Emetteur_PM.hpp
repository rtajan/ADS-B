#ifndef EMETTEUR_PM_H
#define EMETTEUR_PM_H

#include <vector>
#include <streampu.hpp>

class EmetteurPM : public spu::module::Stateful{
protected:
    int n_elmts;

public:
    EmetteurPM(const int n_elmts, double Fe, double Ts);
    virtual ~EmetteurPM() = default;
    void process(const double * input, double * output);

private:
    double Fe;
    double Ts;
    int Fse;
    int len_p;
    double ** porte;
};

#endif // EMETTEUR_PM_H
