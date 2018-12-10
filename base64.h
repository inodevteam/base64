#ifndef base64_h
    #define base64_h
    #include "c_types.h"

    #define BASE64_ENCODE 0
    #define BASE64_DECODE 1

    typedef struct {
        uint8_t buf[2],buf_len,mode,tmp;
        int8_t method;
    } base64_ctx;

    const bool base64_init(base64_ctx*,const uint8_t);
    const bool base64_available(const base64_ctx*);
    const bool base64_write(base64_ctx*,const uint8_t);
    const uint8_t base64_read(base64_ctx*);

#endif // base64_h
