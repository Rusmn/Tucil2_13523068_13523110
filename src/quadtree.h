#ifndef QUADTREE_H
#define QUADTREE_H

#include <bits/stdc++.h>
using namespace std;

// Struktur node untuk quadtree
struct QNode {
    int x, y;            // Posisi
    int w, h;            // Ukuran (width, height)
    int r, g, b;         // Rata-rata warna RGB
    QNode* child[4] = {nullptr}; // Children: top-left, top-right, bottom-left, bottom-right
};

// Struktur untuk menyimpan statistik
struct QStats {
    int nodeCount = 0;    // Jumlah node dalam tree
    int maxDepth = 0;     // Kedalaman maksimum tree
    size_t origSize = 0;  // Ukuran gambar asli
    size_t compSize = 0;  // Ukuran gambar terkompresi
};

// Membangun quadtree dengan algoritma divide and conquer
QNode *buildQTree(unsigned char *imgData, int imgW, int imgH, int chn, int minBlk, double thresh, int errMethod, QStats &stats);

// Menghasilkan gambar dari quadtree yang telah dibangun
void genImage(QNode *root, unsigned char *outImg, int imgW, int chn);

// Membersihkan memori yang digunakan oleh quadtree
void cleanQTree(QNode* root);

// Membuat gambar dengan visualisasi blok-blok quadtree (untuk debugging)
unsigned char *createDbgImg(QNode *root, int w, int h, int chn);

#endif