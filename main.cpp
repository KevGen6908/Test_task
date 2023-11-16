
#include <iostream>
#include "ViterbiCodec.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

vector<int> bsc_chanel(vector<int>& input_bits, double error_prob)
{
    vector<int> output_bits;
    for (int bit : input_bits) {
        if ((rand() / static_cast<double>(RAND_MAX)) < error_prob) {
            output_bits.push_back(1 - bit);
        }
        else {
            output_bits.push_back(bit);
        }
    }
    return output_bits;
}

void write_to_csv(vector<double> porb, vector<double> arg)
{
    ofstream csv_file("csv_arg");
    if (!csv_file.is_open()) {
        cerr << "error open file" << endl;
        return;
    }
    else {
        cout << "file is open" << endl;
    }
    csv_file << "Error Probability,Decoder Output Error Probability" << endl;
    if (porb.size() != arg.size()) {
        cerr << "error size" << endl;
        return;
    }
    else {
        for (int i = 0; i < porb.size(); ++i) {
            csv_file << porb[i] << "," << arg[i] << endl;
        }
        csv_file.close();
        cout << "file is close" << endl;
    }
}

int main()
{
    cout << "---------------------------------Viterbi encoding and decoding--------------------------------------------------------" << endl;

    vector<int> poly = { 7, 5 };
    ViterbiCodec test_1(3, poly);
    string encode = test_1.encode("010111001010001");
    cout << "---Input---" << endl;
    cout << "0 1 0 1 1 1 0 0 1 0 1 0 0 0 1" << endl;
    cout << "---Encoded---" << endl;
    for (auto item : encode) {
        cout << item << " ";
    }
    cout << endl;

    string decode = test_1.decode(encode);
    cout << "---Decode---" << endl;
    for (auto item : decode) {
        cout << item << " ";
    }
    cout << endl;
    //-------------------------------------------simple test encoder and decoder--------------------------------------------

    srand(static_cast<unsigned>(time(0)));

    const int constraint = 3;
    std::vector<int> polynomials = { 7, 5 };
    const int num_trials = 100;

    ViterbiCodec viterbi(constraint, polynomials);

    std::cout << std::setw(20) << "Error Probability | " << std::setw(30) << "Decoder Output Error Probability" << std::endl;
    vector<double> avg_decoder_error_prob_v;
    vector<double> error_prob_v;

    for (double error_prob = 0.0; error_prob <= 0.5; error_prob += 0.01) {
        double total_decoder_errors = 0.0;

        for (int trial = 0; trial < num_trials; ++trial) {
            std::vector<int> input_data;
            string string_input_result;
            for (int i = 0; i < 100; ++i) {
                input_data.push_back(rand() % 2);
            }
            ostringstream oss;

            std::vector<int> channel_output = bsc_chanel(input_data, error_prob);

            for (const auto& element : channel_output) {
                oss << element;
            }
            string_input_result = oss.str();

            std::string encoded_bits = viterbi.encode(string_input_result);
            std::string decoded_bits = viterbi.decode(encoded_bits);

            int decoder_errors = 0;
            for (int i = 0; i < input_data.size(); ++i) {
                if (input_data[i] != decoded_bits[i] - '0') {
                    decoder_errors++;
                }
            }

            total_decoder_errors += static_cast<double>(decoder_errors) / input_data.size();
        }
        avg_decoder_error_prob_v.push_back(total_decoder_errors / num_trials);
        error_prob_v.push_back(error_prob);
    }
    write_to_csv(error_prob_v, avg_decoder_error_prob_v);
    return 0;
}


