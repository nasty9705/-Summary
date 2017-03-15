#pragma once

#include "io.h"
#include "matrix.h"

Image align(Image srcImage, bool isPostprocessing, std::string postprocessingType, double fraction, bool isMirror, 
            bool isInterp, bool isSubpixel, double subScale);

uint minimum(Image blue, Image green, int k, int l);
long int maxsimum(Image blue, Image green, int k, int l);

Image sobel_x(Image src_image);

Image sobel_y(Image src_image);

Image unsharp(Image src_image);

Image gray_world(Image src_image);

Image mirror(Image src_image, uint radius);

Image resize(Image src_image, double scale);

Image custom(Image src_image, Matrix<double> kernel);

Image autocontrast(Image src_image, double fraction);

Image gaussian(Image src_image, double sigma, int radius);

Image gaussian_separable(Image src_image, double sigma, int radius);

Image median(Image src_image, int radius);

Image median_linear(Image src_image, int radius);

Image median_const(Image src_image, int radius);

Image canny(Image src_image, int threshold1, int threshold2);
