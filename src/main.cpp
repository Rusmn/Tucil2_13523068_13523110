#include <iostream>
#include <string>
#include <chrono>

// Library untuk membaca/menulis gambar
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "quadtree.hpp"
#include "error_metrics.hpp"
#include "image_utils.hpp"

using namespace std;
using hrc = chrono::high_resolution_clock;
using millisec = chrono::milliseconds;
using chrono::duration_cast;

void printHeader() {

cout << "===============================================================" << endl;
    cout << R"(
        ___  _   _   _    ____ _____ ____  _____ _____ 
       / _ \| | | | / \  |  _ \_   _|  _ \| ____| ____|
      | | | | | | |/ _ \ | | | || | | |_) |  _| |  _|  
      | |_| | |_| / ___ \| |_| || | |  _ <| |___| |___ 
       \__\_\\___/_/   \_\____/ |_| |_| \_\_____|_____|

===============================================================

)" << endl;
}

int main() {
    string inPath, outPath, gifPath;
    int errMethod, minBlk;
    double thresh, targetComp;

    printHeader();

    cout << "Masukkan path gambar input (ex: input.jpg): ";
    getline(cin, inPath);
    inPath = "test/" + inPath;
    cout << endl;

    cout << "Metode perhitungan error." << endl;
    cout << "1. Varian" << endl; 
    cout << "2. MAD" << endl;
    cout << "3. Max Diff" << endl;
    cout << "4. Entropi" << endl;

    while (true) {
        cout << "Pilih metode perhitungan error (1/2/3/4): ";
        cin >> errMethod;
    
        if (cin.fail()) {
            cout << "Input tidak valid. Masukkan angka bulat antara 1 hingga 4.\n" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
    
        if (errMethod >= 1 && errMethod <= 4) {
            break; 
        } 
        else {
            cout << "Input tidak valid. Masukkan angka 1 hingga 4.\n" << endl;
        }
    }
    
    while (true) {
        cout << "Nilai ambang batas (threshold): ";
        cin >> thresh;

        if (cin.fail()) {
            cout << "Input tidak valid.\n" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        } 
        else {
            break;
        }
    }
    
    cout << "Ukuran blok minimum: ";
    cin >> minBlk;
    while(minBlk < 1) {
        cout << "Ukuran blok minimum harus lebih besar dari 0.\n" << endl;
        cout << "Ukuran blok minimum: ";
        cin >> minBlk;
    }
    
    while (true) {
        cout << "Target persentase kompresi (0.0 - 1.0, 0 untuk menonaktifkan): ";
        cin >> targetComp;

        if (cin.fail() || targetComp < 0.0 || targetComp > 1.0) {
            cout << "Input tidak valid. Masukkan angka antara 0.0 hingga 1.0.\n" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    cout << "Masukkan path gambar output (ex: hasil.png): ";
    getline(cin, outPath);
    outPath = "test/" + outPath;
    cout << endl;
    cout << "===============================================================" << endl;

    // Muat gambar dari file
    int w, h, ch;
    unsigned char *inImg = stbi_load(inPath.c_str(), &w, &h, &ch, 0);
    
    if (!inImg) {
        cerr << "Gagal memuat gambar! (perhatikan path atau format gambar)" << endl;
        cout << "==============================================================="  << endl;
        return 0;
    }
    
    cout << "Gambar berhasil dimuat: " << w << "x" << h << " dengan " << ch << " channel" << endl;
    
    auto startTime = hrc::now();
    
    QStats stats;
    stats.origSize = calcImgSize(w, h, ch);
    
    // Quadtree
    QNode *root = buildQTree(inImg, w, h, ch, minBlk, thresh, errMethod, stats);
    
    // Hasil Quadtree
    unsigned char *outImg = new unsigned char[w * h * ch];
    
    for (int i = 0; i < w * h * ch; i++) {
        outImg[i] = 255;
    }
    
    // Gambar hasil kompresi
    genImage(root, outImg, w, ch);
    
    // Hitung waktu eksekusi
    auto endTime = hrc::now();
    auto duration = duration_cast<millisec>(endTime - startTime);
    
    // Simpan gambar hasil kompresi
    if (!stbi_write_png(outPath.c_str(), w, h, ch, outImg, w * ch)) {
        cerr << "Gagal menyimpan gambar!" << endl;
    } 
    else {
        cout << "Gambar berhasil disimpan di: " << outPath << endl;
        cout << "==============================================================="  << endl;
    }
     
    // Hitung dan tampilkan statistik kompresi
    stats.compSize = stats.nodeCount * sizeof(QNode);
    double compPerc = (1.0 - (double)stats.compSize / stats.origSize) * 100.0;

    cout << "======================= HASIL KOMPRESI ========================" << endl;
    cout << endl;
    cout << "Waktu eksekusi: " << duration.count() << " ms" << endl;
    cout << "Ukuran gambar asli: " << stats.origSize << " bytes" << endl;
    cout << "Ukuran gambar terkompresi: " << stats.compSize << " bytes" << endl;
    cout << "Persentase kompresi: " << compPerc << "%" << endl;
    cout << "Kedalaman pohon: " << stats.maxDepth << endl;
    cout << "Jumlah simpul pada pohon: " << stats.nodeCount << endl;
    cout << "==============================================================="  << endl;
    
    // Bersihkan memori
    stbi_image_free(inImg);
    delete[] outImg;
    cleanQTree(root);
    
    return 0;
}