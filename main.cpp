#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace crypto{

    static const char END_OF_STRING = '\0';
    static const string BASE64_ALPHABET = "AZaz09";

    class Base64{
      private:
         static vector<uint8_t> _chars;
         static void init() {
            if(_chars.size()!=64){
               _chars.clear();
               size_t m=0;
               for(uint8_t c=BASE64_ALPHABET[m];
                   m<BASE64_ALPHABET.length()&&c<=BASE64_ALPHABET[m+1];
                   c=(c!=BASE64_ALPHABET[m+1]?c+=1:BASE64_ALPHABET[m+=2])
               ) _chars.push_back(c);
               _chars.push_back('+');
               _chars.push_back('/');
            }
            cout << "base64_init: (" << _chars.size() << ") -> " << (_chars.size()==64 ? "ok" : "fail") << endl;
         }
         uint8_t _tmp;
         size_t _mode;
         vector<uint8_t> _out;
         Base64() : _mode(0){}
      public:
         static Base64 create(){
            init();
            return Base64();
         }
         static const bool available(const Base64&b64) {
            return b64._out.size()>0 ? true : b64._mode!=4;
         }
         static const bool write(Base64&b64, const uint8_t&c){
            bool next = true;
            switch(b64._mode){
               case 0:
                  b64._out.push_back(_chars[(c>>2)&0x3f]);
                  b64._mode++;
                  break;
               case 1:
                  b64._out.push_back(_chars[((b64._tmp<<4)+((c>>4)&0xf))&0x3f]);
                  b64._mode++;
                  break;
               case 2:
                  next = false;
                  b64._out.push_back(_chars[((b64._tmp<<2)+((c>>6)&0x3))&0x3f]);
                  b64._mode++;
                  break;
               case 3:
                  b64._out.push_back(_chars[c&0x3f]);
                  b64._mode=0;
                  break;
            }
            b64._tmp =c;
            return next;
         }
         static const uint8_t read(Base64&b64){
            uint8_t out = '=';
            if(b64._out.size()>0){
               out = b64._out[0];
               b64._out.erase(b64._out.begin());
               return out;
            }
            switch(b64._mode){
               case 0:
                     b64._tmp = END_OF_STRING;
                     b64._mode=4;
                     return b64._tmp;
               case 1:
                  if(b64._tmp!=END_OF_STRING){
                     out = _chars[(b64._tmp<<4)&0x3f];
                     b64._tmp = END_OF_STRING;
                  }
                  b64._mode++;
                  break;
               case 2:
                  if(b64._tmp!=END_OF_STRING){
                     out = _chars[(b64._tmp<<2)&0x3f];
                     b64._tmp = END_OF_STRING;
                  }
                  b64._mode++;
                  break;
               case 3:
                  if(b64._tmp!=END_OF_STRING){
                     out = _chars[(b64._tmp)&0x3f];
                     b64._tmp = END_OF_STRING;
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
   cout << "source: " << s << endl;
   size_t x=0;
   uint8_t r;
   cout << "output: ";
   while(true)/*void task_loop()*/{
      if(s[x]!=END_OF_STRING){
         if(Base64::write(b64e,s[x]))
            x++;
      }
      if(Base64::available(b64e)){
         r = Base64::read(b64e);
         if(r==END_OF_STRING) break;
         cout << r;
      }
   }
   cout << endl;
}
