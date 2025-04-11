#include "quadtree.hpp"
#include "error_metrics.hpp"
#include "image_utils.hpp"

//helper build Quadtree
QNode *helperQT(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h, int minBlk, double thresh, int errMethod, QStats& stats, int depth) {
    QNode *node = new QNode();
    node->x = x;
    node->y = y;
    node->w = w;
    node->h = h;
    
    calcAvgColor(imgData, imgW, imgH, chn, x, y, w, h, node->r, node->g, node->b);
    
    stats.nodeCount++;
    stats.maxDepth = max(stats.maxDepth, depth);
    
    //DIVIDE
    if (w > minBlk && h > minBlk) {
        
        double error = calcError(imgData, imgW, imgH, chn, x, y, w, h, errMethod);
        
        // Jika error melebihi threshold, bagi menjadi 4 blok (DIVIDE)
        if (error > thresh) {
            int halfW = w / 2;
            int halfH = h / 2;
            
            //CONQUER
            node->child[0] = helperQT(imgData, imgW, imgH, chn, x, y, halfW, halfH, minBlk, thresh, errMethod, stats, depth + 1);
                                            
            node->child[1] = helperQT(imgData, imgW, imgH, chn, x + halfW, y, halfW, halfH, minBlk, thresh, errMethod, stats, depth + 1);
                                            
            node->child[2] = helperQT(imgData, imgW, imgH, chn, x, y + halfH, halfW, halfH, minBlk, thresh, errMethod, stats, depth + 1);
                                            
            node->child[3] = helperQT(imgData, imgW, imgH, chn, x + halfW, y + halfH, halfW, halfH, minBlk, thresh, errMethod, stats, depth + 1);
        }
    }
    
    return node;
}

QNode *buildQTree(unsigned char *imgData, int imgW, int imgH, int chn, int minBlk, double thresh, int errMethod, QStats& stats) {
    stats.nodeCount = 0;
    stats.maxDepth = 0;
    return helperQT(imgData, imgW, imgH, chn, 0, 0, imgW, imgH, minBlk, thresh, errMethod, stats, 0);
}

void genImage(QNode *root, unsigned char *outImg, int imgW, int chn) {
    if (!root){
        return;
    }
    
    if (!root->child[0] && !root->child[1] && !root->child[2] && !root->child[3]) {
        //COMBINE
        for (int y = root->y; y < root->y + root->h; y++) {
            for (int x = root->x; x < root->x + root->w; x++) {
                // Pastikan kita tidak melewati batas gambar
                if (x >= 0 && x < imgW && y >= 0) {
                    setPixel(outImg, imgW, x, y, chn, root->r, root->g, root->b);
                }
            }
        }
    } 
    else {
        //COMBINE
        for (int i = 0; i < 4; i++) {
            if (root->child[i]) {
                genImage(root->child[i], outImg, imgW, chn);
            }
        }
    }
}

void cleanQTree(QNode *root) {
    if (!root){
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (root->child[i]) {
            cleanQTree(root->child[i]);
        }
    }

    delete root;
}

// Fungsi visualisasi debug - HANYA UNTUK VISUALISASI
unsigned char *createDbgImg(QNode *root, int w, int h, int chn, bool drawBorder) {
    unsigned char *dbgImg = new unsigned char[w * h * chn];
    
    // Inisialisasi putih
    for (int i = 0; i < w * h * chn; i++) {
        dbgImg[i] = 255;
    }
    
    std::function<void(QNode*)> drawNode = [&](QNode *node) {
        if (!node) return;
        
        // Warnai blok
        for (int y = node->y; y < node->y + node->h; y++) {
            for (int x = node->x; x < node->x + node->w; x++) {
                if (x >= 0 && x < w && y >= 0 && y < h) {  // Cek batas
                    setPixel(dbgImg, w, x, y, chn, node->r, node->g, node->b);
                }
            }
        }
        
        // Gambar border hanya jika drawBorder=true
        if (drawBorder) {
            // Gunakan warna abu-abu muda untuk border agar tidak terlalu mencolok
            const int borderR = 220, borderG = 220, borderB = 220;
            
            // Gambar horizontal borders
            for (int x = node->x; x < node->x + node->w; x++) {
                if (x >= 0 && x < w && node->y >= 0 && node->y < h)
                    setPixel(dbgImg, w, x, node->y, chn, borderR, borderG, borderB);
                
                if (x >= 0 && x < w && node->y + node->h - 1 >= 0 && node->y + node->h - 1 < h)
                    setPixel(dbgImg, w, x, node->y + node->h - 1, chn, borderR, borderG, borderB);
            }
            
            // Gambar vertical borders
            for (int y = node->y; y < node->y + node->h; y++) {
                if (node->x >= 0 && node->x < w && y >= 0 && y < h)
                    setPixel(dbgImg, w, node->x, y, chn, borderR, borderG, borderB);
                
                if (node->x + node->w - 1 >= 0 && node->x + node->w - 1 < w && y >= 0 && y < h)
                    setPixel(dbgImg, w, node->x + node->w - 1, y, chn, borderR, borderG, borderB);
            }
        }
        
        // Panggil rekursif untuk semua anak
        for (int i = 0; i < 4; i++) {
            if (node->child[i]) {
                drawNode(node->child[i]);
            }
        }
    };
    
    drawNode(root);
    return dbgImg;
}