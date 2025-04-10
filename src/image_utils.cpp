#include "image_utils.hpp"

void getPixel(unsigned char *data, int imgW, int x, int y, int chn, int& r, int& g, int& b) {
    int idx = (y * imgW + x) * chn;
    r = data[idx];
    g = data[idx + 1];
    b = data[idx + 2];
}

void setPixel(unsigned char *data, int imgW, int x, int y, int chn, int r, int g, int b) {
    int idx = (y * imgW + x) * chn;
    data[idx] = r;
    data[idx + 1] = g;
    data[idx + 2] = b;
}

void calcAvgColor(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h, int& r, int& g, int& b) {
    long sumR = 0, sumG = 0, sumB = 0;
    int count = 0;
    
    for (int j = y; j < y + h && j < imgH; j++) {
        for (int i = x; i < x + w && i < imgW; i++) {
            int pixR, pixG, pixB;
            getPixel(imgData, imgW, i, j, chn, pixR, pixG, pixB);
            
            sumR += pixR;
            sumG += pixG;
            sumB += pixB;
            count++;
        }
    }
    
    if (count > 0) {
        r = sumR / count;
        g = sumG / count;
        b = sumB / count;
    } 
    else {
        r = g = b = 0;
    }
}

size_t calcImgSize(int w, int h, int chn) {
    return static_cast<size_t>(w) * h * chn;
}