#include "quadtree.hpp"
#include "error_metrics.hpp"
#include "image_utils.hpp"

QNode *quadTree(unsigned char *imgData, int imgW, int imgH, int chn, int x, int y, int w, int h, int minBlk, double thresh, int errMethod, QStats &stats, int depth) {
    QNode *node = new QNode();
    node->x = x;
    node->y = y;
    node->w = w;
    node->h = h;

    calcAvgColor(imgData, imgW, imgH, chn, x, y, w, h, node->r, node->g, node->b);

    stats.nodeCount++;
    stats.maxDepth = max(stats.maxDepth, depth);

    if (w > minBlk && h > minBlk) {
        double error = calcError(imgData, imgW, imgH, chn, x, y, w, h, errMethod);
        
        if (error > thresh) {
            int halfW = w / 2;
            int halfH = h / 2;

            int w1 = halfW;
            int w2 = w - halfW;
            int h1 = halfH;
            int h2 = h - halfH;

            node->child[0] = quadTree(imgData, imgW, imgH, chn, x, y, w1, h1, minBlk, thresh, errMethod, stats, depth + 1);
            node->child[1] = quadTree(imgData, imgW, imgH, chn, x + w1, y, w2, h1, minBlk, thresh, errMethod, stats, depth + 1);
            node->child[2] = quadTree(imgData, imgW, imgH, chn, x, y + h1, w1, h2, minBlk, thresh, errMethod, stats, depth + 1);
            node->child[3] = quadTree(imgData, imgW, imgH, chn, x + w1, y + h1, w2, h2, minBlk, thresh, errMethod, stats, depth + 1);
        }
    }

    return node;
}

QNode *buildQTree(unsigned char *imgData, int imgW, int imgH, int chn, int minBlk, double thresh, int errMethod, QStats& stats) {
    stats.nodeCount = 0;
    stats.maxDepth = 0;
    return quadTree(imgData, imgW, imgH, chn, 0, 0, imgW, imgH, minBlk, thresh, errMethod, stats, 0);
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