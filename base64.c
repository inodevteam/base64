#include <stdio.h>
#include <stdlib.h>
#include "base64.h"

#define END_OF_STRING '\0'
#define NULL_CHAR '='

static const uint8_t **msg[4] = {
    "base64 context is null",
    "base64 method is unknown. Available only ENCODE(0x0) and DECODE(0x1)"
};

static const int8_t base64_convert(const uint8_t c, const int8_t method){
    if(method == BASE64_ENCODE){
        if(c>=0 && c<=25) return 'A'+c;
        if(c>=26 && c<=51) return 'a'+(c-26);
        if(c>=52 && c<=61) return '0'+(c-52);
        if(c==62) return '+';
        if(c==63) return '/';
    }
    if(method == BASE64_DECODE){
        if(c>='A' && c<='Z') return c-'A';
        if(c>='a' && c<='z') return 26+(c-'a');
        if(c>='0' && c<='9') return 52+(c-'0');
        if(c=='+') return 62;
        if(c=='/') return 63;
    }
    return -1;
}

const bool base64_init(base64_ctx*ctx, const uint8_t method){
    if(ctx == NULL){
         printf("%s: %s\n",__FUNCTION__,msg[0]);
         return false;
    }
    if(method<0 || method>1) {
        printf("%s: %s\n",__FUNCTION__,msg[1]);
        return false;
    }
    ctx->method = method;
    ctx->mode=0;
    ctx->buf_len=0;
    return true;
}

const bool base64_write(base64_ctx*ctx, const uint8_t c){
    if(ctx == NULL){
        printf("%s: %s\n",__FUNCTION__,msg[0]);
        return false;
    }
    bool next = true;
    if(c == END_OF_STRING){
        next = false;
        if(ctx->method == BASE64_DECODE){
            ctx->buf_len++;
            ctx->buf[ctx->buf_len-1]=c;
        }
        return next;
    }
    switch(ctx->method){
        case BASE64_ENCODE:
            switch(ctx->mode){
                case 0:
                    ctx->buf_len++;
                    ctx->buf[ctx->buf_len-1]=base64_convert((c>>2)&0x3f,ctx->method);
                    ctx->mode++;
                    break;
                case 1:
                    ctx->buf_len++;
                    ctx->buf[ctx->buf_len-1]=base64_convert(((ctx->tmp<<4)|(c>>4)&0xf)&0x3f,ctx->method);
                    ctx->mode++;
                    break;
                case 2:
                    next = false;
                    ctx->buf_len++;
                    ctx->buf[ctx->buf_len-1]=base64_convert(((ctx->tmp<<2)|(c>>6)&0x3)&0x3f,ctx->method);
                    ctx->mode++;
                    break;
                case 3:
                    ctx->buf_len++;
                    ctx->buf[ctx->buf_len-1]=base64_convert(c&0x3f,ctx->method);
                    ctx->mode=0;
                    break;
            }
            break;
        case BASE64_DECODE:
            if(base64_convert(c,ctx->method)<0 || c==NULL_CHAR){
                ctx->buf_len++;
                ctx->buf[ctx->buf_len-1] = END_OF_STRING;
                break;
            }
            switch(ctx->mode){
                case 0:
                    ctx->mode++;
                    break;
                case 1:
                    ctx->buf_len++;
                    ctx->buf[ctx->buf_len-1] = (base64_convert(ctx->tmp,ctx->method)<<2)|(base64_convert(c,ctx->method)>>4);
                    ctx->mode++;
                    break;
                case 2:
                    ctx->buf_len++;
                    ctx->buf[ctx->buf_len-1] = (base64_convert(ctx->tmp,ctx->method)<<4)|(base64_convert(c,ctx->method)>>2);
                    ctx->mode++;
                    break;
                case 3:
                    ctx->buf_len++;
                    ctx->buf[ctx->buf_len-1] = (base64_convert(ctx->tmp,ctx->method)<<6)|(base64_convert(c,ctx->method));
                    ctx->mode=0;
                    break;
            }
            break;
    }
    ctx->tmp=c;
    return next;
}

const bool base64_available(const base64_ctx*ctx){
    if(ctx == NULL){
        printf("%s: %s\n",__FUNCTION__,msg[0]);
        return false;
    }
    return ctx->method==BASE64_ENCODE || (ctx->method==BASE64_DECODE && ctx->buf_len>0);
}

const uint8_t base64_read(base64_ctx*ctx){
    uint8_t out = NULL_CHAR;
    if(ctx == NULL){
        printf("%s: %s\n",__FUNCTION__,msg[0]);
        out = END_OF_STRING;
        return out;
    }
    if(ctx->buf_len>0){
        out=ctx->buf[0];
        ctx->buf[0]=ctx->buf[1];
        ctx->buf_len--;
        return out;
    }
    switch(ctx->method){
        case BASE64_ENCODE:
            switch(ctx->mode){
                case 0:
                    out=END_OF_STRING;
                    break;
                case 1:
                    if(ctx->tmp != END_OF_STRING){
                        out = base64_convert((ctx->tmp<<4)&0x3f,ctx->method);
                        ctx->tmp = END_OF_STRING;
                    }
                    ctx->mode++;
                    break;
                case 2:
                    if(ctx->tmp != END_OF_STRING){
                        out = base64_convert((ctx->tmp<<2)&0x3f,ctx->method);
                        ctx->tmp = END_OF_STRING;
                    }
                    ctx->mode++;
                    break;
                case 3:
                    if(ctx->tmp != END_OF_STRING){
                        out = base64_convert((ctx->tmp)&0x3f,ctx->method);
                        ctx->tmp = END_OF_STRING;
                    }
                    ctx->mode=0;
                    break;
            }
            break;
    }
    return out;
}
