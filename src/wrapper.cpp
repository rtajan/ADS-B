#include "Recepteur_PM.hpp"
#include "Emetteur_PM.hpp"
#include <pybind11/pybind11.h>
#include <streampu.hpp>

namespace py = pybind11;
using namespace py::literals;

// Create a	python module using PYBIND11
PYBIND11_MODULE(ADS-B, m)
{
    auto pyspu_stateful = (py::object)py::module_::import("streampu").attr("Stateful");

    py::class_<RecepteurPM, spu::module::Stateful>(m, "RecepteurPM")
        .def(py::init<const int, double, double>(), "n_elmts"_a, "Fe"_a, "Ts"_a);

    py::class_<EmetteurPM, spu::module::Stateful>(m, "EmetteurPM")
        .def(py::init<const int, double, double>(), "n_elmts"_a, "Fe"_a, "Ts"_a);

    py::class_<Canal, spu::module::Stateful>(m, "Canal")
        .def(py::init<const int, const double, const int>(), "n_elmts"_a, "SNR"_a, "len_p"_a);
}