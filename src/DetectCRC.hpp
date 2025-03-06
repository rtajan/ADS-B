#ifndef DETECTCRC_HPP
#define DETECTCRC_HPP

#include <vector>
#include <streampu.hpp>

class DetectCRC : public spu::module::Stateful{
protected:
    int n_elmts;

public:
    DetectCRC(const int n_elmts);
    virtual ~DetectCRC() = default;
    void process(double* tram, int* output);

private:
    std::vector<bool> Polynome_CRC;
};

#endif // DETECTCRC_HPP
