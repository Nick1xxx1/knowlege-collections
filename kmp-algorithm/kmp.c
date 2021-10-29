#include "kmp.h"

/**
 * @brief 生成next数组
 * 
 * @param pattern 用于生成next数组的字符串
 * @param next next数组
 */
static void makeNext(const char *pattern, int *next)
{
    int patternLen = strlen(pattern);
    next[0] = 0;

    for (int prefix = 0, suffix = 1; suffix < patternLen; ++suffix)
    {
        // 不相等时进行回溯
        while (prefix > 0 && pattern[suffix] != pattern[prefix])
        {
            prefix = next[prefix - 1];
        }

        if (pattern[prefix] == pattern[suffix])
        {
            ++prefix;
        }

        next[suffix] = prefix;
    }
}

int kmp(const char *text, const char *pattern, int *next)
{
    makeNext(pattern, next);

    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    int textIndex = 0;
    int patternIndex = 0;
    for (; textIndex < textLen; ++textIndex)
    {
        // 不相等时进行回溯
        while (patternIndex > 0 && text[textIndex] != pattern[patternIndex])
        {
            patternIndex = next[patternIndex - 1];
        }

        if (pattern[patternIndex] == text[textIndex])
        {
            ++patternIndex;
        }

        if (patternIndex == patternLen)
        {
            return textIndex - patternIndex + 1;
        }
    }

    return -1;
}
