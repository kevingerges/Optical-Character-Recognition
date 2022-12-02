#ifndef NUMIMG_H
#define NUMIMG_H

#include "bmplib.h"

#include "digitblob.h"
#include <vector>
#include <string>


/**
 * @brief Reads, owns, and manages the pixel array representing an 
 *        8-bit grayscale image with digits to be recognized as a 
 *        number.
 * 
 */
class NumImg {
public:
    /**
     * @brief Construct a new Num Img object by reading an 8-bit
     *         grayscale BMP image.
     * 
     * @param bmp_filename C-string of the filename to read
     */
    NumImg(const char* bmp_filename);
    
    /**
     * @brief Destructor
     * 
     */
    ~NumImg();

    /**
     * @brief Finds blobs of pixels representing a digit and
     *        creates Digit objects from them. 
     *        
     *        Post-condition:  DigitBlobs should be stored 
     *        in sorted order of the upper-left location of their
     *        bounding box. See sortDigitBlobs() helper function 
     *        provided to you.
     * 
     * @return count of how many digit blobs were identified 
     *         (remember size_t is just an unsigned integer type)
     */
    size_t findAndCreateDigitBlobs();

    /**
     * @brief Classifies/identifies the corresponding digit 0-9 from
     *        each blob of pixels and returns the string of digits found
     *        in ascending order based on the upper-left coordinates of
     *        their bounding box. 
     * @param[in] withDebug true if classification test results should 
     *                           be printed
     * @return string of classified digit characters
     */
    std::string classify(bool withDebug);

    /**
     * @brief Print bounding box information for all DigitBlobs in
     *        a specific format used for debugging/grading.
     * 
     * @param filename 
     */
    void printBoundingBoxes() const;

    /**
     * @brief Draws gray bounding boxes around the digits and saves
     *        to an output .bmp using the filename input argument
     * 
     * @param filename 
     */
    void drawBoundingBoxesAndSave(const char* filename);

    /**
     * @brief Accessor to get a pointer to the i-th DigitBlob (again  
     *        using the upper-left coordinate of their bounding boxes as the
     *        ordering constraint (i.e. first by their top row, 
     *        then by left-most column)
     *
     * @param[in] i index of the DigitBlob to get
     * @return DigitBlob 
     */
    const DigitBlob& getDigitBlob(size_t i) const;

    /**
     * @brief Accessor to get how many DigitBlobs were found
     * 
     * @return size_t 
     */
    size_t numDigitBlobs() const;


private:
    //===========================================================
    //  Private helper functions

    /**
     * @brief Sorts the DigitBlobs in the blobs_ vector in ascending order
     *         of the upper-left Location of their bounding box.
     *         (i.e. first by their top row, then by left-most column)
     */
    void sortDigitBlobs();

    /**
     * @brief Create a Digit Blob object starting with one of its pixels and
     *         then using an 8-connected neighbor BFS to find all pixels that
     *         are part of this digit.
     * 
     * @param explored 2D explored array to avoid to help avoid searching 
     *                  searching pixels we've already explored
     * @param pr Row of the first pixel of a new digit to explre
     * @param pc Col of the first pixel of a new digt to explore
     * @return DigitBlob corresponding to this newly found digit
     */
    DigitBlob createDigitBlob(int pr, int pc);

    //===========================================================
    //  Add other private functions as necessary

    bool **expd;

    //===========================================================
    //  Data members

    /// Pointer to the 2D dynamically allocated array of pixels
    uint8_t** img_;
    /// Height and width of input image (not always 256x256)
    int h_;
    int w_;
    /// Vector to store the DigitBlobs we find in the image
    std::vector<DigitBlob> blobs_;

    //===========================================================
    // Add appropriate data member(s) here to store the Digit objects
    // or any other data that needs to be tracked
};
#endif
