#ifndef DECIMATION_PM_HPP
#define DECIMATION_PM_HPP

#include <vector>
#include <streampu.hpp>

class DecimationPM : public spu::module::Stateful {
protected:
    int n_elmts;

public:
    //DecimationPM(int nombreVoies); On force le nomre de voie à 10
    DecimationPM(const int n_elmts, int isComplex);
    virtual ~DecimationPM() = default;
    void process(const double* input,
        double* voie1,
        double* voie2,
        );

    int isComplex;


};

#endif // DECIMATION_PM_HPP
