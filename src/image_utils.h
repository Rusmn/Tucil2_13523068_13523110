#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <bits/stdc++.h>
using namespace std;

// Mengambil nilai R, G, B dari piksel pada posisi (x,y)
void getPixel(unsigned char *data, int imgW, int x, int y, int chn, int& r, int& g, int& b);

// Menetapkan nilai R, G, B pada piksel di posisi (x,y)
void setPixel(unsigned char *data, int imgW, int x, int y, int chn, int r, int g, int b);

// Menghitung rata-rata warna dalam suatu blok gambar
void calcAvgColor(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h, int& r, int& g, int& b);

// Menghitung ukuran gambar dalam bytes
size_t calcImgSize(int w, int h, int chn);

#endif