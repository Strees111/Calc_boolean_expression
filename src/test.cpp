#include <string>
#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include "boolexpr.h"

TEST(BooleanTest, And_1) {
    BooleanExpression expression("x1 & x2");
    EXPECT_EQ(expression.table(), "0001");
}

TEST(BooleanTest, Or_1) {
    BooleanExpression expression("x1 v x2");
    EXPECT_EQ(expression.table(), "0111");
}

TEST(BooleanTest, Not_1) {
    BooleanExpression expression("~x1");
    EXPECT_EQ(expression.table(), "10");
}

TEST(BooleanTest, Xor_1) {
    BooleanExpression expression("x1 + x2");
    EXPECT_EQ(expression.table(), "0110");
}

TEST(BooleanTest, Implication_1) {
    BooleanExpression expression("x1 > x2");
    EXPECT_EQ(expression.table(), "1101");
}

TEST(BooleanTest, ReverseImplication_1) {
    BooleanExpression expression("x1 < x2");
    EXPECT_EQ(expression.table(), "1011");
}

TEST(BooleanTest, Equivalence_1) {
    BooleanExpression expression("x1 = x2");
    EXPECT_EQ(expression.table(), "1001");
}

TEST(BooleanTest, Sheffer_1) {
    BooleanExpression expression("x1 | x2");
    EXPECT_EQ(expression.table(), "1110");
}

TEST(BooleanTest, Peirce_1) {
    BooleanExpression expression("x1 ^ x2");
    EXPECT_EQ(expression.table(), "1000");
}
TEST(BooleanTest, NotX2) {
    BooleanExpression expression("~x2");
    EXPECT_EQ(expression.table(), "10");
}

TEST(BooleanTest, NotX3) {
    BooleanExpression expression("~x3");
    EXPECT_EQ(expression.table(), "10");
}

TEST(BooleanTest, And_X1_X3) {
    BooleanExpression expression("x1 & x3");
    EXPECT_EQ(expression.table(), "0001");
}

TEST(BooleanTest, Or_X2_X3) {
    BooleanExpression expression("x2 v x3");
    EXPECT_EQ(expression.table(), "0111");
}

TEST(BooleanTest, Implication_X1_X3) {
    BooleanExpression expression("x1 > x3");
    EXPECT_EQ(expression.table(), "1101");
}

TEST(BooleanTest, ReverseImpl_X3_X2) {
    BooleanExpression expression("x3 < x2");
    EXPECT_EQ(expression.table(), "1101");
}

TEST(BooleanTest, Equiv_X2_X3) {
    BooleanExpression expression("x2 = x3");
    EXPECT_EQ(expression.table(), "1001");
}

TEST(BooleanTest, Xor_X1_X3) {
    BooleanExpression expression("x1 + x3");
    EXPECT_EQ(expression.table(), "0110");
}

TEST(BooleanTest, Sheffer_X2_X3) {
    BooleanExpression expression("x2 | x3");
    EXPECT_EQ(expression.table(), "1110");
}

TEST(BooleanTest, Peirce_X1_X2) {
    BooleanExpression expression("x1 ^ x2");
    EXPECT_EQ(expression.table(), "1000");
}

TEST(BooleanTest, NotAnd) {
    BooleanExpression expression("~(x1 & x2)");
    EXPECT_EQ(expression.table(), "1110");
}

TEST(BooleanTest, NotOr) {
    BooleanExpression expression("~(x1 v x3)");
    EXPECT_EQ(expression.table(), "1000");
}

TEST(BooleanTest, Complex1) {
    BooleanExpression expression("(x1 & x2) v x3");
    EXPECT_EQ(expression.table(), "01010111");
}

TEST(BooleanTest, Complex2) {
    BooleanExpression expression("(x1 > x2) & x3");
    EXPECT_EQ(expression.table(), "01010001");
}

TEST(BooleanTest, Complex3) {
    BooleanExpression expression("~(x1 + x2)");
    EXPECT_EQ(expression.table(), "1001");
}

TEST(BooleanTest, Complex4) {
    BooleanExpression expression("(x1 ^ x2) > x3");
    EXPECT_EQ(expression.table(), "01111111");
}

TEST(BooleanTest, Complex5) {
    BooleanExpression expression("x1 & (x2 v x3)");
    EXPECT_EQ(expression.table(), "00000111");
}

TEST(BooleanTest, Complex6) {
    BooleanExpression expression("(x1 = x2) < x3");
    EXPECT_EQ(expression.table(), "11101011");
}

TEST(BooleanTest, Complex7) {
    BooleanExpression expression("(x1 | x2) = (x2 | x1)");
    EXPECT_EQ(expression.table(), "1111");
}

TEST(BooleanTest, Complex8) {
    BooleanExpression expression("x1 > (x2 = x3)");
    EXPECT_EQ(expression.table(), "11111001");
}

