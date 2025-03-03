#include "Recepteur_pm.hpp"
#include <pybind11/pybind11.h>
#include <streampu.hpp>

namespace py = pybind11;
using namespace py::literals;

// Create a	python module using PYBIND11, here our module will be named pyaf
PYBIND11_MODULE(ADS-B, m)
{
    auto pyspu_stateful = (py::object)py::module_::import("streampu").attr("Stateful");
    py::class_<ReceptuerPM, spu::module::Stateful>(m, "RecepteurPM")
        .def(py::init<const int, double, double>(), "n_elmts"_a, "Fe"_a, "Ts"_a);
}