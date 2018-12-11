#ifndef c_types_h
  #define c_types_h
  // 8-bit
  typedef unsigned char       uint8_t;
  typedef signed char         sint8_t;
  typedef signed char         int8_t;
  // 16-bit
  typedef unsigned short      uint16_t;
  typedef signed short        sint16_t;
  typedef signed short        int16_t;
  // 32-bit
  typedef unsigned long       uint32_t;
  typedef signed long         sint32_t;
  typedef signed long         int32_t;
  // 64-bit
  typedef signed long long    sint64_t;
  typedef unsigned long long  uint64_t;
  typedef unsigned long long  u_int64_t;
  // real
  typedef float               real32_t;
  typedef double              real64_t;
  // size
  typedef unsigned int        size_t;
  // nullptr
  #ifndef NULL
      #define NULL (void *)0
  #endif /* NULL */

  #ifndef __cplusplus
      typedef unsigned char   bool;
      #define true            (1)
      #define false           (0)
  #endif /* !__cplusplus */
#endif // c_types_h
