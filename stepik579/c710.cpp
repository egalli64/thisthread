/**
 * Encoding a Message as ASCII Values
 * author: Manny egalli64@gmail.com
 * info: http://thisthread.blogspot.com/
 *       https://stepik.org/lesson/26125/step/2
 *
 */
#include "gtest/gtest.h"
#include <iostream>
#include <string>

namespace
{
    void encodeString1(const std::string& message) {
        for (auto& c : message) {
            std::cout << (int)c << ' ';
        }
        std::cout << std::endl;
    }

    void encodeString(const std::string& message) {
        for (auto& c : message) {
            switch (c) {
            case 'A':
                std::cout << "00";
                break;
            case 'C':
                std::cout << "01";
                break;
            case 'G':
                std::cout << "10";
                break;
            case 'T':
                std::cout << "11";
                break;
            }
        }
        std::cout << std::endl;
    }

    void decodeString(const std::string& message) {
        for (size_t i = 0; i < message.size(); i += 2) {
            if (message[i] == '0') {
                if (message[i + 1] == '0')
                    std::cout << 'A';
                else
                    std::cout << 'C';
            }
            else {
                if (message[i + 1] == '0')
                    std::cout << 'G';
                else
                    std::cout << 'T';
            }
        }
    }
}

TEST(TestEncoder, Simple)
{
    encodeString("ACGT");
    decodeString("00011011");
}
