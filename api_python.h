#pragma once
#include "common.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>
namespace py = pybind11;


int face_init();
void face_input_image(py::array_t<unsigned char>& input);
bool face_check_person();
std::string  face_get_face_info();