TEST(BooleanTest, Complex9) {
    BooleanExpression expression("~(x1 & (x2 | x3))");
    EXPECT_EQ(expression.table(), "11110001");
}

TEST(BooleanTest, Complex10) {
    BooleanExpression expression("(x1 + x2) + x3");
    EXPECT_EQ(expression.table(), "01101001");
}

TEST(BooleanTest, Complex11) {
    BooleanExpression expression("((x1 & x2) | x3) > x1");
    EXPECT_EQ(expression.table(), "00001111");
}

TEST(BooleanTest, Complex12) {
    BooleanExpression expression("~(x1 ^ (x2 & x3))");
    EXPECT_EQ(expression.table(), "00011111");
}

TEST(BooleanTest, Complex13) {
    BooleanExpression expression("((x1 > x2) & (x2 > x3))");
    EXPECT_EQ(expression.table(), "11010001");
}

TEST(BooleanTest, Complex14) {
    BooleanExpression expression("(x1 < x2) v (x3 < x1)");
    EXPECT_EQ(expression.table(), "11111111");
}

TEST(BooleanTest, Complex15) {
    BooleanExpression expression("(x1 = x2) & (x2 = x3)");
    EXPECT_EQ(expression.table(), "10000001");
}

TEST(BooleanTest, Complex16) {
    BooleanExpression expression("~((x1 v x2) & x3)");
    EXPECT_EQ(expression.table(), "11101010");
}

TEST(BooleanTest, Complex17) {
    BooleanExpression expression("((x1 | x2) ^ x3)");
    EXPECT_EQ(expression.table(), "00000010");
}

TEST(BooleanTest, Complex18) {
    BooleanExpression expression("x1 = (x2 = x3)");
    EXPECT_EQ(expression.table(), "01101001");
}
TEST(BooleanTest, Complex19) {
    BooleanExpression expression("x1 & ~x2");
    EXPECT_EQ(expression.table(), "0010");
}

TEST(BooleanTest, Complex20) {
    BooleanExpression expression("~x1 & x2");
    EXPECT_EQ(expression.table(), "0100");
}

TEST(BooleanTest, Complex21) {
    BooleanExpression expression("x1 & x2 & x3");
    EXPECT_EQ(expression.table(), "00000001");
}

TEST(BooleanTest, Complex22) {
    BooleanExpression expression("x1 v x2 v x3");
    EXPECT_EQ(expression.table(), "01111111");
}

TEST(BooleanTest, Complex23) {
    BooleanExpression expression("x1 > (x2 > x3)");
    EXPECT_EQ(expression.table(), "11111101");
}

TEST(BooleanTest, Complex24) {
    BooleanExpression expression("x1 < (x2 < x3)");
    EXPECT_EQ(expression.table(), "01001111");
}

TEST(BooleanTest, Complex25) {
    BooleanExpression expression("(x1 = x2) = x3");
    EXPECT_EQ(expression.table(), "01101001");
}

TEST(BooleanTest, Complex26) {
    BooleanExpression expression("(x1 > x2) = (x2 > x3)");
    EXPECT_EQ(expression.table(), "11010001");
}

TEST(BooleanTest, Complex27) {
    BooleanExpression expression("~(x1 = x2)");
    EXPECT_EQ(expression.table(), "0110");
}

TEST(BooleanTest, Complex28) {
    BooleanExpression expression("(x1 + x2) > (x2 + x3)");
    EXPECT_EQ(expression.table(), "11100111");
}

TEST(BooleanTest, Complex29) {
    BooleanExpression expression("x1 ^ (x2 & ~x3)");
    EXPECT_EQ(expression.table(), "11010000");
}

TEST(BooleanTest, Complex30) {
    BooleanExpression expression("(x1 | x2) | x3");
    EXPECT_EQ(expression.table(), "10101011");
}

TEST(BooleanTest, Complex31) {
    BooleanExpression expression("(x1 & x2) ^ (x2 & x3)");
    EXPECT_EQ(expression.table(), "11101100");
}

TEST(BooleanTest, Complex39) {
    BooleanExpression expression("(x1 = x2) & ~(x2 = x3)");
    EXPECT_EQ(expression.table(), "01000010");
}

TEST(BooleanTest, Complex48) {
    BooleanExpression expression("x1 ^ (x2 ^ x3)");
    EXPECT_EQ(expression.table(), "01110000");
}

TEST(BooleanTest, Hard_1) {
    BooleanExpression expression("(x1 & x2 & x3) v (x4 & x5 & x6) v (x7 & x8 & x9)");
    EXPECT_EQ(expression.table(), "00000001000000010000000100000001000000010000000100000001111111110000000100000001000000010000000100000001000000010000000111111111000000010000000100000001000000010000000100000001000000011111111100000001000000010000000100000001000000010000000100000001111111110000000100000001000000010000000100000001000000010000000111111111000000010000000100000001000000010000000100000001000000011111111100000001000000010000000100000001000000010000000100000001111111111111111111111111111111111111111111111111111111111111111111111111" /* ... 512 chars */ );
}

