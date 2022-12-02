// Copy in your bigint.cpp (overwrite this file)

#include <iostream>
#include <algorithm>
#include <cmath>
#include "bigint.h"

using namespace std;

BigInt::BigInt(string s, int b)
{
    unsigned int count = 0;
    for(count=0; s[count] == '0' && count<s.length()-1; count++);
    s.erase(0, count);
    base = b;
    if(base == 10){
        for(unsigned int i = 0; i<s.size(); i++){
            char c = s[i];
            int x = static_cast<int>((c) - '0');
            sped.push_back(x);
        }
    }
}

// to string 
std::string BigInt::to_string() const
{
    string s = "";
    if(base == 10){
        for(unsigned int i=0; i<sped.size(); i++)
        {
            char c = (char)(sped[i]+'0');
            s += c;
        }
    }
    return s;

}

int BigInt::digitno() const{
    return sped.size();
}

std::vector<int> BigInt::get_vector() const{
    return vector<int>(sped);
}

BigInt BigInt::operator+(BigInt const &b) {
    int a_size = digitno();
    int b_size =  b.digitno();

    std::vector<int> a_vec = get_vector();
    std::vector<int> b_vec = b.get_vector();

    if(a_size < b_size){
        for(int i=0; i<b_size-a_size; i++)
            a_vec.insert(a_vec.begin(), 0);
    }
    else{
        for(int i=0; i<a_size-b_size; i++)
            b_vec.insert(b_vec.begin(), 0);
    }

    std::string result_string = "";

    int carry = 0;

    for(int i=a_vec.size()-1; i>=0; i--){
        int digit_sum =  a_vec[i] + b_vec[i] + carry;
        carry = digit_sum / 10;
        int digit = digit_sum % 10;
        result_string += std::to_string(digit);
    }

    if(carry){
        result_string+=std::to_string(carry);
    }
    reverse(result_string.begin(), result_string.end());

    BigInt result(result_string);
    return result;
}


BigInt BigInt::operator-(BigInt const &b){

    int a_size = digitno();
    int b_size =  b.digitno();

    std::vector<int> a_vec = get_vector();
    std::vector<int> b_vec = b.get_vector();

    if(a_size < b_size){
        for(int i=0; i<b_size-a_size; i++)
            a_vec.insert(a_vec.begin(), 0);
    }
    else{
        for(int i=0; i<a_size-b_size; i++)
            b_vec.insert(b_vec.begin(), 0);
    }
    std::string result_string = "";

    if(*this<b){
        for(int i=b_vec.size()-1; i>=0; i--){
            if((b_vec[i] - a_vec[i]) < 0){
                b_vec[i] += 10;
                b_vec[i-1] -= 1;
            }
            int sub_digit = b_vec[i] - a_vec[i];
            result_string += std::to_string(sub_digit);
        }
    }
    else{
        for(int i=a_vec.size()-1; i>=0; i--){
            if((a_vec[i] - b_vec[i]) < 0){
                a_vec[i] += 10;
                a_vec[i-1] -= 1;
            }
            int sub_digit = a_vec[i] - b_vec[i];
            result_string += std::to_string(sub_digit);
        }
    }
    reverse(result_string.begin(), result_string.end());
    int count = 0;
    for(count=0; result_string[count] == '0'; count++);
    result_string.erase(0, count);

    if(result_string.length() == 0){
        result_string += '0';
    }

    BigInt result(result_string);
    return result;
}

bool BigInt::operator<(BigInt const &b){
    int a_size = digitno();
    int b_size =  b.digitno();

    std::vector<int> a_vec = get_vector();
    std::vector<int> b_vec = b.get_vector();

    if(a_size < b_size){
        for(int i=0; i<b_size-a_size; i++)
            a_vec.insert(a_vec.begin(), 0);
    }
    else{
        for(int i=0; i<a_size-b_size; i++)
            b_vec.insert(b_vec.begin(), 0);
    }

    for(int i=0; i<a_size; i++){
        if(a_vec[i] < b_vec[i]) return true;
        if(a_vec[i] > b_vec[i]) break;
    }
    return false;
} 
