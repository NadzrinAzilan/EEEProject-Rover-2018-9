#ifndef DEBUG_H
#define DEBUG_H

//Debug functions
template<typename T> void Log(T);
template<typename T> void Logln(T);
void Log(const char[]);
void Logln(const char[]);
void Log(const char[],unsigned int);
void Logln(const char[],unsigned int);
void Logln();


void LOG(const char[]);
void LOG(const char[], unsigned int);
const unsigned int HTTP_LOG_SIZE = 10240u;
extern char _log_diag[HTTP_LOG_SIZE];
extern unsigned int log_cur_index;
void resetLOG();

#endif