TEST(BooleanTest, Hard_2) {
    BooleanExpression expression("((x1 > x2) & (x3 > x4)) > ((x5 < x6) | (x7 = x8))");
    EXPECT_EQ(expression.table(), "0110111101100110011011110110011011111111111111110110111101100110011011110110011001101111011001101111111111111111011011110110011011111111111111111111111111111111111111111111111111111111111111110110111101100110011011110110011011111111111111110110111101100110" /* precomputed 512-bit result */);
}

TEST(BooleanTest, Hard_3) {
    BooleanExpression expression("~((x1 + x2 + x3 + x4) = (x5 + x6 + x7 + x8 + x9))");
    EXPECT_EQ(expression.table(), "01101001100101101001011001101001100101100110100101101001100101101001011001101001011010011001011001101001100101101001011001101001100101100110100101101001100101100110100110010110100101100110100101101001100101101001011001101001100101100110100101101001100101101001011001101001011010011001011001101001100101101001011001101001011010011001011010010110011010011001011001101001011010011001011001101001100101101001011001101001100101100110100101101001100101101001011001101001011010011001011001101001100101101001011001101001" /* 512 chars */);
}

TEST(BooleanTest, Hard_4) {
    BooleanExpression expression("((x1 | x2) & (x3 | x4) & (x5 | x6) & (x7 | x8 | x9))");
    EXPECT_EQ(expression.table(), "10101011101010111010101100000000101010111010101110101011000000001010101110101011101010110000000000000000000000000000000000000000101010111010101110101011000000001010101110101011101010110000000010101011101010111010101100000000000000000000000000000000000000001010101110101011101010110000000010101011101010111010101100000000101010111010101110101011000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" /* 512 chars */);
}

TEST(BooleanTest, Hard_5) {
    BooleanExpression expression("(((x1 > x2) = (x3 < x4)) + ((x5 ^ x6) > ~(x7 & x8)))");
    EXPECT_EQ(expression.table(), "0001000000000000111011111111111100010000000000000001000000000000000100000000000011101111111111110001000000000000000100000000000011101111111111110001000000000000111011111111111111101111111111110001000000000000111011111111111100010000000000000001000000000000" /* 512 chars */);
}

TEST(BooleanTest, Hard_6) {
    BooleanExpression expression("~(((x1 v x2) & (x3 v x4)) > ((x5 + x6) = (x7 + x8 + x9)))");
    EXPECT_EQ(expression.table(), "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110100110010110100101100110100101101001100101101001011001101001011010011001011010010110011010010000000000000000000000000000000001101001100101101001011001101001011010011001011010010110011010010110100110010110100101100110100100000000000000000000000000000000011010011001011010010110011010010110100110010110100101100110100101101001100101101001011001101001" /* 512 chars */);
}

TEST(BooleanTest, Hard_7) {
    BooleanExpression expression("((x1 & x2 & x3 & x4) > (x5 & x6 & x7 & x8 & x9))");
    EXPECT_EQ(expression.table(), "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111100000000000000000000000000000001" /* 512 chars */);
}

TEST(BooleanTest, Hard_8) {
    BooleanExpression expression("(((x1 = x2) & (x3 = x4)) | ((x5 = x6) & (x7 = x8))) > x9");
    EXPECT_EQ(expression.table(), "11010111010101010101010111010111010101010101010101010101010101010101010101010101010101010101010111010111010101010101010111010111010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010111010111010101010101010111010111010101010101010101010101010101010101010101010101010101010101010111010111010101010101010111010111" /* 512 chars */);
}

TEST(BooleanTest, Hard_9) {
    BooleanExpression expression("(x7 + x8) < (x9 ^ x2) < (x1 | x7)");
    EXPECT_EQ(expression.table(), "01111101111111110111111111111111" /* 512 chars */);
}

TEST(BooleanTest, Hard_10) {
    BooleanExpression expression("~(((x1 + x2) = (x3 + x4)) + ((x5 > x6) = (x7 < x8))) > x9");
    EXPECT_EQ(expression.table(), "01110101011101011101111101110101110111111101111101110101110111111101111111011111011101011101111101110101011101011101111101110101110111111101111101110101110111110111010101110101110111110111010101110101011101011101111101110101110111111101111101110101110111111101111111011111011101011101111101110101011101011101111101110101011101010111010111011111011101011101111111011111011101011101111101110101011101011101111101110101110111111101111101110101110111111101111111011111011101011101111101110101011101011101111101110101" /* 512 chars */);
}

TEST(zhegalkinTest, Hard_1) {
    BooleanExpression expression("~(((x1 + x2) = (x3 + x4)) + ((x5 > x6) = (x7 < x8))) > x9");
    EXPECT_EQ(expression.GetZhegalkin(), "x9+x8+x8&x9+x7&x8+x7&x8&x9+x5+x5&x9+x5&x6+x5&x6&x9+x4+x4&x9+x3+x3&x9+x2+x2&x9+x1+x1&x9");
}

