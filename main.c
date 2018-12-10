#include <stdio.h>
#include <stdlib.h>
#include "c_types.h"
#include "base64.h"

void doWork(base64_ctx*ctx, const uint8_t method, const uint8_t*s,uint8_t*out){
    if(ctx == NULL || !base64_init(ctx,method)) return;
    uint8_t x=0,r,r_cnt=0;
    while(true){
        if(base64_write(ctx,s[x]))
            x++;
        if(base64_available(ctx)){
            r=base64_read(ctx);
            out[r_cnt]=r;
            if(r == '\0') break;
            r_cnt++;
        }
    }
}

int main()
{
    uint8_t *s = "Hello, world!";
    uint8_t buf[32], buf2[32];
    base64_ctx ctx;
    printf("Source: %s\n",s);
    doWork(&ctx,BASE64_ENCODE,s,buf);
    printf("Encoded: %s\n",buf);
    doWork(&ctx,BASE64_DECODE,buf,buf2);
    printf("Decoded: %s\n",buf2);

    return 0;
}
