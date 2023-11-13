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
class ViterbiCodec {
public:
    ViterbiCodec(int constrain, vector<int>& polynom);
    int getEncLen() const { return encoderLength; }
    const vector<int>& getPolynom() const { return polynom; }
    string encode(const string bits);
    string decode(const string bits);

private:
    const int encoderLength;
    const vector<int> polynom;
    vector<string> outputs;
    void initOutput();

    void updatePath_Metrics(const string& bits, vector<int>* path_metrics, vector<vector<int> >* trellis);
    int branchMetric(const string& bits, int sourse_state, int target_state);
    pair<int, int> pathMetric(const string& bits, vector<int> previosPathMetrinc, int state);
};

#endif //TESTCASE_VITERBICODEC_H