#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include "Trie.cpp"
#include "BST.cpp"

using namespace std;
using namespace std::chrono;

string randomWord(int length) {
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(0, 25);

    string s;
    for (int i = 0; i < length; i++) {
        s += alphabet[dist(gen)];
    }
    return s;
}

int randomInt(int minVal, int maxVal) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(minVal, maxVal);
    return dist(gen);
}

int main() {
    const int N = 10000;
    vector<string> words;
    vector<int> numbers;

    for (int i = 0; i < N; i++) {
        words.push_back(randomWord(5 + (i % 5)));
        numbers.push_back(randomInt(1, 100000));
    }

    Trie trie;
    BST bst;

    auto start = high_resolution_clock::now();
    for (auto &w : words) trie.insert(w);
    auto end = high_resolution_clock::now();
    cout << "Trie insertion time: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (auto &n : numbers) bst.insert(n);
    end = high_resolution_clock::now();
    cout << "BST insertion time: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) trie.search(words[i]);
    end = high_resolution_clock::now();
    cout << "Trie search time (1000 queries): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) bst.search(numbers[i]);
    end = high_resolution_clock::now();
    cout << "BST search time (1000 queries): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (int i = 0; i < 500; i++) trie.remove(words[i]);
    end = high_resolution_clock::now();
    cout << "Trie delete time (500 words): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    for (int i = 0; i < 500; i++) bst.remove(numbers[i]);
    end = high_resolution_clock::now();
    cout << "BST delete time (500 numbers): "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    return 0;
}

