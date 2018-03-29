/**
 * Implementing a Lexicon using different ADTs
 * author: Manny egalli64@gmail.com
 * info: http://thisthread.blogspot.com/
 *
 */
#include "gtest/gtest.h"
#include<string>
#include<vector>
#include<list>
#include<unordered_set>
#include<unordered_map>
#include<algorithm>


namespace
{
    class LexiconList {
    public:
        std::list<std::string> linkedList;
        bool find(const std::string& word);
        void insert(const std::string& word);
        void remove(const std::string& word);
    };

    bool LexiconList::find(const std::string& word) {
        return std::find(linkedList.begin(), linkedList.end(), word) != linkedList.end();
    }

    void LexiconList::insert(const std::string& word) {
        linkedList.push_back(word);
    }

    void LexiconList::remove(const std::string& word) {
        linkedList.remove(word);
    }

    class LexiconVector {
    public:
        std::vector<std::string> array;
        bool find(const std::string& word);
        void insert(const std::string& word);
        void remove(const std::string& word);
    };

    bool LexiconVector::find(const std::string& word) {
        return std::binary_search(array.begin(), array.end(), word);
    }

    void LexiconVector::insert(const std::string& word) {
        array.push_back(word);
        std::sort(array.begin(), array.end());
    }

    void LexiconVector::remove(const std::string& word) {
        array.erase(std::remove(array.begin(), array.end(), word));
    }

    class LexiconSet {
    public:
        std::set<std::string> tree;
        bool find(const std::string& word);
        void insert(const std::string& word);
        void remove(const std::string& word);
    };

    bool LexiconSet::find(const std::string& word) {
        return tree.find(word) != tree.end();
    }

    void LexiconSet::insert(const std::string& word) {
        tree.insert(word);
    }

    void LexiconSet::remove(const std::string& word) {
        tree.erase(word);
    }

    class LexiconUSet {
    public:
        std::unordered_set<std::string> hashTable;
        bool find(const std::string& word);
        void insert(const std::string& word);
        void remove(const std::string& word);
    };

    bool LexiconUSet::find(const std::string& word) {
        return hashTable.find(word) != hashTable.end();
    }

    void LexiconUSet::insert(const std::string& word) {
        hashTable.insert(word);
    }

    void LexiconUSet::remove(const std::string& word) {
        hashTable.erase(word);
    }

    class Dictionary {
    public:
        std::unordered_map<std::string, std::string> hashMap;
        std::string find(const std::string& word);
        void insert(const std::string& word, const std::string& def);
        void remove(const std::string& word);
    };

    /* Return the definition of word if Dictionary contains word, otherwise return empty string ("") */
    std::string Dictionary::find(const std::string& word) {
        auto it = hashMap.find(word);
        return it == hashMap.end() ? "" : it->second;
    }

    /* Insert a (word, definition) pair into Dictionary (return nothing) */
    void Dictionary::insert(const std::string& word, const std::string& def) {
        hashMap[word] = def;
    }

    /* Remove word from Dictionary (return nothing) */
    void Dictionary::remove(const std::string& word) {
        hashMap.erase(word);
    }
}

TEST(TestLexicon, Vector)
{
    LexiconVector lex;
    lex.insert("hello");
    ASSERT_TRUE(lex.find("hello"));

    lex.insert("andy");
    ASSERT_TRUE(lex.find("hello"));
    ASSERT_TRUE(lex.find("andy"));

    lex.insert("zoe");
    ASSERT_TRUE(lex.find("hello"));
    ASSERT_TRUE(lex.find("andy"));
    ASSERT_TRUE(lex.find("zoe"));

    lex.remove("hello");
    ASSERT_TRUE(lex.find("andy"));
    ASSERT_TRUE(lex.find("zoe"));
    ASSERT_FALSE(lex.find("hello"));

    lex.remove("andy");
    ASSERT_FALSE(lex.find("andy"));
    ASSERT_TRUE(lex.find("zoe"));
    ASSERT_FALSE(lex.find("hello"));
}