TEST(zhegalkinTest, Hard_2) {
    BooleanExpression expression("(x1 & x2 & x3) v (x4 & x5 & x6) v (x7 & x8 & x9)");
    EXPECT_EQ(expression.GetZhegalkin(), "x7&x8&x9+x4&x5&x6+x4&x5&x6&x7&x8&x9+x1&x2&x3+x1&x2&x3&x7&x8&x9+x1&x2&x3&x4&x5&x6+x1&x2&x3&x4&x5&x6&x7&x8&x9");
}

TEST(zhegalkinTest, Hard_3) {
    BooleanExpression expression("((x1 > x2) & (x3 > x4)) > ((x5 < x6) | (x7 = x8))");
    EXPECT_EQ(expression.GetZhegalkin(), "x8+x7+x6+x6&x8+x6&x7+x5&x6+x5&x6&x8+x5&x6&x7+x3+x3&x8+x3&x7+x3&x6+x3&x6&x8+x3&x6&x7+x3&x5&x6+x3&x5&x6&x8+x3&x5&x6&x7+x3&x4+x3&x4&x8+x3&x4&x7+x3&x4&x6+x3&x4&x6&x8+x3&x4&x6&x7+x3&x4&x5&x6+x3&x4&x5&x6&x8+x3&x4&x5&x6&x7+x1+x1&x8+x1&x7+x1&x6+x1&x6&x8+x1&x6&x7+x1&x5&x6+x1&x5&x6&x8+x1&x5&x6&x7+x1&x3+x1&x3&x8+x1&x3&x7+x1&x3&x6+x1&x3&x6&x8+x1&x3&x6&x7+x1&x3&x5&x6+x1&x3&x5&x6&x8+x1&x3&x5&x6&x7+x1&x3&x4+x1&x3&x4&x8+x1&x3&x4&x7+x1&x3&x4&x6+x1&x3&x4&x6&x8+x1&x3&x4&x6&x7+x1&x3&x4&x5&x6+x1&x3&x4&x5&x6&x8+x1&x3&x4&x5&x6&x7+x1&x2+x1&x2&x8+x1&x2&x7+x1&x2&x6+x1&x2&x6&x8+x1&x2&x6&x7+x1&x2&x5&x6+x1&x2&x5&x6&x8+x1&x2&x5&x6&x7+x1&x2&x3+x1&x2&x3&x8+x1&x2&x3&x7+x1&x2&x3&x6+x1&x2&x3&x6&x8+x1&x2&x3&x6&x7+x1&x2&x3&x5&x6+x1&x2&x3&x5&x6&x8+x1&x2&x3&x5&x6&x7+x1&x2&x3&x4+x1&x2&x3&x4&x8+x1&x2&x3&x4&x7+x1&x2&x3&x4&x6+x1&x2&x3&x4&x6&x8+x1&x2&x3&x4&x6&x7+x1&x2&x3&x4&x5&x6+x1&x2&x3&x4&x5&x6&x8+x1&x2&x3&x4&x5&x6&x7");
}

TEST(zhegalkinTest, Hard_4) {
    BooleanExpression expression("~((x1 + x2 + x3 + x4) = (x5 + x6 + x7 + x8 + x9))");
    EXPECT_EQ(expression.GetZhegalkin(), "x9+x8+x7+x6+x5+x4+x3+x2+x1");
}

TEST(zhegalkinTest, Hard_5) {
    BooleanExpression expression("((x1 | x2) & (x3 | x4) & (x5 | x6) & (x7 | x8 | x9))");
    EXPECT_EQ(expression.GetZhegalkin(), "1+x9+x7&x8&x9+x5&x6+x5&x6&x9+x5&x6&x7&x8&x9+x3&x4+x3&x4&x9+x3&x4&x7&x8&x9+x3&x4&x5&x6+x3&x4&x5&x6&x9+x3&x4&x5&x6&x7&x8&x9+x1&x2+x1&x2&x9+x1&x2&x7&x8&x9+x1&x2&x5&x6+x1&x2&x5&x6&x9+x1&x2&x5&x6&x7&x8&x9+x1&x2&x3&x4+x1&x2&x3&x4&x9+x1&x2&x3&x4&x7&x8&x9+x1&x2&x3&x4&x5&x6+x1&x2&x3&x4&x5&x6&x9+x1&x2&x3&x4&x5&x6&x7&x8&x9");
}

