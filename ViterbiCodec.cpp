//
// Created by kevgen on 11.11.23.
//
#include "ViterbiCodec.h"
#include <algorithm>
#include <cassert>
#include <limits>
#include <string>
#include <utility>
#include <vector>

ViterbiCodec::ViterbiCodec(int constrain, vector<int>& polynom_)
        : encoderLength(constrain)
        , polynom(polynom_)
{
    if (polynom.empty()) {
        return;
    }
    for (int i = 0; i < polynom.size(); ++i) {
        assert(polynom[i] > 0);
        assert(polynom[i] < (1 << encoderLength));
    }
    initOutput();
}

int bitReverse(int bits, int input)
{
    assert(input < (1 << bits));
    int output = 0;
    while (bits-- > 0) {
        output = (output << 1) + (input & 1);
        input >> 1;
    }
    return output;
}

void ViterbiCodec::initOutput()
{
    outputs.resize(1 << encoderLength);
    for (int i = 0; i < outputs.size(); ++i) {
        for (int j = 0; j < polynom.size(); ++j) {
            int polynomial = bitReverse(encoderLength, polynom[j]);
            int input = i;
            int output = 0;
            for (int k = 0; k < encoderLength; ++k) {
                output ^= (input & 1) & (polynomial & 1);
                polynomial >> 1;
                input >> 1;
            }
            outputs[i] += output ? "1" : "0";
        }
    }
}

string ViterbiCodec::encode(const string bits)
{
    string encoded;
    int state = 0;

    for (int i = 0; i < bits.size(); ++i) {
        char item = bits[i];
        assert(item == '0' || item == '1');
        int input = item - '0';
        encoded += outputs.at(state | (input << (encoderLength - 1)));
        state = (state >> 1) | (input << (encoderLength - 2));
    }

    for (int i = 0; i < encoderLength; ++i) {
        encoded += outputs.at(state | (0 << (encoderLength - 1)));
        state = (state >> 1) | (0 << (encoderLength - 2));
    }

    return encoded;
}

string ViterbiCodec::decode(const string bits)
{
    vector<vector<int> > trellis;
    vector<int> path_metric(1 << (encoderLength - 1), numeric_limits<int>::max());
    path_metric.front() = 0;

    for (int i = 0; i < bits.size(); i += polynom.size()) {
        string currentBit(bits, i, polynom.size());
        if (currentBit.size() < polynom.size())
            currentBit.append(string(polynom.size() - currentBit.size(), '0'));
        updatePath_Metrics(currentBit, &path_metric, &trellis);
    }

    string decoded;
    int state = min_element(path_metric.begin(), path_metric.end()) - path_metric.begin();
    for (int i = trellis.size() - 1; i >= 0; i--) {
        decoded += state >> (encoderLength - 2) ? '1' : '0';
        state = trellis[i][state];
    }
    reverse(decoded.begin(), decoded.end());
    return decoded.substr(0, decoded.size() - encoderLength + 1);
}

pair<int, int> ViterbiCodec::pathMetric(const string& bits, vector<int> previosPathMetrinc, int state)
{
    int nextState = (state & ((1 << (encoderLength - 2)) - 1)) << 1;
    int sourceState_1 = nextState | 0;
    int sourceState_2 = nextState | 1;

    int ppm_1 = previosPathMetrinc[sourceState_1];
    if (ppm_1 < numeric_limits<int>::max()) {
        ppm_1 = branchMetric(bits, sourceState_1, state);
    }
    int ppm_2 = previosPathMetrinc[sourceState_2];
    if (ppm_2 < numeric_limits<int>::max()) {
        ppm_2 = branchMetric(bits, sourceState_2, state);
    }

    if (ppm_1 <= ppm_2) {
        return make_pair(ppm_1, sourceState_1);
    }
    else {
        return make_pair(ppm_2, sourceState_2);
    }
}

void ViterbiCodec::updatePath_Metrics(const string& bits, vector<int>* path_metrics, vector<vector<int> >* trellis)
{
    vector<int> newPathMetrics(path_metrics->size());
    vector<int> trellisColumn(1 << (encoderLength - 1));

    for (int i = 0; i < path_metrics->size(); ++i) {
        pair<int, int> path = pathMetric(bits, *path_metrics, i);
        newPathMetrics[i] = path.first;
        trellisColumn[i] = path.second;
    }

    *path_metrics = newPathMetrics;
    trellis->push_back(trellisColumn);
}

int hammingDistance(const string& x, const string& y)
{
    assert(x.size() == y.size());
    int distance = 0;
    for (int i = 0; i < x.size(); ++i) {
        distance += x[i] != y[i];
    }
    return distance;
}

int ViterbiCodec::branchMetric(const string& bits, int sourse_state, int target_state)
{
    const string output = outputs.at(sourse_state | (target_state >> (encoderLength - 2) << (encoderLength - 1)));
    return hammingDistance(bits, output);
}
