#include "edits.h"

int* inputImage(int* w, int* h, System::String^ imagePath)
{
    int* input;

    int OriginalImageWidth, OriginalImageHeight;

    System::Drawing::Bitmap BM(imagePath);

    OriginalImageWidth = BM.Width;
    OriginalImageHeight = BM.Height;
    *w = BM.Width;
    *h = BM.Height;
    int* Red = new int[BM.Height * BM.Width];
    int* Green = new int[BM.Height * BM.Width];
    int* Blue = new int[BM.Height * BM.Width];
    input = new int[BM.Height * BM.Width];
    for (int i = 0; i < BM.Height; i++)
    {
        for (int j = 0; j < BM.Width; j++)
        {
            System::Drawing::Color c = BM.GetPixel(j, i);

            Red[i * BM.Width + j] = c.R;
            Blue[i * BM.Width + j] = c.B;
            Green[i * BM.Width + j] = c.G;

            input[i * BM.Width + j] = ((c.R + c.B + c.G) / 3);
        }

    }
    return input;
}

int* outputImage(int* image, int width, int height, System::String^ imagePath)
{
    System::Drawing::Bitmap MyNewImage(width, height);
    for (int i = 0; i < MyNewImage.Height; i++)
    {
        for (int j = 0; j < MyNewImage.Width; j++)
        {
            if (image[i * width + j] < 0)
            {
				image[i * width + j] = 0;
			}
            if (image[i * width + j] > 255)
            {
				image[i * width + j] = 255;
			}
			System::Drawing::Color c = System::Drawing::Color::FromArgb(image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j]);
			MyNewImage.SetPixel(j, i, c);
		}
	}
	MyNewImage.Save(imagePath);
	cout << "result Image Saved " << endl;
	return image;
}

void createImage(int* image, int width, int height, int index, int mode)
{
    System::Drawing::Bitmap MyNewImage(width, height);

    for (int i = 0; i < MyNewImage.Height; i++)
    {
        for (int j = 0; j < MyNewImage.Width; j++)
        {
            if (image[i * width + j] < 0)
            {
                image[i * width + j] = 0;
            }
            if (image[i * width + j] > 255)
            {
                image[i * width + j] = 255;
            }
            System::Drawing::Color c = System::Drawing::Color::FromArgb(image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j]);
            MyNewImage.SetPixel(j, i, c);
        }
    }
    if (mode == 0) {
        MyNewImage.Save("..//Data//Output//openmp" + index + ".png");
    }
    else {
        MyNewImage.Save("..//Data//Output//mpi" + index + ".png");

    }
    cout << "result Image Saved " << index << endl;
}

// Function to apply a low pass filter with a variable kernel size using OpenMP
void applyLowPassFilter_OpenMP(int* input, int width, int height, int kernelSize)
{
    // Calculate kernel dimensions
    int kernelRadius = kernelSize / 2;
    int kernelWidth = kernelSize;
    int kernelHeight = kernelSize;

    // Allocate memory for kernel
    int* kernel = new int[kernelWidth * kernelHeight];

    // Initialize kernel with ones
    for (int i = 0; i < kernelWidth * kernelHeight; i++)
    {
        kernel[i] = 1;
    }

    // Loop through image pixels and apply filter
#pragma omp parallel for collapse(2)
    for (int i = kernelRadius; i < height - kernelRadius; i++)
    {
        for (int j = kernelRadius; j < width - kernelRadius; j++)
        {
            int sum = 0;
            for (int k1 = -kernelRadius; k1 <= kernelRadius; k1++)
            {
                for (int k2 = -kernelRadius; k2 <= kernelRadius; k2++)
                {
                    sum += input[(i + k1) * width + (j + k2)] * kernel[(k1 + kernelRadius) * kernelWidth + (k2 + kernelRadius)];
                }
            }
            input[i * width + j] = sum / (kernelSize * kernelSize); // Divide by total kernel weight to get average
        }
    }

    // Set border pixels to black
#pragma omp parallel for
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < kernelRadius; j++)
        {
            input[i * width + j] = 0;
            input[i * width + (width - 1 - j)] = 0;
        }
    }

#pragma omp parallel for
    for (int j = kernelRadius; j < width - kernelRadius; j++)
    {
        for (int i = 0; i < kernelRadius; i++)
        {
            input[i * width + j] = 0;
            input[(height - 1 - i) * width + j] = 0;
        }
    }

    // Free memory for kernel
    delete[] kernel;
}

// Function to apply a low pass filter with a variable kernel size using MPI
void applyLowPassFilter_MPI(int* imageData, int ImageWidth, int ImageHeight, int kernelSize, int world_size, int rank)
{ 
    int* localData = new int[ImageWidth * ImageHeight / world_size];
    MPI_Scatter(imageData, ImageWidth * ImageHeight / world_size, MPI_INT, localData, ImageWidth * ImageHeight / world_size, MPI_INT, 0, MPI_COMM_WORLD);

    int* filteredData = new int[ImageWidth * ImageHeight / world_size];
    int* kernel = new int[kernelSize * kernelSize];

    for (int i = 0; i < kernelSize * kernelSize; i++)
    {
        kernel[i] = 1;
    }

    for (int i = kernelSize / 2; i < ImageHeight / world_size - kernelSize / 2; i++)
    {
        for (int j = kernelSize / 2; j < ImageWidth - kernelSize / 2; j++)
        {
            int sum = 0;
            for (int k = -kernelSize / 2; k <= kernelSize / 2; k++)
            {
                for (int l = -kernelSize / 2; l <= kernelSize / 2; l++)
                {
                    sum += localData[(i + k) * ImageWidth + (j + l)] * kernel[(k + kernelSize / 2) * kernelSize + (l + kernelSize / 2)];
                }
            }
            filteredData[i * ImageWidth + j] = sum / (kernelSize * kernelSize);
        }
    }

    MPI_Gather(filteredData, ImageWidth * ImageHeight / world_size, MPI_INT, imageData, ImageWidth * ImageHeight / world_size, MPI_INT, 0, MPI_COMM_WORLD);

    delete[] localData;
    delete[] filteredData;
    delete[] kernel;
}
