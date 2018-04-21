/**
 * HackerRank C++ Strings Challenges
 * https://www.hackerrank.com/domains/cpp/cpp-strings
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/
 */
#include<iterator>
#include<sstream>
#include<string>
#include<unordered_map>
#include<vector>

#include "gtest/gtest.h"

using VString = std::vector<std::string>;
using HashStrStr = std::unordered_map<std::string, std::string>;
using IstrItStr = std::istream_iterator<std::string>;

namespace
{
    std::string join(const VString& data, char separator)
    {
        std::ostringstream oss;

        if (!data.empty())
        {
            oss << data[0];
        }

        for (int i = 1, end = data.size(); i < end; ++i)
        {
            oss << separator << data[i];
        }

        return oss.str();
    }

    VString split(const std::string& s)
    {
        std::istringstream iss(s);
        return VString(IstrItStr{iss}, IstrItStr());
    }


    /**
     * HackerRank C++ Strings Attribute Parser
     * https://www.hackerrank.com/challenges/attribute-parser/problem
     */
    VString attributeParser(const VString& tags, const VString& queries)
    {
        HashStrStr dict;
        VString fullName;
        for (const auto& tag : tags)
        {
            VString tagInfo = split(tag);
            if (tagInfo[0][1] == '/')
            {   // closing tag
                fullName.pop_back();
                continue;
            }

            std::string tagName = tagInfo[0].substr(1, tagInfo[0].size() - (tagInfo.size() == 1 ? 2 : 1));
            fullName.push_back(tagName);

            for (size_t i = 1, end = tagInfo.size(); i < end; i += 3)
            {
                int shift = i == tagInfo.size() - 3 ? 1 : 0;
                std::string attrName = tagInfo[i];
                std::string attrValue = tagInfo[i+2].substr(1, tagInfo[i+2].size() - (2 + shift));

                std::string key = join(fullName, '.') + '~' + attrName;
                dict[key] = attrValue;
            }
        }

        VString results;
        for (const auto& query : queries)
        {
            auto it = dict.find(query);

            results.push_back(it != dict.end() ? it->second : "Not Found!");
        }

        return results;
    }
}

TEST(TestAttributeParser, Given)
{
    VString tags{ "<tag1 value = \"HelloWorld\">", "<tag2 name = \"Name1\">", "</tag2>", "</tag1>" };
    VString queries{ "tag1.tag2~name", "tag1~name", "tag1~value" };

    auto results = attributeParser(tags, queries);
    ASSERT_EQ(queries.size(), results.size());
    EXPECT_EQ("Name1", results[0]);
    EXPECT_EQ("Not Found!", results[1]);
    EXPECT_EQ("HelloWorld", results[2]);
}

TEST(TestAttributeParser, Extra1)
{
    VString tags{
        "<tag1 value = \"HelloWorld\">",
            "<tag2 name = \"Name1\">",
                "<tag3 name = \"Name3\">", "</tag3>",
                "<tag4 name = \"Name4\" text = \"Super\">",
                    "<tag6 buzz = \"Buzz6\">", "</tag6>",
                "</tag4>",
                "<tag5>","</tag5>",
            "</tag2>",
        "</tag1>"
    };

    VString queries{ "tag1.tag2~name", "tag1~name", "tag1~value", "tag1.tag2.tag3~name",
        "tag1.tag2.tag3~value", "tag1.tag2.tag4~text","tag1.tag2.tag4.tag6~text", "tag1.tag2.tag4.tag6~buzz"
    };

    auto results = attributeParser(tags, queries);
    ASSERT_EQ(queries.size(), results.size());
    EXPECT_EQ("Name1", results[0]);
    EXPECT_EQ("Not Found!", results[1]);
    EXPECT_EQ("HelloWorld", results[2]);
    EXPECT_EQ("Name3", results[3]);
    EXPECT_EQ("Not Found!", results[4]);
    EXPECT_EQ("Super", results[5]);
    EXPECT_EQ("Not Found!", results[6]);
    EXPECT_EQ("Buzz6", results[7]);
}


TEST(TestAttributeParser, Extra2)
{
    VString tags{
        "<tag1 v1 = \"123\" v2 = \"43.4\" v3 = \"hello\">",
        "</tag1>",
        "<tag2 v4 = \"v2\" name = \"Tag2\">",
            "<tag3 v1 = \"Hello\" v2 = \"World!\">",
            "</tag3>",
            "<tag4 v1 = \"Hello\" v2 = \"Universe!\">",
            "</tag4>",
        "</tag2>",
        "<tag5>",
            "<tag7 new_val = \"New\">",
            "</tag7>",
        "</tag5>",
        "<tag6>",
            "<tag8 intval = \"34\" floatval = \"9.845\">",
            "</tag8>",
        "</tag6>"
    };

    VString queries{ "tag1~v1", "tag1.v1", "tag1~v2", "tag1~v3", "tag4~v2", "tag2.tag4~v1",
        "tag2.tag4~v2", "tag2.tag3~v2", "tag5.tag7~new_val", "tag5~new_val", "tag7~new_val",
        "tag6.tag8~intval", "tag6.tag8~floatval", "tag6.tag8~val", "tag8~intval"
    };

    auto results = attributeParser(tags, queries);
    ASSERT_EQ(queries.size(), results.size());
    EXPECT_EQ("123", results[0]);
    EXPECT_EQ("Not Found!", results[1]);
    EXPECT_EQ("43.4", results[2]);
    EXPECT_EQ("hello", results[3]);
    EXPECT_EQ("Not Found!", results[4]);
    EXPECT_EQ("Hello", results[5]);
    EXPECT_EQ("Universe!", results[6]);
    EXPECT_EQ("World!", results[7]);
    EXPECT_EQ("New", results[8]);
    EXPECT_EQ("Not Found!", results[9]);
    EXPECT_EQ("Not Found!", results[10]);
    EXPECT_EQ("34", results[11]);
    EXPECT_EQ("9.845", results[12]);
    EXPECT_EQ("Not Found!", results[13]);
    EXPECT_EQ("Not Found!", results[14]);
}


#ifdef STANDALONE

int main()
{
    int n, q;
    std::cin >> n >> q;
    std::cin.ignore();

    std::vector<std::string> tags;
    for (int i = 0; i < n; ++i)
    {
        std::string cur;
        std::getline(std::cin, cur);
        tags.push_back(cur);
    }

    std::vector<std::string> queries;
    for (int i = 0; i < q; ++i)
    {
        std::string cur;
        std::getline(std::cin, cur);
        queries.push_back(cur);
    }

    VString results = attributeParser(tags, queries);
    for (const auto& result : results)
    {
        std::cout << result << std::endl;
    }
}

#endif // STANDALONE