TEST(zhegalkinTest, Hard_6) {
    BooleanExpression expression("(((x1 > x2) = (x3 < x4)) + ((x5 ^ x6) > ~(x7 & x8)))");
    EXPECT_EQ(expression.GetZhegalkin(), "x7&x8+x6&x7&x8+x5&x7&x8+x5&x6&x7&x8+x4+x3&x4+x1+x1&x2");
}
TEST(zhegalkinTest, Hard_7) {
    BooleanExpression expression("~(((x1 v x2) & (x3 v x4)) > ((x5 + x6) = (x7 + x8 + x9)))");
    EXPECT_EQ(expression.GetZhegalkin(), "x2&x4&x9+x2&x4&x8+x2&x4&x7+x2&x4&x6+x2&x4&x5+x2&x3&x9+x2&x3&x8+x2&x3&x7+x2&x3&x6+x2&x3&x5+x2&x3&x4&x9+x2&x3&x4&x8+x2&x3&x4&x7+x2&x3&x4&x6+x2&x3&x4&x5+x1&x4&x9+x1&x4&x8+x1&x4&x7+x1&x4&x6+x1&x4&x5+x1&x3&x9+x1&x3&x8+x1&x3&x7+x1&x3&x6+x1&x3&x5+x1&x3&x4&x9+x1&x3&x4&x8+x1&x3&x4&x7+x1&x3&x4&x6+x1&x3&x4&x5+x1&x2&x4&x9+x1&x2&x4&x8+x1&x2&x4&x7+x1&x2&x4&x6+x1&x2&x4&x5+x1&x2&x3&x9+x1&x2&x3&x8+x1&x2&x3&x7+x1&x2&x3&x6+x1&x2&x3&x5+x1&x2&x3&x4&x9+x1&x2&x3&x4&x8+x1&x2&x3&x4&x7+x1&x2&x3&x4&x6+x1&x2&x3&x4&x5");
}
TEST(zhegalkinTest, Hard_8) {
    BooleanExpression expression("((x1 & x2 & x3 & x4) > (x5 & x6 & x7 & x8 & x9))");
    EXPECT_EQ(expression.GetZhegalkin(), "1+x1&x2&x3&x4+x1&x2&x3&x4&x5&x6&x7&x8&x9");
}
TEST(zhegalkinTest, Hard_9) {
    BooleanExpression expression("(x1 = x2) & (x3 = x4) | (x5 = x6)");
    EXPECT_EQ(expression.GetZhegalkin(), "x6+x5+x4+x4&x6+x4&x5+x3+x3&x6+x3&x5+x2+x2&x6+x2&x5+x2&x4+x2&x4&x6+x2&x4&x5+x2&x3+x2&x3&x6+x2&x3&x5+x1+x1&x6+x1&x5+x1&x4+x1&x4&x6+x1&x4&x5+x1&x3+x1&x3&x6+x1&x3&x5");
}

TEST(zhegalkinTest, Hard_10) {
    BooleanExpression expression("(x7 + x8) < (x9 ^ x2) < (x1 | x7)");
    EXPECT_EQ(expression.GetZhegalkin(), "x9+x8+x8&x9+x7+x7&x9+x2+x2&x9+x2&x8+x2&x8&x9+x2&x7+x2&x7&x9+x1&x7&x8+x1&x7&x8&x9+x1&x2&x7&x8+x1&x2&x7&x8&x9");
}


TEST(SKNFandSDNFTest, Hard_1) {
    BooleanExpression expression("x1 > x2 > x3");
    EXPECT_EQ(expression.GetSDNF(), "~x1&~x2&x3v~x1&x2&x3vx1&~x2&~x3vx1&~x2&x3vx1&x2&x3");
    EXPECT_EQ(expression.GetSKNF(), "(x1vx2vx3)&(x1v~x2vx3)&(~x1v~x2vx3)");
}


// TEST(FullSystemTest, Hard_1) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 & x2"),BooleanExpression("x1 v x3"),BooleanExpression("~x4")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_2) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 & x2"),BooleanExpression("~x3")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_3) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 & x2"),BooleanExpression("x1 v x3")};
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// TEST(FullSystemTest, Hard_4) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 + (x2 + x3)"),BooleanExpression("(x1 + x2) + 1")};
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// TEST(FullSystemTest, Hard_5) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("~x1 & x3 v (x1&~x3)"),BooleanExpression("x1 < x2")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_6) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 + x2 + x3"),BooleanExpression("x1 v x2"),BooleanExpression("0"),BooleanExpression("1")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_7) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 & x2"),BooleanExpression("0"),BooleanExpression("1"),BooleanExpression("x1 + x2 + x3")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_8) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 | x2")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_9) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 ^ x2")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_10) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 & x2"),BooleanExpression("(x2 v x3)"),BooleanExpression("x4")};
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// TEST(FullSystemTest, Hard_11) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("~x1"),BooleanExpression("x2 > x1")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_12) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("~x1"),BooleanExpression("x1=x2")};
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// TEST(FullSystemTest, Hard_13) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 > ~x2"),BooleanExpression("~x1 & x2")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

