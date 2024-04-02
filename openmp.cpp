#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <chrono>
#include <algorithm>

using namespace std;
const long long MOD = 1e9 + 7;
const long long p = 31;
vector<long long> step;

// Функция для вычисления хеша строки (просто для примера)
long long hashString(const string& str) {
    long long hash = 0;
    for (int i = 0; i < str.length(); ++i)
        hash = (hash + ((str[i]-'a'+1)*step[i] % MOD)) % MOD;

    return hash;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <num_threads> <input_file>" << std::endl;
        return 1;
    }

    int numThreads = std::atoi(argv[1]);
    string filename = argv[2]; 

    string line;
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open input file: " << filename << "\n";
        return 1;
    }
    getline(file, line);
    auto start = chrono::high_resolution_clock::now();
    
    int sub_size =  (line.length() + numThreads - 1)/ numThreads;

    step.resize(sub_size + 1);
    step[0] = 1;
        for (int i = 1; i <= sub_size; ++i)
            step[i] = (step[i-1]*p) % MOD;

    
    vector<long long> hash(numThreads);

#pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < numThreads; ++i) {
        int start = i * sub_size;
        int end = min(start + sub_size, (int)line.length());

        string substring = line.substr(start, max(0, end - start));

         hash[i] = hashString(substring);
    }

    long long result = 0;
    long long _pow = 1;
    for (int i = 0; i < numThreads; ++i){
        cout << "[" << i << " procces]:: " << hash[i] << "\n";
        result = (result + (hash[i]*_pow % MOD)) % MOD;
        _pow = (_pow * step[sub_size]) % MOD;
    } 
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "RESULT :: " << result << "\n";
    cout << "DURATION :: " << duration.count() << " ms\n";

    return 0;
}
