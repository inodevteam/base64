#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace crypto{
    class Base64{
      private:
         static vector<uint8_t> _chars;
         static void init() {
            if(_chars.size()!=64){
               for(uint8_t c='A';c<='Z';c++)
                  _chars.push_back(c);
               for(uint8_t c='a';c<='z';c++)
                  _chars.push_back(c);
               for(uint8_t c='0';c<='9';c++)
                  _chars.push_back(c);
               _chars.push_back('+');
               _chars.push_back('/');
            }
            cout << "base64_init -> " << (_chars.size()!=64 ? "fail" : "ok") << endl;
         }
         uint8_t _tmp;
         size_t _mode;
         vector<uint8_t> _bufo;
         Base64() : _mode(0){
         }
      public:
         static Base64 create(){
            init();
            return Base64();
         }
         static const bool write(Base64&b64, const uint8_t&c){
            bool next = true;
            switch(b64._mode){
               case 0:
                  b64._bufo.push_back(_chars[(c>>2)&0x3f]);
                  b64._mode++;
                  break;
               case 1:
                  b64._bufo.push_back(_chars[((b64._tmp<<4)+((c>>4)&0xf))&0x3f]);
                  b64._mode++;
                  break;
               case 2:
                  next = false;
                  b64._bufo.push_back(_chars[((b64._tmp<<2)+((c>>6)&0x3))&0x3f]);
                  b64._mode++;
                  break;
               case 3:
                  b64._bufo.push_back(_chars[c&0x3f]);
                  b64._mode=0;
                  break;
            }
            b64._tmp =c;
            return next;
         }
         static const uint8_t read(Base64&b64){
            uint8_t out = '=';
            if(b64._bufo.size()>0){
               out = b64._bufo[0];
               b64._bufo.erase(b64._bufo.begin());
               return out;
            }
            switch(b64._mode){
               case 0:
                     out = '\0';
                  break;
               case 1:
                  if(b64._tmp!='\0'){
                     out = _chars[(b64._tmp<<4)&0x3f];
                     b64._tmp = '\0';
                  }
                  b64._mode++;
                  break;
               case 2:
                  if(b64._tmp!='\0'){
                     out = _chars[(b64._tmp<<2)&0x3f];
                     b64._tmp = '\0';
                  }
                  b64._mode++;
                  break;
               case 3:
                  if(b64._tmp!='\0'){
                     out = _chars[(b64._tmp)&0x3f];
                     b64._tmp = '\0';
                  }
                  b64._mode=0;
                  break;
               }
            return out;
         }
    };
    vector<uint8_t> Base64::_chars;
}

using namespace crypto;

int main() {
   Base64 b64e = Base64::create();
   string s = "Hello, world!";
   size_t x=0;
   uint8_t r;
   while(true){
      if(s[x]!='\0'){
         if(Base64::write(b64e,s[x]))
            x++;
      }
      r = Base64::read(b64e);
      if(r=='\0') break;
      cout << r;
   }
   cout << endl;
}
