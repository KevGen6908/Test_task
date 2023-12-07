//
// Created by kevgen on 11.11.23.
//

#ifndef TESTCASE_VITERBICODEC_H
#define TESTCASE_VITERBICODEC_H
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
class Viterbi {
public:

    Viterbi(int constraint, const std::vector<int>& polynomials);
    std::string encode(const std::string& bits) const;
    std::string decode(const std::string& bits) const;
    int constraint() const { return constraint_; }
    const std::vector<int>& polynomials() const { return polynomials_; }
    std::string introduce_channel_errors(const std::string& bits, double error_prob);

private:
    typedef std::vector<std::vector<int> > Trellis;
    int num_parity_bits() const;
    void initializeOutputs();
    int nextState(int current_state, int input) const;
    std::string output(int current_state, int input) const;
    int branchMetric(const std::string& bits, int source_state, int target_state) const;
    std::pair<int, int> pathMetric(const std::string& bits, const std::vector<int>& prev_path_metrics, int state) const;
    void updatePathMetrics(const std::string& bits, std::vector<int>* path_metrics, Trellis* trellis) const;
    const int constraint_;
    const std::vector<int> polynomials_;
    std::vector<std::string> outputs_;
};

int reverseBits(int num_bits, int input);

#endif //TESTCASE_VITERBICODEC_H