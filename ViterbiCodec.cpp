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


int hammingDistance(const std::string& x, const std::string& y) {
    assert(x.size() == y.size());
    int distance = 0;
    for (int i = 0; i < x.size(); i++) {
        distance += x[i] != y[i];
    }
    return distance;
}

int reverseBits(int num_bits, int input) {
    assert(input < (1 << num_bits));
    int output = 0;
    while (num_bits-- > 0) {
        output = (output << 1) + (input & 1);
        input >>= 1;
    }
    return output;
}

Viterbi::Viterbi(int constraint, const std::vector<int>& polynomials) : constraint_(constraint), polynomials_(polynomials) {
    assert(!polynomials_.empty());
    for (int i = 0; i < polynomials_.size(); i++) {
        assert(polynomials_[i] > 0);
        assert(polynomials_[i] < (1 << constraint_));
    }
    initializeOutputs();
}

int Viterbi::num_parity_bits() const {
    return polynomials_.size();
}

int Viterbi::nextState(int current_state, int input) const {
    return (current_state >> 1) | (input << (constraint_ - 2));
}

std::string Viterbi::output(int current_state, int input) const {
    return outputs_.at(current_state | (input << (constraint_ - 1)));
}

std::string Viterbi::encode(const std::string& bits) const {
    std::string encoded;
    int state = 0;

    for (int i = 0; i < bits.size(); i++) {
        char symbol = bits[i];
        assert(symbol == '0' || symbol == '1');
        int input = symbol - '0';
        encoded += output(state, input);
        state = nextState(state, input);
    }

    for (int i = 0; i < constraint_ - 1; i++) {
        encoded += output(state, 0);
        state = nextState(state, 0);
    }

    return encoded;
}

void Viterbi::initializeOutputs() {
    outputs_.resize(1 << constraint_);
    for (int i = 0; i < outputs_.size(); i++) {
        for (int j = 0; j < num_parity_bits(); j++) {
            int polynomial = reverseBits(constraint_, polynomials_[j]);
            int input = i;
            int output = 0;
            for (int k = 0; k < constraint_; k++) {
                output ^= (input & 1) & (polynomial & 1);
                polynomial >>= 1;
                input >>= 1;
            }
            outputs_[i] += output ? "1" : "0";
        }
    }
}

int Viterbi::branchMetric(const std::string& bits, int source_state, int target_state) const {
    assert(bits.size() == num_parity_bits());
    assert((target_state & ((1 << (constraint_ - 2)) - 1)) == source_state >> 1);
    const std::string result = output(source_state, target_state >> (constraint_ - 2));
    return hammingDistance(bits, result);
}

std::pair<int, int>
Viterbi::pathMetric( const std::string& bits, const std::vector<int>& prev_path_metrics, int state) const {
    int current_state = (state & ((1 << (constraint_ - 2)) - 1)) << 1;
    int source_state1 = current_state | 0;
    int source_state2 = current_state | 1;

    int path_metrics1 = prev_path_metrics[source_state1];
    if (path_metrics1 < std::numeric_limits<int>::max()) {
        path_metrics1 += branchMetric(bits, source_state1, state);
    }
    int path_metrics2 = prev_path_metrics[source_state2];
    if (path_metrics2 < std::numeric_limits<int>::max()) {
        path_metrics2 += branchMetric(bits, source_state2, state);
    }


    if (path_metrics1 <= path_metrics2) {
        return std::make_pair(path_metrics1, source_state1);
    } else {
        return std::make_pair(path_metrics2, source_state2);
    }
}

void Viterbi::updatePathMetrics(const std::string& bits, std::vector<int>* path_metrics, Trellis* trellis) const {
    std::vector<int> new_path_metrics(path_metrics->size());
    std::vector<int> new_trellis_column(1 << (constraint_ - 1));
    for (int i = 0; i < path_metrics->size(); i++) {
        std::pair<int, int> p = pathMetric(bits, *path_metrics, i);
        new_path_metrics[i] = p.first;
        new_trellis_column[i] = p.second;
    }

    *path_metrics = new_path_metrics;
    trellis->push_back(new_trellis_column);
}

std::string Viterbi::decode(const std::string& bits) const {
    Trellis trellis;
    std::vector<int> path_metrics(1 << (constraint_ - 1),std::numeric_limits<int>::max());
    path_metrics.front() = 0;
    for (int i = 0; i < bits.size(); i += num_parity_bits()) {
        std::string current_bits(bits, i, num_parity_bits());
        if (current_bits.size() < num_parity_bits()) {
            current_bits.append(std::string(num_parity_bits() - current_bits.size(), '0'));
        }
        updatePathMetrics(current_bits, &path_metrics, &trellis);
    }

    std::string decoded;
    int state = std::min_element(path_metrics.begin(), path_metrics.end()) -
                path_metrics.begin();
    for (int i = trellis.size() - 1; i >= 0; i--) {
        decoded += state >> (constraint_ - 2) ? "1" : "0";
        state = trellis[i][state];
    }
    std::reverse(decoded.begin(), decoded.end());

    return decoded.substr(0, decoded.size() - constraint_ + 1);
}

std::string Viterbi::introduce_channel_errors(const std::string& bits, double error_prob)
{
    std::string transmitted_bits;
    for (char bit : bits) {
        if (rand() / (RAND_MAX + 1.0) < error_prob) {
            transmitted_bits += (bit == '0') ? '1' : '0';
        } else {
            transmitted_bits += bit;
        }
    }
    return transmitted_bits;
}