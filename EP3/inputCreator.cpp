#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

std::string generateSequence(int length) {
    std::string sequence;
    static const char nucleotides[] = "ACGT";
    static const int numNucleotides = sizeof(nucleotides) - 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, numNucleotides - 1);

    for (int i = 0; i < length; ++i) {
        sequence += nucleotides[dis(gen)];
    }

    return sequence;
}

std::vector<std::string> splitSequence(const std::string& sequence, int fragmentLength, int minOverlap, int maxOverlap) {
    std::vector<std::string> fragments;

    int start = 0;
    while (start + fragmentLength <= sequence.length()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(minOverlap, maxOverlap);

        int overlap = dis(gen);
        std::string fragment = sequence.substr(start, fragmentLength);
        fragments.push_back(fragment);
        start += fragmentLength - overlap;
    }

    return fragments;
}

void saveFragmentsToFile(const std::string& filename, const std::vector<std::string>& fragments) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << fragments.size() << std::endl;
        for (const auto& fragment : fragments) {
            file << fragment << std::endl;
        }
        file.close();
        std::cout << "Fragments saved to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

int main() {
    int sequenceLength = 30;
    int desiredNumFragments = 16;
    int minFragmentLength = 9;
    int maxFragmentLength = 11;
    int minOverlap = 2;
    int maxOverlap = 5;
    std::string filename = "dna5.txt";

    std::string sequence = generateSequence(sequenceLength);
    std::cout<<sequence<<std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> lengthDis(minFragmentLength, maxFragmentLength);
    std::vector<std::string> fragments;

    int generatedNumFragments = 0;
    while (generatedNumFragments < desiredNumFragments) {
        int fragmentLength = lengthDis(gen);
        std::vector<std::string> tempFragments = splitSequence(sequence, fragmentLength, minOverlap, maxOverlap);
        fragments.insert(fragments.end(), tempFragments.begin(), tempFragments.end());
        generatedNumFragments += tempFragments.size();
    }

    fragments.resize(desiredNumFragments);

    saveFragmentsToFile(filename, fragments);

    return 0;
}

