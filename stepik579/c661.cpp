/**
 * Multiway Trie
 * author: Manny egalli64@gmail.com
 * info: http://thisthread.blogspot.com/
 *       https://stepik.org/lesson/30819/step/11
 *
 */
#include "gtest/gtest.h"
#include<string>
#include<vector>
#include<cctype>

namespace
{
    struct Node {
        bool word = false;
        Node* children[26];
        Node() {
            for (int i = 0; i < 26; ++i) {
                children[i] = nullptr;
            }
        }
    };

    class MultiwayTrie {
    public:
        Node * root = new Node();
        bool find(const std::string& word);
        void insert(const std::string& word);
        void remove(const std::string& word);
    };

    bool MultiwayTrie::find(const std::string& word) {
        Node* current = root;
        for (auto& c : word) {
            uint8_t i = std::tolower(c) - 'a';
            current = current->children[i];
            if (current == nullptr)
                return false;
        }

        return current->word;
    }

    void MultiwayTrie::insert(const std::string& word) {
        Node* current = root;
        for (auto& c : word) {
            uint8_t i = std::tolower(c) - 'a';
            if (!current->children[i]) {
                current->children[i] = new Node();
            }
            current = current->children[i];
        }
        current->word = true;
    }

    void MultiwayTrie::remove(const std::string& word) {
        Node* current = root;
        for (auto& c : word) {
            uint8_t i = std::tolower(c) - 'a';
            current = current->children[i];
            if (current == nullptr)
                return;
        }
        current->word = false;
    }
}

TEST(TestLexicon, Vector)
{
    MultiwayTrie trie;

    ASSERT_FALSE(trie.find("hello"));

    trie.insert("hello");
    ASSERT_TRUE(trie.find("hello"));

    trie.remove("hello");
    ASSERT_FALSE(trie.find("hello"));
}
