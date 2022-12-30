#include "custom_sys_func.h"

#include <assert.h>

void *CustomMemcpy(void *dest, void *src, nc_uint32_t n) {
  if (!dest || !src) {
    return nullptr;
  }

  nc_char_t *d = static_cast<nc_char_t*>(dest);
  const nc_char_t *s = static_cast<const nc_char_t*>(src);

  while (n--) {
    *d++ = *s++;
  }

  return dest;
}

void *CustomMemmove(void *dest, void *src, nc_uint32_t n) {
  if (!dest || !src) {
    return nullptr;
  }

  if (dest == src) {
    return dest;
  }

  nc_char_t *d = static_cast<nc_char_t*>(dest);
  const nc_char_t *s = static_cast<const nc_char_t*>(src);

  if (d < s) {
    while (n--) {
      *d++ = *s++;
    }
  } else {
    d = d + n - 1;
    s = s + n - 1;
    while (n--) {
      *d-- = *s--;
    }
  }

  return dest;
}

nc_char_t *CustomStrcat(nc_char_t *dest, const nc_char_t *src) {
  if (!dest || !src) {
    return nullptr;
  }

  nc_char_t *d = dest;
  while (*d) {
    ++d;
  }

  *d = *src;
  while (*d) {
    ++d;
    ++src;
    *d = *src;
  }

  return dest;
}

nc_char_t *CustomStrcpy(nc_char_t *dest, const nc_char_t *src) {
  if (!dest || !src) {
    return nullptr;
  }

  nc_char_t *d = dest;
  *d = *src;
  while (*d) {
      ++d;
      ++src;
      *d = *src;
  }

  return dest;
}

nc_int32_t CustomStrcmp(const nc_char_t *s1, const nc_char_t *s2) {
  if (!s1) {
    return -1;
  }

  if (!s2) {
    return 1;
  }

  nc_int32_t ret = 0;
  while (!(ret = *(nc_uint8_t *)s1 - *(nc_uint8_t*)s2) && *s2) {
    ++s1;
    ++s2;
  }

  if (ret < 0) {
    ret = -1;
  } else if (ret > 0) {
    ret = 1;
  } else {
    ret = 0;
  }

  return ret;
}

nc_int32_t CustomStrncmp(const nc_char_t *s1, const nc_char_t *s2, nc_uint32_t n) {
  if (n == 0) {
    return 0;
  }

  if (!s1) {
    return -1;
  }

  if (!s2) {
    return 1;
  }

  while (--n && *s2 && *s1 == *s2) {
    s1++;
    s2++;
  }

  nc_int32_t ret = *s1 - *s2;
  if (ret < 0) {
    ret = -1;
  } else if (ret > 0) {
    ret = 1;
  } else {
    ret = 0;
  }

  return ret;
}

nc_uint32_t CustomStrlen(const nc_char_t *str) {
  if (!str) {
    return 0;
  }

  nc_uint32_t len = 0;
  while (*str++) {
    ++len;
  }

  return len;
}

nc_char_t *CustomStrstr(const nc_char_t *s1, const nc_char_t *s2) {
  if (!s2) {
    return nullptr;
  }

  nc_int32_t s2_len = CustomStrlen(s2);
  if (s2_len == 0) {
    return const_cast<nc_char_t *>(s1);
  }

  for (; *s1; ++s1) {
    if ((*s1 == *s2) && CustomStrncmp(s1, s2, s2_len) == 0) {
      return const_cast<nc_char_t *>(s1);
    }
  }

  return nullptr;
}

static nc_int64_t core(nc_char_t *str, nc_bool_t is_minus) {
  nc_int64_t sum = 0;
  while (*str) {
    if (*str < '0' || *str > '9') {
      sum = 0;
      break;
    }

    nc_int32_t flag = is_minus ? -1 : 1;
    sum = sum * 10 + (*str - '0') * flag;
    if ((!is_minus && sum > 0x7fffffff) ||
        (is_minus && sum < (nc_int32_t)0x80000000)) {
      sum = 0;
      break;
    }

    ++str;
  }

  return sum;
}

nc_int32_t CustomAtoi(nc_char_t *str) {
  bool is_minus = false;
  if (*str == '-') {
    is_minus = true;
    ++str;
  } else if (*str == '+') {
    ++str;
  }

  nc_int64_t num = core(str, is_minus);
  return static_cast<nc_int32_t>(num);
}

void *CustomMemset(void *mem, nc_int32_t val, nc_uint32_t n) {
  if (!mem) {
    return nullptr;
  }

  nc_char_t *cp = (nc_char_t *)mem;
  while (n--) {
    *cp++ = (nc_char_t)val;
  }

  return mem;
}
