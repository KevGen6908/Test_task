//
// Created by kevgen on 09.11.23.
//

#ifndef TESTCASE_FASTFOURIERTRANSFORM_H
#define TESTCASE_FASTFOURIERTRANSFORM_H

#include <complex>
#include <vector>
#include "cmath"
#include "iostream"

using namespace std;

class FastFourierTransform {
public:
    static void transform(vector<complex<double>>& data, int basis);
    static void inverseTransform(vector<complex<double>>& data, int basis );

private:
    static void fft(vector<complex<double>>& data, bool invert);
    //static complex<double> omega(int n, int k, int N, bool invert);
    static void fft_base3(vector<complex<double>>& data, bool invert);
};


#endif //TESTCASE_FASTFOURIERTRANSFORM_H