// TEST(FullSystemTest, Hard_14) {
//     std::vector<BooleanExpression> expressions{BooleanExpression("x1 > x2"),BooleanExpression("x1 + x2"),BooleanExpression("x1 v x2")};
//     EXPECT_EQ(isFullSystem(expressions), true);
// }
// // Тесты для проверки полноты системы Linear
// TEST(FullSystemTest, Linear) {
//     std::vector<BooleanExpression> expressions{
//         BooleanExpression("x1 + x2"), // Линейная функция
//         BooleanExpression("0"),       // Константа 0
//         BooleanExpression("1")        // Константа 1
//     };
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// // Тесты для проверки полноты системы Monotonous
// TEST(FullSystemTest, Monotonous) {
//     std::vector<BooleanExpression> expressions{
//         BooleanExpression("x1 & x2"), // Монотонная функция
//         BooleanExpression("x1 v x2") // Монотонная функция
//     };
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// // Тесты для проверки полноты системы SelfDual
// TEST(FullSystemTest, SelfDual) {
//     std::vector<BooleanExpression> expressions{
//         BooleanExpression("x1 + ~x2"), // Самодвойственная функция
//         BooleanExpression("x1 = ~x2") // Самодвойственная функция
//     };
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// // Тесты для проверки полноты системы SaveOne
// TEST(FullSystemTest, SaveOne) {
//     std::vector<BooleanExpression> expressions{
//         BooleanExpression("x1 & x2"), // Сохраняющая 1 функция
//         BooleanExpression("x1 v x2") // Сохраняющая 1 функция
//     };
//     EXPECT_EQ(isFullSystem(expressions), false);
// }

// // Тесты для проверки полноты системы SaveZero
// TEST(FullSystemTest, SaveZero) {
//     std::vector<BooleanExpression> expressions{
//         BooleanExpression("x1 | x2"), // Сохраняющая 0 функция
//         BooleanExpression("x1 & x2") // Сохраняющая 0 функция
//     };
//     EXPECT_EQ(isFullSystem(expressions), true);
// }

TEST(MinimizeTest, Hard_1) {
    BooleanExpression expression("x0 & x1 v x0 & ~x1");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0");
}

TEST(MinimizeTest, Hard_2) {
    BooleanExpression expression("x0 & x1 v ~x0 & x1");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x1");
}

TEST(MinimizeTest, Hard_3) {
    BooleanExpression expression("x0 & x1 & x2 v x0 & ~x1 & x2");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x2");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0&x2");
}

TEST(MinimizeTest, Hard_4) {
    BooleanExpression expression("x0 v ~x0");
    EXPECT_EQ(expression.GetMinimizedDNF(), "1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "1");
}

TEST(MinimizeTest, Hard_5) {
    BooleanExpression expression("x0 & ~x0");
    EXPECT_EQ(expression.GetMinimizedDNF(), "0");
    EXPECT_EQ(expression.GetMinimizedCNF(), "0");
}

TEST(MinimizeTest, Hard_6) {
    BooleanExpression expression("~x0 & x1 v x0 & ~x1");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&~x1v~x0&x1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "(~x0v~x1)&(x0vx1)");
}

TEST(MinimizeTest, Hard_7) {
    BooleanExpression expression("~x0 & ~x1 v x0 & x1");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1v~x0&~x1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "(~x0vx1)&(x0v~x1)");
}

TEST(MinimizeTest, Hard_8) {
    BooleanExpression expression("x0 & x1 v x0 & x2");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1vx0&x2");
    EXPECT_EQ(expression.GetMinimizedCNF(), "(x0)&(x1vx2)");
}
TEST(MinimizeTest, Hard_9) {
    BooleanExpression expression("~x0 & x1 v x0 & x1");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x1");
}

TEST(MinimizeTest, Hard_10) {
    BooleanExpression expression("x0 & ~x1 v x0 & x1");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0");
}

// Тесты с тремя переменными
TEST(MinimizeTest, ThreeVar_1) {
    BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0&x1");
}

TEST(MinimizeTest, ThreeVar_2) {
    BooleanExpression expression("x0 & ~x1 & x2 v ~x0 & x1 & x2");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&~x1&x2v~x0&x1&x2");
    EXPECT_EQ(expression.GetMinimizedCNF(), "(~x0v~x1)&(x0vx1)&(x2)");
}

// TEST(MinimizeTest, ThreeVar_3) {
//     BooleanExpression expression("~x0 & ~x1 & x2 v x0 & x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1&x2v~x0&~x1&x2");
// }

// TEST(MinimizeTest, ThreeVar_4) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & ~x1 & x2 v ~x0 & x1 & x2 v ~x0 & ~x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x2");
// }

// TEST(MinimizeTest, ThreeVar_5) {
//     BooleanExpression expression("x0 & x1 & ~x2 v x0 & ~x1 & ~x2 v ~x0 & x1 & ~x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x2");
// }

