#include "debug.h"

char _log_diag[HTTP_LOG_SIZE] ={0};
unsigned int log_cur_index = 0;

/*/
template< typename _A, typename _B > bool CompareType( _A a, _B b )
{
  return false;
}

template< typename _A > bool CompareType( _A a, _A b )
{
  return true;
}/**/

template<typename T> void Log(T x){
  if(Serial)
    Serial.print(x);
  else{
    /**
    if(typeof(x)==typeof(String)){
      LOG(static_cast<String>(x).c_str());
    }
    else if(typeof(x)==typeof(const char*)){
      LOG(static_cast<const char*>(x));
    }
    else if(typeof(x)==typeof(char*)){
      LOG(static_cast<char*>(x));
    }
    else if(typeof(x)==typeof(int)){
      char y[20] = {0};
      LOG(itoa(static_cast<int>(x),*y[0],10));
    }
    else if(typeof(x)==typeof(unsigned int)){
      char y[20] = {0};
      LOG(itoa(static_cast<unsigned int>(x),*y[0],10));
    }
    else if(typeof(x)==typeof(long)){
      char y[30] = {0};
      LOG(ltoa(static_cast<long>(x),*y[0],10));
    }
    else if(typeof(x)==typeof(unsigned long)){
      char y[30] = {0};
      LOG(itoa(static_cast<unsigned long>(x),*y[0],10));
    }
    else {} //No matching type
    /**/
  }
}
template<typename T> void Logln(T x){
  if(Serial)
    Serial.println(x);
}
void Logln(){
  if(Serial)
    Serial.println();
  else LOG("\n");
}
void Log(const char x[]){
  if(Serial)
    Serial.print(x);
  else LOG(x);
}
void Logln(const char x[]){
  if(Serial)
      Serial.println(x);
  else {LOG(x);LOG("\n");}
}
void Log(const char x[],unsigned int n){
  char z[n+1];
  memcpy(z,x,n);
  z[n-1]=0;
  if(Serial) Serial.print(z);
  else LOG(z);
}
void Logln(const char x[],unsigned int n){
  char z[n+2];
  memcpy(z,x,n);
  z[n-2]='\n';
  z[n-1]=0;
  if(Serial) Serial.print(z);
  else LOG(z);
}

void LOG(const char text[]){
    unsigned int i;
    for(i=0; i < HTTP_LOG_SIZE;i++) if(text[i]==0)break;
    LOG(text,i);
}
void LOG(const char text[], unsigned int n){
    if(n + log_cur_index > HTTP_LOG_SIZE){
      n = HTTP_LOG_SIZE - log_cur_index - 1; //Take account the null terminator
    }
    memcpy(_log_diag+log_cur_index,text,n);
    _log_diag[log_cur_index+n] = 0;
    log_cur_index += n;
}
void resetLOG(){
    memset(_log_diag,0, log_cur_index);
    log_cur_index = 0;
}
