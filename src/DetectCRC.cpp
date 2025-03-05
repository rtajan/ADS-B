#include "DetectCRC.hpp"

DetectCRC::DetectCRC(const int n_elmts)
    : Stateful()
    , n_elmts(n_elmts){

    Polynome_CRC = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1};

    const std::string name = "DetectCRC";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<bool>(p, "output", 1); // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& detectCRC = static_cast<DetectCRC&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          detectCRC.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });

}

bool DetectCRC::process(double* tram, bool output) {
    std::vector<bool> tram_sans_crc = tram;


    tram_sans_crc.insert(tram_sans_crc.end(), 24, 0);


    for (size_t i = 0; i < tram_sans_crc.size() - 24; ++i) {
        if (tram_sans_crc[i]) {
            for (size_t j = 0; j < 25; ++j) {
                tram_sans_crc[i + j] = tram_sans_crc[i + j] ^ Polynome_CRC[j];
            }
        }
    }


    for (size_t i = tram_sans_crc.size() - 24; i < tram_sans_crc.size(); ++i) {
        if (tram_sans_crc[i]) {
            output = false;
            //return false;
        }
    }
    output = true;
    //return true;
}
