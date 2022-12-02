// Copy in your bigint.h (overwrite this file)
#ifndef BIGINT_H
#define BIGINT_H
#include <string>
#include <vector>

class BigInt {
public:
    BigInt(std::string s, int base=10); // convert string to BigInt
    std::string to_string() const; // get string representation
    BigInt operator+(BigInt const &b); // add another BigInt to this one
    BigInt operator-(BigInt const &b); // add another BigInt to this one
    bool operator<(BigInt const &b);
    int digitno() const;
    std::vector<int> get_vector() const;
    int base;
private:
    std::vector<int> sped;
    // whatever you need

};
#endif



// vector<T>() // default constructor: new empty vector
// void vector.push_back(T t) // add element to end of the list
// T vector[int i] // get the element at position i
// size_t vector.size() // current size (size_t is usually a 64-bit unsigned int)/  and then add the appropriate code
