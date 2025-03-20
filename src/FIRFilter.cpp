#include "FIRFilter.hpp"


FIRFilter::FIRFilter(const int n_elmts,const std::vector<double>& b, int bufferSize)
        : Stateful()
        ,n_elmts(n_elmts)
        , b(b)
        , buffer(bufferSize, 0.0)
        , head(0)
        , size_b((int)b.size())
         {
        assert(size_b > 0);



    const std::string name = "FIRFilter";
    this->set_name(name);

    auto& p = this->create_task("process");                                                  // Create the task
    size_t ps_input = this->template create_socket_in<double>(p, "input", this->n_elmts);    // Create the input socket
    size_t ps_output = this->template create_socket_out<double>(p, "output", size_b + this->n_elmts - 1);   // Create the output socket

    // create the codelet
    this->create_codelet(
      p,
      [ps_input, ps_output](spu::module::Module& m, spu::runtime::Task& t, const size_t frame_id) -> int
      {
          // Recover the Module and Sockets in the codelet
          auto& filter = static_cast<FIRFilter&>(m);
          double* input = (double*)(t[ps_input].get_dataptr());
          double* output = (double*)(t[ps_output].get_dataptr());

          // Process the data
          filter.process(input, output);
          return spu::runtime::status_t::SUCCESS;
      });
}



void FIRFilter::process(const double* input, double* output) {
    int rest = n_elmts % 4;

    for (int i = 0; i < rest; ++i) {
        buffer[head] = input[i];
        head = (head + 1) % buffer.size();

        output[i] = 0.0;
        for (int j = 0; j < size_b; ++j) {
            output[i] += b[j] * buffer[(head - j - 1 + buffer.size()) % buffer.size()];
        }
    }

    for (int i = rest; i < n_elmts; i += 4) {
        double ps0 = 0.0, ps1 = 0.0, ps2 = 0.0, ps3 = 0.0;

        for (int k = 0; k < size_b; k += 4) {
            double reg_b0 = b[k + 0];
            double reg_b1 = b[k + 1];
            double reg_b2 = b[k + 2];
            double reg_b3 = b[k + 3];

            double reg_x0 = input[i + k + 0];
            double reg_x1 = input[i + k + 1];
            double reg_x2 = input[i + k + 2];
            double reg_x3 = input[i + k + 3];

            ps0 += reg_b0 * reg_x0;
            ps1 += reg_b1 * reg_x1;
            ps2 += reg_b2 * reg_x2;
            ps3 += reg_b3 * reg_x3;
        }

        output[i] = ps0;
        output[i + 1] = ps1;
        output[i + 2] = ps2;
        output[i + 3] = ps3;

        for (int j = 0; j < 4; ++j) {
            buffer[head] = input[i + j];
            head = (head + 1) % buffer.size();
        }
    }
}

void FIRFilter::reset() {
    std::fill(buffer.begin(), buffer.end(), 0.0);
    head = 0;
}