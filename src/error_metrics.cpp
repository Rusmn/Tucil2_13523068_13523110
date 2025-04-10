#include "error_metrics.h"
#include <bits/stdc++.h>
using namespace std;

// Menghitung variansi warna dalam blok gambar
double calcVar(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h){
    double mean[3] = {0.0, 0.0, 0.0};
    double var[3] = {0.0, 0.0, 0.0};
    int count = 0;

    // Menghitung rata-rata tiap channel dalam blok gambar
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            int idx = (i * imgW + j) * chn;
            for (int c = 0; c < chn; c++) {
                mean[c] += imgData[idx + c];
            }
            count++;
        }
    }

    for (int c = 0; c < chn; c++) {
        mean[c] /= count;
    }

    // Menghitung variansi tiap channel dalam blok gambar
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            int idx = (i * imgW + j) * chn;
            for (int c = 0; c < chn; c++) {
                var[c] += pow(imgData[idx + c] - mean[c], 2);
            }
        }
    }

    for (int c = 0; c < chn; c++) {
        var[c] /= count;
    }

    double sumOfSquares = 0.0;
    for(int c = 0; c < chn; c++) {
        sumOfSquares += pow(var[c], 2); // Menghitung jumlah kuadrat dari variansi tiap channel
    }

    double meanOfSquares = sumOfSquares / chn; // Menghitung rata-rata kuadrat dari variansi
    return meanOfSquares;

// Menghitung Mean Absolute Deviation dalam blok gambar
double calcMAD(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h);

// Menghitung perbedaan maksimum piksel dalam blok gambar
double calcMaxDiff(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h);

// Menghitung entropi warna dalam blok gambar
double calcEntropy(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h);

// Memilih dan memanggil metode pengukuran error yang sesuai
double calcError(unsigned char* imgData, int imgW, int imgH, int chn, int x, int y, int w, int h, int method);
