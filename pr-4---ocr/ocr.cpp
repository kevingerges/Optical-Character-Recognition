#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <cstdlib>
#include "numimg.h"
#include "bmplib.h"
#include "bigint.h"

using namespace std;

int main(int argc, char *argv[])
{
  if(argc < 3){
    cout << "Usage ./ocr img1_filename img2_filename debug-level level-option" << endl;
    return 1;
  }
  int debug = 0;
  if(argc >= 4) {
    debug = atoi(argv[3]);
  }

  // ===================
 // Fill in the arguments to the constructors below
  NumImg img1(argv[1]);
  NumImg img2(argv[2]);

  // ===================
   // call findAndCreateDigitBlobs on each img 
  img1.findAndCreateDigitBlobs();
  img2.findAndCreateDigitBlobs();


  // ===================
  if(debug == 1) {
    img1.printBoundingBoxes();
  }
  else if(debug == 2) {
    img2.printBoundingBoxes();
  }
  else if(debug == 3) {
    if(argc < 5){
      cout << "Please provide an extra argument of the filename to save the image." << endl;
    }
    else {
      img1.drawBoundingBoxesAndSave(argv[4]);
    }
  }
  else if(debug == 4) {
    if(argc < 5){
      cout << "Please provide an extra argument of the filename to save the image." << endl;
    }
    else {
      img2.drawBoundingBoxesAndSave(argv[4]);
    }
  }
  else if(debug == 5 || debug == 7) {
    // pass true if debug is 5 and false otherwise (i.e. 7)
    string num = img1.classify( debug == 5 );
    cout << "Image 1 digit string: " << num << endl;
  }
  else if(debug == 6 || debug == 8) {
    // pass true if debug is 6 and false otherwise (i.e. 8)
    string num = img2.classify( debug == 6 );
    cout << "Image 2 digit string: " << num << endl;
  }
  else {

    string str1 = img1.classify(false);
    string str2 = img2.classify(false);
    
    BigInt num1(str1);
    BigInt num2(str2);
    BigInt sum = num1 + num2;
    cout << num1.to_string() << " + " << num2.to_string() 
          << " = " << sum.to_string() << endl;
    if(num1 < num2){
      BigInt diff = num2 - num1;
      cout << num2.to_string() << " - " << num1.to_string() 
            << " = " << diff.to_string() << endl;
    }
    else {
      BigInt diff = num1 - num2;
      cout << num1.to_string() << " - " << num2.to_string() 
            << " = " << diff.to_string() << endl;
    }
  }

  return 0;
}

