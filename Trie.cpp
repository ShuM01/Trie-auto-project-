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
            currRow[i] = min({insertCost, deleteCost, replaceCost});
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
             [](auto &a, auto &b){ return a.second > b.second; });

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
             [](auto &a, auto &b){ return a.second > b.second; });

        return results;
    }
};

int main() {
    Trie trie;

    trie.insert("apple");
    trie.insert("app");
    trie.insert("apple");
    trie.insert("apply");
    trie.insert("app");

    cout << "Frequency of 'apple': " << trie.getFrequency("apple") << endl;
    cout << "Frequency of 'app': " << trie.getFrequency("app") << endl;

    auto suggestions = trie.getSuggestions("app");
    cout << "\nSuggestions for 'app':\n";
    for (auto &p : suggestions) {
        cout << p.first << " (freq: " << p.second << ")\n";
    }

    auto corrections = trie.getCorrections("appl", 1);
    cout << "\nSpell check suggestions for 'appl' (max edit distance 1):\n";
    for (auto &p : corrections) {
        cout << p.first << " (freq: " << p.second << ")\n";
    }

    return 0;
}
