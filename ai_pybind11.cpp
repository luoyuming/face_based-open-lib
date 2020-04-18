#include <pybind11/pybind11.h>
#include "api_python.h"


namespace py = pybind11;


PYBIND11_MODULE(tface, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

	m.def("face_init", &face_init);	
	m.def("face_check_person", &face_check_person);
	m.def("face_get_face_info", &face_get_face_info);
    m.def("face_input_image", &face_input_image, R"pbdoc(
        face_input_image

        Some other explanation about the face_input_image function.
    )pbdoc");


#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