// TEST(MinimizeTest, ThreeVar_6) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v x0 & ~x1 & x2 v x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0");
// }

// TEST(MinimizeTest, ThreeVar_7) {
//     BooleanExpression expression("~x0 & x1 & x2 v ~x0 & x1 & ~x2 v ~x0 & ~x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x0");
// }

// TEST(MinimizeTest, ThreeVar_8) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v ~x0 & x1 & x2 v ~x0 & x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x1");
// }

// TEST(MinimizeTest, ThreeVar_9) {
//     BooleanExpression expression("x0 & ~x1 & x2 v x0 & ~x1 & ~x2 v ~x0 & ~x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x1");
// }

// // Тесты с группами размера 4
// TEST(MinimizeTest, Group4_1) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v x0 & ~x1 & x2 v x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0");
// }

// TEST(MinimizeTest, Group4_2) {
//     BooleanExpression expression("~x0 & x1 & x2 v ~x0 & x1 & ~x2 v ~x0 & ~x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x0");
// }

// TEST(MinimizeTest, Group4_3) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v ~x0 & x1 & x2 v ~x0 & x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x1");
// }

// TEST(MinimizeTest, Group4_4) {
//     BooleanExpression expression("x0 & ~x1 & x2 v x0 & ~x1 & ~x2 v ~x0 & ~x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x1");
// }

// // Смешанные тесты с различными группами
// TEST(MinimizeTest, Mixed_1) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & ~x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x2v~x0&~x1&~x2");
// }

// TEST(MinimizeTest, Mixed_2) {
//     BooleanExpression expression("x0 & x1 & ~x2 v x0 & ~x1 & ~x2 v ~x0 & x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&~x2v~x0&x1&x2");
// }

// TEST(MinimizeTest, Mixed_3) {
//     BooleanExpression expression("x0 & x1 & x2 v ~x0 & x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x1&x2v~x0&~x1&~x2");
// }

TEST(MinimizeTest, Mixed_4) {
    BooleanExpression expression("x0 & x1 & ~x2 v ~x0 & x1 & ~x2 v x0 & ~x1 & x2");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&~x1&x2vx1&~x2");
    EXPECT_EQ(expression.GetMinimizedCNF(), "(~x1v~x2)&(x0v~x2)&(x1vx2)");
}

// // Тесты с четырьмя переменными
TEST(MinimizeTest, FourVar_1) {
    BooleanExpression expression("x0 & x1 & x2 & x3 v x0 & x1 & x2 & ~x3");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1&x2");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0&x1&x2");
}

TEST(MinimizeTest, FourVar_2) {
    BooleanExpression expression("x0 & x1 & x2 & x3 v x0 & x1 & ~x2 & x3");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1&x3");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0&x1&x3");
}

TEST(MinimizeTest, FourVar_3) {
    BooleanExpression expression("x0 & x1 & x2 & x3 v x0 & x1 & x2 & ~x3 v x0 & x1 & ~x2 & x3 v x0 & x1 & ~x2 & ~x3");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0&x1");
}

TEST(MinimizeTest, FourVar_4) {
    BooleanExpression expression("x0 & x1 & x2 & x3 v x0 & ~x1 & x2 & x3 v ~x0 & x1 & x2 & x3 v ~x0 & ~x1 & x2 & x3");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x2&x3");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x2&x3");
}

// // Сложные случаи
// TEST(MinimizeTest, Complex_1) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & ~x1 & x2 v ~x0 & x1 & ~x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x2v~x0&~x2");
// }

// TEST(MinimizeTest, Complex_2) {
//     BooleanExpression expression("x0 & x1 & ~x2 v x0 & ~x1 & x2 v ~x0 & x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1&~x2vx0&~x1&x2v~x0&x1&x2v~x0&~x1&~x2");
// }

// TEST(MinimizeTest, Complex_3) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v ~x0 & ~x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1v~x0&~x1");
// }

// // Граничные случаи
// TEST(MinimizeTest, Edge_1) {
//     BooleanExpression expression("x0");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0");
// }

TEST(MinimizeTest, Edge_2) {
    BooleanExpression expression("~x0");
    EXPECT_EQ(expression.GetMinimizedDNF(), "~x0");
    EXPECT_EQ(expression.GetMinimizedCNF(), "~x0");
}

// TEST(MinimizeTest, Edge_3) {
//     BooleanExpression expression("x0 & x1");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1");
// }

// TEST(MinimizeTest, Edge_4) {
//     BooleanExpression expression("x0 v x1");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0vx1");
// }

// // Тесты с дублирующимися термами
// TEST(MinimizeTest, Duplicate_1) {
//     BooleanExpression expression("x0 & x1 v x0 & x1");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1");
// }

// TEST(MinimizeTest, Duplicate_2) {
//     BooleanExpression expression("x0 & x1 v x0 & x1 v x0 & ~x1");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0");
// }

