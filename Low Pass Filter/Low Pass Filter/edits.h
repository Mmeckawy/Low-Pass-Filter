#pragma once
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <msclr\marshal_cppstd.h>
#include <ctime>
#include <omp.h>
#include <mpi.h> // include MPI header
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
using namespace std;
using namespace msclr::interop;

int* inputImage(int* w, int* h, System::String^ imagePath);
int* outputImage(int* image, int width, int height, System::String^ imagePath);
void createImage(int* image, int width, int height, int index, int mode);
void applyLowPassFilter_Sequential(int* input, int width, int height, int kernelSize);
void applyLowPassFilter_OpenMP(int* input, int width, int height, int kernelSize, int threads);
void applyLowPassFilter_MPI(int* imageData, int ImageWidth, int ImageHeight, int kernelSize, int world_size, int rank);
