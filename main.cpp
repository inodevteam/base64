#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace crypto{

   #define END_OF_STRING '\0'
   #define NULL_CHAR '='
   static const string BASE64_ALPHABET="AZaz09++//";

   enum base64_method_t{
      BASE64_ENCODE,
      BASE64_DECODE
   };

   class Base64{
   private:
      static vector<uint8_t> _ev;
      static map<uint8_t,size_t> _dm;
      static void init() {
         if(_ev.size()!=64){
            _ev.clear();
            for(uint8_t m=0,c=BASE64_ALPHABET[m];
                m<BASE64_ALPHABET.length()&&c<=BASE64_ALPHABET[m+1];
                c=(c!=BASE64_ALPHABET[m+1]?c+=1:BASE64_ALPHABET[m+=2])
            ){
               _ev.push_back(c);
               _dm.insert(pair<uint8_t,size_t>(c,_dm.size()));
            }
         }
      }
      uint8_t _tmp;
      size_t _mode;
      const base64_method_t _method;
      vector<uint8_t> _out;
      Base64(const base64_method_t&_method):_method(_method), _mode(0){}
   public:
      static Base64 create(const base64_method_t&_method){
         init();
         return Base64(_method);
      }
      static const bool available(const Base64&b64){
         return b64._method==BASE64_ENCODE || (b64._method==BASE64_DECODE&&b64._out.size()>0);
      }
      static const bool write(Base64&b64, const uint8_t&c){
         bool next=true;
         if(c==END_OF_STRING){
            next=false;
            if(b64._method==BASE64_DECODE)
               b64._out.push_back(c);
            return next;
         }
         switch(b64._method){
            case BASE64_ENCODE:
               switch(b64._mode){
                  case 0:
                     b64._out.push_back(_ev[(c>>2)&0x3f]);
                     b64._mode++;
                     break;
                  case 1:
                     b64._out.push_back(_ev[((b64._tmp<<4)+((c>>4)&0xf))&0x3f]);
                     b64._mode++;
                     break;
                  case 2:
                     next=false;
                     b64._out.push_back(_ev[((b64._tmp<<2)+((c>>6)&0x3))&0x3f]);
                     b64._mode++;
                     break;
                  case 3:
                     b64._out.push_back(_ev[c&0x3f]);
                     b64._mode=0;
                     break;
               }
               break;
               case BASE64_DECODE:
                  if(_dm.find(c)==_dm.end()||c==NULL_CHAR){
                     b64._out.push_back(END_OF_STRING);
                     break;
                  }
                  switch(b64._mode){
                     case 0:
                        b64._mode++;
                        break;
                     case 1:
                        b64._out.push_back((char)(_dm[b64._tmp]<<2|_dm[c]>>4));
                        b64._mode++;
                        break;
                     case 2:
                        b64._out.push_back((char)(_dm[b64._tmp]<<4|_dm[c]>>2));
                        b64._mode++;
                        break;
                     case 3:
                       b64._out.push_back((char)(_dm[b64._tmp]<<6|_dm[c]));
                        b64._mode=0;
                        break;
                  }
                  break;
         }
         b64._tmp=c;
         return next;
      }
      static const uint8_t read(Base64&b64){
         uint8_t out=NULL_CHAR;
         if(b64._out.size()>0){
            out=b64._out[0];
            b64._out.erase(b64._out.begin());
            return out;
         }
         switch(b64._method){
            case BASE64_ENCODE:
               switch(b64._mode){
                  case 0:
                        out=END_OF_STRING;
                        break;
                  case 1:
                     if(b64._tmp!=END_OF_STRING){
                        out=_ev[(b64._tmp<<4)&0x3f];
                        b64._tmp=END_OF_STRING;
                     }
                     b64._mode++;
                     break;
                  case 2:
                     if(b64._tmp!=END_OF_STRING){
                        out=_ev[(b64._tmp<<2)&0x3f];
                        b64._tmp=END_OF_STRING;
                     }
                     b64._mode++;
                     break;
                  case 3:
                     if(b64._tmp!=END_OF_STRING){
                        out=_ev[(b64._tmp)&0x3f];
                        b64._tmp=END_OF_STRING;
                     }
                     b64._mode=0;
                     break;
                  }
               break;
         }
         return out;
      }
   };
   vector<uint8_t> Base64::_ev;
   map<uint8_t,size_t> Base64::_dm;

}

using namespace crypto;

int main() {
   Base64 b64e=Base64::create(/*BASE64_ENCODE*/BASE64_DECODE);
   string s=/*"Hello, world!"*/"SGVsbG8sIHdvcmxkIQ==";
   cout << "source: " << s << endl;
   size_t x=0;
   uint8_t r;
   cout << "output: ";
   while(true)/*void task_loop()*/{
      if(Base64::write(b64e,s[x]))
         x++/*in.read(x)*/;
      if(Base64::available(b64e)){
         r=Base64::read(b64e);
         if(r!=END_OF_STRING)
            cout << r/*out.write(r)*/;
         else break;
      }
   }
   cout << endl;
}
