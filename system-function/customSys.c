#include "customSys.h"
#include "assert.h"

void *customMemcpy(void *dest, void *src, size_t n)
{
    if (!dest || !src)
    {
        return NULL;
    }

    char *d = (char *)dest;
    const char *s = (const char *)src;

    while (n--)
    {
        *d++ = *s++;
    }

    return dest;
}

void *customMemmove(void *dest, void *src, size_t n)
{
    if (!dest || !src)
    {
        return NULL;
    }

    char *d = (char *)dest;
    const char *s = (const char *)src;

    if (d < s)
    {
        while (n--)
        {
            *d++ = *s++;
        }
    }
    else if (d > s)
    {
        d = d + n - 1;
        s = s + n - 1;
        while (n--)
        {
            *d-- = *s--;
        }
    }

    return dest;
}

char *customStrcat(char *dest, const char *src)
{
    if (!dest || !src)
    {
        return NULL;
    }

    char *d = dest;
    while (*d)
    {
        d++;
    }

    *d = *src;
    while (*d)
    {
        ++d;
        ++src;
        *d = *src;
    }

    return dest;
}

char *customStrcpy(char *dest, const char *src)
{
    if (!dest || !src)
    {
        return NULL;
    }

    char *d = dest;
    *d = *src;
    while (*d)
    {
        ++d;
        ++src;
        *d = *src;
    }

    return dest;
}

int customStrcmp(const char *s1, const char *s2)
{
    assert(s1 && s2);
    int ret = 0;
    while (!(ret = *(unsigned char *)s1 - *(unsigned char *)s2) && *s2)
    {
        ++s1;
        ++s2;
    }

    if (ret < 0)
    {
        return -1;
    }
    else if (ret > 0)
    {
        return 1;
    }

    return 0;
}

int customStrncmp(const char *s1, const char *s2, size_t n)
{
    if (!n)
    {
        return 0;
    }

    assert(s1 && s2);

    while (--n && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }

    return (*s1 - *s2);
}

size_t customStrlen(const char *str)
{
    if (!str)
    {
        return 0;
    }

    size_t len = 0;
    while (*str++)
    {
        ++len;
    }

    return len;
}

char *customStrstr(const char *s1, const char *s2)
{
    if (!s2)
    {
        return NULL;
    }

    int lenOfs2 = customStrlen(s2);
    if (lenOfs2 == 0)
    {
        return (char *)s1;
    }

    for (; *s1; ++s1)
    {
        if ((*s1 == *s2) && customStrncmp(s1, s2, lenOfs2) == 0)
        {
            return (char *)s1;
        }
    }

    return NULL;
}

#define bool int
#define TRUE 1
#define FALSE 0

static long long core(char *str, bool isMinus)
{
    long long sum = 0;
    while (*str)
    {
        if (*str < '0' || *str > '9')
        {
            sum = 0;
            break;
        }

        int flag = isMinus ? -1 : 1;
        sum = sum * 10 + (*str - '0') * flag;
        if ((!isMinus && sum > 0x7fffffff) ||
            (isMinus && sum < (signed int)0x80000000))
        {
            sum = 0;
            break;
        }

        ++str;
    }

    return sum;
}

int customAtoi(char *str)
{
    assert(str && *str);

    bool isMinus = FALSE;
    if (*str == '-')
    {
        isMinus = TRUE;
        ++str;
    }
    else if (*str == '+')
    {
        ++str;
    }

    long long num = core(str, isMinus);
    return (int)num;
}

void *customMemset(void *s, int c, size_t n)
{
    if (!s)
    {
        return NULL;
    }

    char *cp = (char *)s;
    while (n--)
    {
        *cp++ = (char)c;
    }

    return s;
}
