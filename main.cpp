
#include <iostream>
#include "ViterbiCodec.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <random>


using namespace std;

int main()
{
    cout << "---------------------------------Viterbi encoding and decoding simple test---------------------------------" << endl;

    vector<int> poly = { 7, 5 };
    Viterbi test_1(3, poly);
    string input = "010111001010001";
    string encode = test_1.encode(input); // 010111001010001
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

    cout << "---Check---" << endl;
    if(decode == input)
        cout << "Successful decoding" << endl;
    cout << endl;
    //-------------------------------------------simple test encoder and decoder----------------------------------------

    std::random_device rd;
    std::mt19937 gen(rd());
    srand(static_cast<unsigned>(time(0)));

    const int constraint = 3;
    std::vector<int> polynomials = { 7, 5 };
    const int num_trials = 500;

    Viterbi viterbi(constraint, polynomials);
    std::vector<double> channel_error_probabilities;
    for (double prob = 0; prob <= 0.25 ; prob += 0.005) {
        channel_error_probabilities.push_back(prob);
    }

    std::ofstream output_file("simulation_result.txt");
    if(!output_file.is_open()){
        std::cerr << "Unable to open output file." << std::endl;
        return 1;
    }

    string message;
    int string_size = 50;
    for (double chanel_error_prob: channel_error_probabilities) {
        for (int i = 0; i < string_size; ++i) {
            message += (gen() % 2) ? '1' : '0';
        }

        int successful_decoded = 0;
        for (int i = 0; i < num_trials; i++) {
            std::string encoded_message = viterbi.encode(message);
            std::string received_bits = viterbi.introduce_channel_errors(encoded_message, chanel_error_prob);
            std::string decoded_message = viterbi.decode(received_bits);
            if(message == decoded_message){
                successful_decoded++;
            }
        }
        double result_in_chanel = (double) (num_trials - successful_decoded) / num_trials;
        output_file << result_in_chanel <<  " " << chanel_error_prob << std::endl;
        message = "";
    }

    output_file.close();

    return 0;
}