// // Тесты с избыточными термами
// TEST(MinimizeTest, Redundant_1) {
//     BooleanExpression expression("x0 v x0 & x1");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0");
// }

// TEST(MinimizeTest, Redundant_2) {
//     BooleanExpression expression("x0 & x1 v x0 & x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1");
// }

// // Тесты с симметричными функциями
// TEST(MinimizeTest, Symmetric_1) {
//     BooleanExpression expression("x0 & ~x1 & ~x2 v ~x0 & x1 & ~x2 v ~x0 & ~x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&~x1&~x2v~x0&x1&~x2v~x0&~x1&x2");
// }

// TEST(MinimizeTest, Symmetric_2) {
//     BooleanExpression expression("x0 & x1 & ~x2 v x0 & ~x1 & x2 v ~x0 & x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1&~x2vx0&~x1&x2v~x0&x1&x2");
// }

// // Тесты с отрицаниями
// TEST(MinimizeTest, Negation_1) {
//     BooleanExpression expression("~x0 & ~x1 v ~x0 & x1");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x0");
// }

// TEST(MinimizeTest, Negation_2) {
//     BooleanExpression expression("~x0 & x1 & x2 v ~x0 & ~x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x0&x2");
// }

// TEST(MinimizeTest, Negation_3) {
//     BooleanExpression expression("x0 & ~x1 & ~x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x1&~x2");
// }

// TEST(MinimizeTest, ManyTerms_1) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v x0 & ~x1 & x2 v ~x0 & x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1vx0&x2vx1&x2");
// }

// TEST(MinimizeTest, ManyTerms_2) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & ~x1 & ~x2 v ~x0 & x1 & ~x2 v ~x0 & ~x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1&x2vx0&~x1&~x2v~x0&x1&~x2v~x0&~x1&x2");
// }

// Тесты на максимальную минимизацию
TEST(MinimizeTest, MaxMinimize_1) {
    BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v x0 & ~x1 & x2 v x0 & ~x1 & ~x2 v ~x0 & x1 & x2 v ~x0 & x1 & ~x2");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0vx1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0vx1");
}

// TEST(MinimizeTest, MaxMinimize_2) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v x0 & ~x1 & x2 v ~x0 & ~x1 & x2 v ~x0 & ~x1 & ~x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1v~x0&~x1v~x1&x2");
// }

// TEST(MinimizeTest, MaxCoverage_1) {
//     BooleanExpression expression("x0 & x1 & x2 v x0 & x1 & ~x2 v x0 & ~x1 & x2 v ~x0 & x1 & x2 v ~x0 & x1 & ~x2 v ~x0 & ~x1 & x2");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x1vx2");
// }

// TEST(MinimizeTest, Negation_Operations_1) {
//     BooleanExpression expression("~(x0 > x1) v ~(x0 + x2)");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x2v~x0&~x2v~x1&~x2");
// }

// TEST(MinimizeTest, Nested_Operations_1) {
//     BooleanExpression expression("(x0 > (x1 + x2)) v (x0 = (x1 | x2))");
//     EXPECT_EQ(expression.GetMinimizedDNF(), "~x0v~x1v~x2");
// }

TEST(MinimizeTest, Constants_Operations_1) {
    BooleanExpression expression("x0 & 1 v x0 | 0");
    EXPECT_EQ(expression.GetMinimizedDNF(), "1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "1");
}

TEST(MinimizeTest, Ultimate_1) {
    BooleanExpression expression("((x0 > x1) + (x1 = x2)) | ((x0 ^ x1) < (x1 | x2))");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0vx1v~x2");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0vx1v~x2");
}

TEST(MinimizeTest, Ultimate_2) {
    BooleanExpression expression("~((x0 | x1) = (x1 ^ x2)) + ((x0 < x1) > (x1 + x2))");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&x1&~x2");
    EXPECT_EQ(expression.GetMinimizedCNF(), "x0&x1&~x2");
}

TEST(MinimizeTest, Ultimate_3) {
    BooleanExpression expression("(x0 & x1 & x2) v (x0 > x1 > x2) v (x0 + x1 + x2) v (x0 = x1 = x2)");
    EXPECT_EQ(expression.GetMinimizedDNF(), "x0&~x1vx2v~x0&x1");
    EXPECT_EQ(expression.GetMinimizedCNF(), "(~x0v~x1vx2)&(x0vx1vx2)");
}

TEST(MinimizeTest, Ultimate_4) {
    BooleanExpression expression("x0&x1&x2&x3&x4&x5&x6vx7|x8");
    EXPECT_EQ(expression.GetMinimizedDNF(), "~x0&~x7v~x1&~x7v~x2&~x7v~x3&~x7v~x4&~x7v~x5&~x7v~x6&~x7v~x8");
    EXPECT_EQ(expression.GetMinimizedCNF(), "(~x0v~x1v~x2v~x3v~x4v~x5v~x6v~x8)&(~x7v~x8)");
}
