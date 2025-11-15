#include #include #include using namespace std;

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


void collectWords(TrieNode* node, string current, vector>& result) {
if (!node) return;

if (node->isEndOfWord) {
result.push_back({current, node->frequency});
}

for (int i = 0; i < 26; i++) {
if (node->children[i]) {
collectWords(node->children[i], current + char('a' + i), result);
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

if (!current->children[index]) {
current->children[index] = new TrieNode();
}

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


vector> getSuggestions(const string &prefix) {
TrieNode* current = root;

for (char ch : prefix) {
int index = ch - 'a';
if (!current->children[index])
return {}; 
current = current->children[index];
}

vector> result;
collectWords(current, prefix, result);


sort(result.begin(), result.end(),
[](auto &a, auto &b){ return a.second > b.second; });

return result;
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

return 0;
}