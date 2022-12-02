#ifndef DIGITBLOB_H
#define DIGITBLOB_H

#include <cstdint> // for uint8_t
#include <vector>

/**
 * @brief Models a row/column location and provides an
 *        initializing constructor and comparison ability
 *
 *  
 */
struct Location {
    int row;
    int col;
    // Default constructor
    Location() {
        row = -1;
        col = -1;
    }
    // Initializing constructor
    Location(int r, int c) {
        row = r;
        col = c;
    }
    /**
     * @brief Comparison operator to allow sorting of Locations with std::sort
     * Should return true if the column of this Location is less than
     *  other's column and then check if this Location's row is less than
     *  the other Location's row only if the columns are 
     *  equal.
     *
     * @param other 
     * @return true if this Location is "less-than" other (using criteria above)
     * @return false , otherwise
     */
    bool operator<(const Location& other) const;

};


/**
 * @brief Models the area of pixels representing a digit in the image
 *        and the operations necessary to classify/identify the digit.
 *
 */
class DigitBlob {
public:
    /**
     * @brief Construct a new Digit Blob object
     *
     */
    DigitBlob();

    /**
     * @brief Construct a new Digit Blob object using
     *        given the image and the bounding box
     *
     */
    DigitBlob(uint8_t** img, Location upperleft, int height, int width);

    /**
     * @brief Destroy the Digit Blob object
     *
     */
    ~DigitBlob();

    /**
     * @brief Runs classification code to identify the represented digit
     *
     */
    void classify();

    /**
     * @brief Get the digit character that was identified from classify()
     *
     * @return char
     */
    char getClassification() const;

    /**
     * @brief Optional function that you can use for debugging to
     *        print the results of classification tests
     *
     */
    void printClassificationResults() const;

    /**
     * @brief Accessors
     *
     */
    const Location& getUpperLeft() const;
    int getHeight() const;
    int getWidth() const;

    /**
     * @brief Comparison operator to allow for sorting of DigitBlobs
     *        using the upper-left corner of their bounding box
     *
     */
    bool operator<(const DigitBlob& other);

private:
    void calc_bit_quads();
    void calc_euler_number();
    //===========================================================
    // Define appropriate, private member functions for various
    // classificaiton tests
    void calc_centers_of_mass();
    void calc_symmetry();
    void calc_aspect_ratio();
    void calc_thirds();
    void calc_corners();
    void calc_vert_lines();
    void calc_horizontal_half();

    //===========================================================
    // Data members

    // 2D pixel array pointer to allow accessing of the pixel data
    //  (Should only be accessing pixels within the bounding box of this digit)
    uint8_t** img_;
    // upper left location
    Location ul_;
    // height and width
    int h_;
    int w_;
    // holds the recognized digit character
    char digit_;

    int bq0_,bq1_,bq2_,bq3_,bq4_,bqd_;
    int euler_;
    //===========================================================
    // Add more data member(s) here to store the results of
    // classification tests or other info you deem necessary
    
    double vs_, hs_;
    double vc_, hc_;
    double vh_;
    double hr_;


};
#endif
