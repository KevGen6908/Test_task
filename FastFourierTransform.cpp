//
// Created by kevgen on 09.11.23.
//

#include "FastFourierTransform.h"

void FastFourierTransform::fft(vector<complex<double>> &data, bool invert) {
    int size = data.size();
    if(size <= 1)
        return;

    vector<complex<double>> half1(size/2), half2(size/2);
    for (int i = 0, j = 0; i < size; i += 2,  ++j) {
        half1[j] = data[i];
        half2[j] = data[i + 1];
    }

    fft(half1, invert);
    fft(half2, invert);

    double angle = 2 * M_PI / size * (invert ? -1 : 1);
    complex<double> rootOfOne(1), rotateRoot(cos(angle), sin(angle));
    for (int i = 0; i < size / 2; ++i) {
        complex<double> currentMultiplication = rootOfOne * half2[i];
        data[i] = half1[i] + currentMultiplication;
        data[i + size / 2] = half1[i] - currentMultiplication;
        if(invert){
            data[i] /= 2;
            data[i + size / 2] /= 2;
        }
        rootOfOne *= rotateRoot;
    }
}

void FastFourierTransform::transform(vector<complex<double>>& data, int basis) {
    //fft(data, false);
    switch (basis) {
        case 2:
            fft(data, false);
            break;
        case 3:
            fft_base3(data, false);
            break;
        default:
            break;
    }
}

void FastFourierTransform::inverseTransform(vector<complex<double>> &data, int basis) {
    int size = data.size();

    switch (basis) {
        case 2:
            fft(data, true);

            // Исправлено: деление на size, а не size / 2
            for (int i = 0; i < size; ++i) {
                data[i] /= size;
            }
            break;
        case 3:
            fft_base3(data, true);

            //data.resize(size / 3);

            //for (int i = 0; i < size; ++i) {
            //    data[i] /= size / 3;
            //}
            break;
    }

}

static complex<double> omega(int n, int k, int N, bool invert) {
    double sign = invert ? 1.0 : -1.0;
    double angle = sign * 2.0 * M_PI * k * n / N;
    complex<double> result = polar(1.0, angle);
    cout << "omega(" << n << ", " << k << ", " << N << ", " << invert << "): " << result << endl;
    return result;
}



void FastFourierTransform::fft_base3(vector<complex<double>> &data, bool invert) {
    int size = data.size();
    if (size <= 1)
        return;

    vector<complex<double>> half1(size / 3), half2(size / 3), half3(size / 3);

    for (int i = 0, j = 0; i < size; i += 3, ++j) {
        half1[j] = data[i];
        half2[j] = data[i + 1];
        half3[j] = data[i + 2];
    }

    fft_base3(half1, invert);
    fft_base3(half2, invert);
    fft_base3(half3, invert);

    double angle = 2 * M_PI / size * (invert ? 1 : -1);
    complex<double> rootOfOne(1), rotateRoot(cos(angle), sin(angle));
    for (int i = 0; i < size / 3; ++i) {
        complex<double> currentRotation1 = rootOfOne * half2[i];
        complex<double> currentRotation2 = rootOfOne * half3[i] * rotateRoot;
        complex<double> rotate_1 = half1[i] + currentRotation1 + currentRotation2;
        complex<double> rotate_2 = half1[i] + currentRotation1 * omega(1, i, size / 3, invert)
                                   + currentRotation2 * omega(2, i, size / 3, invert);
        complex<double> rotate_3 = half1[i] + currentRotation1 * omega(2, i, size / 3, invert)
                                   + currentRotation2 * omega(1, i, size / 3, invert);

        data[i] = rotate_1;
        data[i + size / 3] = rotate_2;
        data[i + 2 * size / 3] = rotate_3;

        if (invert) {
            data[i] /= 3;
            data[i + size / 3] /= 3;
            data[i + 2 * size / 3] /= 3;
        }

        rootOfOne *= rotateRoot;
    }
}


