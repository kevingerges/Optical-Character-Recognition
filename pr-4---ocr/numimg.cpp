#include "digitblob.h"
#include "numimg.h"
#include "bmplib.h"
#include <algorithm> // for std::sort
#include <deque>
#include <iomanip>
#include <iostream>

using namespace std;


NumImg::NumImg(const char* bmp_filename)
{
    //  Note: readGSBMP dynamically allocates a 2D array 
    //    (i.e. array of pointers (1 per row/height) where each  
    //    point to an array of unsigned char (uint8_t) pixels)


    img_ = readGSBMP(bmp_filename, h_, w_);


    expd = new bool*[h_];
    for(int i=0; i<h_; i++){
        expd[i] = new bool[w_];
    }

    for(int i =0; i < h_; i++){
        for(int j = 0; j < w_; j++){
            expd[i][j] = false;
        }
    }


    if(img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }
    
    // Convert to Black and White using a fixed threshold 
    for(int i =0; i < h_; i++){
        for(int j = 0; j < w_; j++){
            if(img_[i][j] > 150){
                img_[i][j] = 255;
            }
            else {
                img_[i][j] = 0;
            }
        }
    }


}


NumImg::~NumImg()
{


    for(int i=0; i<h_; i++){
        delete[] img_[i];
        delete[] expd[i];
    }
    delete[] img_;
    delete[] expd;

}


size_t NumImg::findAndCreateDigitBlobs()
{

    for(int i=0; i<h_; i++){
        for(int j=0; j<w_; j++){
            if((int)img_[i][j] == 0 && !expd[i][j]){
                blobs_.push_back(createDigitBlob(i, j));
                // return 1;
            }
            expd[i][j] = true;
        }
    }

    sort(blobs_.begin(), blobs_.end());
    return blobs_.size();
}


std::string NumImg::classify(bool withDebug)
{
    std::string res;
    for(size_t i = 0; i < blobs_.size(); i++){
        blobs_[i].classify();
        if(withDebug){
            blobs_[i].printClassificationResults();
        }
        char c = blobs_[i].getClassification();
        res += c;
    }
    return res;
}


void NumImg::printBoundingBoxes() const
{
    cout << setw(2) << "i" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for(size_t i = 0; i < blobs_.size(); i++){
        const DigitBlob& b = blobs_[i];
        cout << setw(2) << i << setw(6) << b.getUpperLeft().row << setw(6) << b.getUpperLeft().col 
        << setw(4) << b.getHeight() << setw(4) << b.getWidth()  << endl;
        // cout << "Blob " << i << " ul=(" << b.getUpperLeft().row << "," << b.getUpperLeft().col 
        //     << ") h=" << b.getHeight() << " w=" << b.getWidth() << endl;
    }

}


const DigitBlob& NumImg::getDigitBlob(size_t i) const
{
    if(i >= blobs_.size()){
        throw std::out_of_range("Index to getDigitBlob is out of range");
    }
    return blobs_[i];
}


size_t NumImg::numDigitBlobs() const
{
    return blobs_.size();
}


void NumImg::sortDigitBlobs()
{
    std::sort(blobs_.begin(), blobs_.end());
}


void NumImg::drawBoundingBoxesAndSave(const char* filename)
{
    for(size_t i=0; i < blobs_.size(); i++){
        Location ul = blobs_[i].getUpperLeft();
        int h = blobs_[i].getHeight();
        int w = blobs_[i].getWidth();
        for(int i = ul.row-1; i < ul.row + h + 1; i++){
            img_[i][ul.col-1] = 128;
            img_[i][ul.col+w] = 128;
        }
        for(int j = ul.col-1; j < ul.col + w + 1; j++){
            img_[ul.row-1][j] = 128;
            img_[ul.row+h][j] = 128;
        }
    }
    writeGSBMP(filename, img_, h_, w_);
}


DigitBlob NumImg::createDigitBlob(int pr, int pc)
{
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};


    deque<Location> d;
    d.push_back(Location(pr, pc));

    Location tl(pr, pc), br(pr, pc);

    while(!d.empty()){
        Location cur = d.front();
        Location t;
        d.pop_front();
        expd[cur.row][cur.col] = true;

        for(int i=0; i<8; i++){
            t.row = cur.row + neighbor_row[i];
            t.col = cur.col + neighbor_col[i];

            if(expd[t.row][t.col])continue;
            if(img_[t.row][t.col] != 0)continue;

            if(t.row>br.row) br.row = t.row;
            if(t.col>br.col) br.col = t.col;

            if(t.row<tl.row) tl.row = t.row;
            if(t.col<tl.col) tl.col = t.col;

            d.push_back(Location(t.row, t.col));
            expd[t.row][t.col] = true;
        }
    }

    int width = br.col - tl.col + 1;
    int height = br.row - tl.row + 1;

    return DigitBlob(img_, tl, height, width); 

}

