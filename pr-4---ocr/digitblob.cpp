#include "digitblob.h"
#include <iostream>
#include <vector>

using namespace std;


bool Location::operator<(const Location& other) const
{
    if(this->col < other.col) return true;
    if(this->col == other.col && this->row < other.col) return true;
    return false;
}


DigitBlob::DigitBlob()
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // ul_'s Location default constructor already initializes it to -1,-1

    // Initilaize h_ and w_ and any other data members

    h_ = -1, w_ = -1;
    vs_ = -1, hs_= -1;
    vc_ = -1, hc_= -1;
    vh_ = -1;
    hr_ = -1;

}


DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // Initilaize ul_, h_ and w_ and any other data members

    ul_ = upperleft;
    h_ = height;
    w_ = width;
    vs_ = -1, hs_= -1;
    vc_ = -1, hc_= -1;
    vh_ = -1;
    hr_ = -1;
}


DigitBlob::~DigitBlob()
{


}


void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features
    calc_symmetry();
    calc_centers_of_mass();
    calc_vert_lines();
    calc_horizontal_half();



    if(euler_ == -1){
        digit_ = '8';
    }
    else if(euler_ == 0){
        if(hs_ > 85 && vs_ > 85){
            digit_ = '0';
        }
        else if(vh_ > 0.8){
            digit_ = '4';
        }
        else if(hr_>0.99){
            digit_='9';
        }
        else{
            digit_='6';
        }
    }
    else{
        if(vs_>65 ){
            if(hc_>0.32 ){
                digit_ = '3';
            }
            else{
                digit_ = '1';
            }
        }
        else{
            if(vc_<0.6){
                digit_='7';
            }
            else if(vs_>57){
                digit_='2';
            }
            else{
                digit_='5';
            }
        }
    }
}


char DigitBlob::getClassification() const
{
    return digit_;
}


void DigitBlob::printClassificationResults() const
{
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << "Euler number is " << euler_ << endl;
    // TO DO: Add any other couts to print classification test data
    cout << "Symmetry: Horizontal=" << hs_ << "  Vertical=" << vs_ << endl;
    cout << "Centroid: Horizontal=" << hc_ << "  Vertical=" << vc_ << endl;
    cout << "Vertical Height = " << vh_ << endl;
    cout << "Pixel concentration Ratio: " << hr_ << endl;





    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}


const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}


int DigitBlob::getHeight() const
{
    return h_;
}


int DigitBlob::getWidth() const
{
    return w_;
}


bool DigitBlob::operator<(const DigitBlob& other)
{
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}


void DigitBlob::calc_euler_number()
{
    euler_ = (bq1_ - bq3_ - 2*bqd_) / 4;
}


void DigitBlob::calc_bit_quads()
{

    int count;
    for(int i=ul_.row-1; i<ul_.row+h_; i++){
        for(int j=ul_.col-1; j<ul_.col+w_; j++){
            count = 0;
            if(img_[i][j] == 0) count++;
            if(img_[i][j+1] == 0) count++;
            if(img_[i+1][j] == 0) count++;
            if(img_[i+1][j+1] == 0) count++;

            if(count == 0) bq0_++;
            if(count == 1) bq1_++;
            if(count == 2){
                if((img_[i][j] == 0 && img_[i+1][j+1] == 0)
                    || (img_[i][j+1] == 0 && img_[i+1][j] == 0))
                {
                    bqd_++;
                }
                else{
                    bq2_++;
                }
            }
            if(count == 3) bq3_++;
            if(count == 4) bq4_++;
        }
    }

}


void DigitBlob::calc_symmetry(){
    int match=0, looked=0;
    for(int i=ul_.row, j=(h_-1)+ul_.row; i<(h_/2)+ul_.row; i++, j--){
        for(int k=ul_.col; k<w_+ul_.col; k++){
            if(img_[i][k] == img_[j][k])match++;
            looked++;
        }
    }
    vs_ = ((double)match/looked)*100;

    match=0, looked=0;
    for(int i=ul_.row; i<h_+ul_.row; i++){
        for(int j=ul_.col, k=(w_-1)+ul_.col; j<(w_/2)+ul_.col; j++, k--){
            if(img_[i][j] == img_[i][k])match++;
            looked++;
        }
    }
    hs_ = ((double)match/looked)*100;
}

void DigitBlob::calc_centers_of_mass(){
    int sumi=0, sumj=0, count=0;
    for(int i=ul_.row; i<ul_.row + h_; i++){
        for(int j=ul_.col; j<ul_.col + w_; j++){
            if(img_[i][j] == 0){
                sumi += i-ul_.row;
                sumj += j-ul_.col;
                count++;
            }
        }
    }
    hc_ = ((double)sumj/count)/(h_-1);
    vc_ = ((double)sumi/count)/(w_-1);
}

void DigitBlob::calc_vert_lines(){
    int len = 0;
    int i, j;
    bool found = false;
    for(i=ul_.row+h_-1; i>=ul_.row; i--){
        for(j=ul_.col+w_-1; j>=ul_.col; j--){
            if(img_[i][j] == 0){
                found = true;
                break;
            }
        }
        if(found) break;
    }
    while(img_[i][j]==0){
        len++;
        j--;
    }
    j += len/2;
    j++;
    int ic = i;
    while(img_[i][j]==0){
        len++;
        i--;
    }
    vh_ = (((double)ic - i) / (h_-1));
}
void DigitBlob::calc_horizontal_half(){
    int top_sum = 0, bottom_sum=0, count=0;
    for(int i=ul_.row, j=(h_-1)+ul_.row; i<(h_/2)+ul_.row; i++, j--){
        for(int k=ul_.col; k<w_+ul_.col; k++){
            if(img_[i][k]==0)top_sum++;
            if(img_[j][k]==0)bottom_sum++;
            count++;
        }
    }

    hr_ = (double)top_sum/bottom_sum;
}
