#ifndef ERROR_METRICS_H
#define ERROR_METRICS_H

#include <cstddef>
using namespace std;

// Menghitung variansi warna dalam blok gambar
double calcVar(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h);

// Menghitung Mean Absolute Deviation dalam blok gambar
double calcMAD(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h);

// Menghitung perbedaan maksimum piksel dalam blok gambar
double calcMaxDiff(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h);

// Menghitung entropi warna dalam blok gambar
double calcEntropy(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h);

// Memilih dan memanggil metode pengukuran error yang sesuai
double calcError(unsigned char* imgData, int imgW, int imgH, int chn, int x, int y, int w, int h, int method);

#endif