#include <iostream>
#include <algorithm>
#include <numeric>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int similarity(string &s1, string &s2) {
	if (s1.size() == 0 || s2.size() == 0 || s1.size() != s2.size()) {
		return 0;
	}
	int NumSimLetters = 0;
	for (unsigned int i = 0; i < s2.size(); ++i) {
		if (s1[i] == s2[i]) { NumSimLetters++; }
	}
	return NumSimLetters*100 / s1.size();
}

void readLines(vector<int> &means, string filename_1, string filename_2, int init, int end) {
	int numLine = 0;
	ifstream file_1(filename_1);
	ifstream file_2(filename_2);
	string word_1;
	string word_2;
	
	while (numLine != init && getline(file_1, word_1, '\n') && getline(file_2, word_2, '\n')) {
		++numLine;
	}

	for (int i = init; i < end; i++) {
		getline(file_1, word_1, '\n');
		getline(file_2, word_2, '\n');
		means.push_back(similarity(word_1, word_2));
	}
}

int numLines(string& filename) {
	string text;
	int num = 0;
	ifstream file(filename);

	if (!file.is_open()) {
		std::cout << "File is not found" << std::endl;
		return 0;
	}
	while (getline(file, text, '\n')) {
		num++;
	}
	return num;
}

int main() {
    
	int numThreads = 6;
	vector <thread> th;
	vector <int> means;

	string filename_1 = "AY274119.txt";
	string filename_2 = "AY278488.2.txt";

	int numLinesFile_1 = numLines(filename_1);
	int numLinesFile_2 = numLines(filename_2);
	int numLines = numLinesFile_1 <= numLinesFile_2 ? numLinesFile_1 : numLinesFile_2;

	int param = 0;
	int separation = numLines / numThreads;

	for (int i = 0; i < numThreads; i++) {
		int temp = param + separation;
		th.emplace_back(readLines, ref(means), filename_1, filename_2, param, temp);
		param = temp;
		++param;
	}

	for (int i = 0; i < numThreads; i++) {
		th[i].join();
	}
	float mean = std::accumulate(begin(means), end(means), 0.0) / means.size();
	std::cout << "The two genes are " << mean << "% alike";

	return 0;
}
