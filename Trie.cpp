#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool isEndOfWord;
    int frequency;

    TrieNode() {
        isEndOfWord = false;
        frequency = 0;
        for (int i = 0; i < 26; i++)
            children[i] = nullptr;
    }
};

class Trie {
private:
    TrieNode* root;

    void collectWords(TrieNode* node, string current, vector<pair<string, int>> &result) {
        if (!node) return;
        if (node->isEndOfWord)
            result.push_back({current, node->frequency});

        for (int i = 0; i < 26; i++) {
            if (node->children[i])
                collectWords(node->children[i], current + char('a' + i), result);
        }
    }

    void searchRecursive(TrieNode* node, const string &word, string current,
                         vector<int> prevRow, int maxEdits,
                         vector<pair<string, int>> &results) {
        int n = word.length();
        vector<int> currRow(n + 1);
        currRow[0] = prevRow[0] + 1;

        for (int i = 1; i <= n; i++) {
            int insertCost = currRow[i - 1] + 1;
            int deleteCost = prevRow[i] + 1;
            int replaceCost = prevRow[i - 1] + (word[i - 1] != current.back());
            currRow[i] = min(insertCost, min(deleteCost, replaceCost));
        }

        if (node->isEndOfWord && currRow[n] <= maxEdits)
            results.push_back({current, node->frequency});

        if (*min_element(currRow.begin(), currRow.end()) <= maxEdits) {
            for (int i = 0; i < 26; i++) {
                if (node->children[i]) {
                    searchRecursive(node->children[i], word, current + char('a' + i),
                                    currRow, maxEdits, results);
                }
            }
        }
    }

    // Recursive helper for delete (fixed with size_t depth)
    bool deleteWord(TrieNode* node, const string &word, size_t depth) {
        if (!node) return false;

        if (depth == word.size()) {
            if (!node->isEndOfWord) return false; // word not found
            node->isEndOfWord = false;
            node->frequency = 0;

            // check if node has no children
            for (int i = 0; i < 26; i++) {
                if (node->children[i]) return false;
            }
            return true; // signal to delete this node
        }

        int index = word[depth] - 'a';
        if (deleteWord(node->children[index], word, depth + 1)) {
            delete node->children[index];
            node->children[index] = nullptr;

            if (!node->isEndOfWord) {
                for (int i = 0; i < 26; i++) {
                    if (node->children[i]) return false;
                }
                return true;
            }
        }
        return false;
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string &word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (!current->children[index])
                current->children[index] = new TrieNode();
            current = current->children[index];
        }
        current->isEndOfWord = true;
        current->frequency++;
    }

    bool search(const string &word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (!current->children[index])
                return false;
            current = current->children[index];
        }
        return current->isEndOfWord;
    }

    int getFrequency(const string &word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (!current->children[index])
                return 0;
            current = current->children[index];
        }
        return current->isEndOfWord ? current->frequency : 0;
    }

    vector<pair<string, int>> getSuggestions(const string &prefix) {
        TrieNode* current = root;
        for (char ch : prefix) {
            int index = ch - 'a';
            if (!current->children[index])
                return {};
            current = current->children[index];
        }

        vector<pair<string, int>> result;
        collectWords(current, prefix, result);

        sort(result.begin(), result.end(),
             [](const pair<string,int> &a, const pair<string,int> &b){ return a.second > b.second; });

        return result;
    }

    vector<pair<string, int>> getCorrections(const string &word, int maxEdits) {
        vector<pair<string, int>> results;
        vector<int> initialRow(word.length() + 1);
        iota(initialRow.begin(), initialRow.end(), 0);

        for (int i = 0; i < 26; i++) {
            if (root->children[i]) {
                string current(1, char('a' + i));
                searchRecursive(root->children[i], word, current, initialRow, maxEdits, results);
            }
        }

        sort(results.begin(), results.end(),
             [](const pair<string,int> &a, const pair<string,int> &b){ return a.second > b.second; });

        return results;
    }

    void remove(const string &word) {
        deleteWord(root, word, 0);
    }
};


