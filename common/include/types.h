#ifndef _KNOWLEGE_COLLECTIONS_TYPES_H_
#define _KNOWLEGE_COLLECTIONS_TYPES_H_

using nc_char_t = char;
using nc_void_t = void;
using nc_bool_t = bool;
using nc_uint8_t = unsigned char;
using nc_int8_t = signed char;
using nc_uint16_t = unsigned short;
using nc_int16_t = signed short;
using nc_uint32_t = unsigned int;
using nc_int32_t = signed int;
using nc_uint64_t = unsigned long long;
using nc_int64_t = signed long long;
using nc_socket_t = signed int;
using nc_float32_t = float;
using nc_float64_t = double;

constexpr nc_int8_t kError = -1;
constexpr nc_int8_t kOk = 0;
constexpr nc_uint32_t kByteBits = 8;  // 一个字节的比特数

#endif // _KNOWLEGE_COLLECTIONS_TYPES_H_
