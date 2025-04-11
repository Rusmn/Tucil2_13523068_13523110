#include "error_metrics.hpp"
#include <bits/stdc++.h>
#include <algorithm>
#include <cmath>
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
}

// Menghitung Mean Absolute Deviation dalam blok gambar
double calcMAD(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h){
    double mean[3] = {0.0, 0.0, 0.0};
    double mad[3] = {0.0, 0.0, 0.0};
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

    // Menghitung Mean Absolute Deviation tiap channel dalam blok gambar
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            int idx = (i * imgW + j) * chn;
            for (int c = 0; c < chn; c++) {
                mad[c] += fabs(imgData[idx + c] - mean[c]);
            }
        }
    }
    for (int c = 0; c < chn; c++) {
        mad[c] /= count;
    }

    double sumOfAbsDiffs = 0.0;
    for(int c = 0; c < chn; c++) {
        sumOfAbsDiffs += mad[c]; // Menghitung jumlah dari MAD tiap channel
    }

    double meanOfAbsDiffs = sumOfAbsDiffs / chn; // Menghitung rata-rata dari MAD
    return meanOfAbsDiffs;
}

// Menghitung perbedaan maksimum piksel dalam blok gambar
double calcMaxDiff(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h){
    int minVal[3] = {255, 255, 255};
    int maxVal[3] = {0, 0, 0};

    // Menghitung nilai minimum dan maksimum tiap channel dalam blok gambar
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            int idx = (i * imgW + j) * chn;
            for (int c = 0; c < chn; c++) {
                int val = imgData[idx + c];
                minVal[c] = min(minVal[c], val);
                maxVal[c] = max(maxVal[c], val);
            }
        }
    }

    double sumOfMaxDiffs = 0.0;
    for(int c = 0; c < chn; c++) {
        double maxDiff = maxVal[c] - minVal[c]; // Menghitung perbedaan maksimum tiap channel
        sumOfMaxDiffs += maxDiff; // Menghitung jumlah dari perbedaan maksimum tiap channel
    }

    double meanOfMaxDiffs = sumOfMaxDiffs / chn; // Menghitung rata-rata dari perbedaan maksimum
    return meanOfMaxDiffs;
}

// Menghitung entropi warna dalam blok gambar
double calcEntropy(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h){
    int freqR[256] = {0};
    int freqG[256] = {0};
    int freqB[256] = {0};
    int total = w * h;

    for (int i = y; i < y + h; ++i) {
        for (int j = x; j < x + w; ++j) {
            int idx = (i * imgW + j) * chn;
            freqR[imgData[idx]]++;
            freqG[imgData[idx + 1]]++;
            freqB[imgData[idx + 2]]++;
        }
    }

    auto calcOneEntropy = [](int freq[256], int total) -> double {
        double entropy = 0.0;
        for (int k = 0; k < 256; ++k) {
            if (freq[k] > 0) {
                double p = (double)freq[k] / total;
                entropy -= p * log2(p);
            }
        }
        return entropy;
    };

    double entropyR = calcOneEntropy(freqR, total);
    double entropyG = calcOneEntropy(freqG, total);
    double entropyB = calcOneEntropy(freqB, total);

    return (entropyR + entropyG + entropyB) / 3.0;
}
    
// Memilih dan memanggil metode pengukuran error yang sesuai
double calcError(unsigned char* imgData, int imgW, int imgH, int chn, int x, int y, int w, int h, int method){
    switch (method) {
        case 1:
            return calcVar(imgData, imgW, imgH, chn, x, y, w, h);
        case 2:
            return calcMAD(imgData, imgW, imgH, chn, x, y, w, h);
        case 3:
            return calcMaxDiff(imgData, imgW, imgH, chn, x, y, w, h);
        case 4:
            return calcEntropy(imgData, imgW, imgH, chn, x, y, w, h);
        default:
            return -1.0; // Error code
    }
}