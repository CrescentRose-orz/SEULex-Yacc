#ifndef CONSTANT
#define CONSTANT


#define isTerminal(x) (x < TNBound)
#define I2S(x) IntToStr[x]
#define S2I(x) StrToInt[x]
#define VISUAL
#define getLeft(x) TranslationRule_Int[x].first
#define getRight(x) TranslationRule_Int[x].second

#endif