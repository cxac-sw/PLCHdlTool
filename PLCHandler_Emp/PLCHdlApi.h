#ifndef PLCHDLAPI_H__
#define PLCHDLAPI_H__

#include "iostream"
#include "list"
#include "vector"
#include "stdio.h"
#define MAX_PLC_VARIANT_NUM  40

int PHA_Demo(unsigned int id);

//对应CoDeSys中常用的变量类型, 参考CmpStd.h
typedef enum tagE_PLC_TYPE{
    EPC_INVALID,
    EPC_BOOL,							// typedef RTS_I8		RTS_IEC_BOOL,
    EPC_SINT,							// typedef RTS_I8		RTS_IEC_SINT,
    EPC_USINT,							// typedef RTS_UI8		RTS_IEC_USINT,
    EPC_BYTE,							// typedef RTS_UI8		RTS_IEC_BYTE,
    EPC_INT,							// typedef RTS_I16		RTS_IEC_INT,
    EPC_UINT,							// typedef RTS_UI16		RTS_IEC_UINT,
    EPC_WORD,							// typedef RTS_UI16		RTS_IEC_WORD,
    EPC_DINT,							// typedef RTS_I32		RTS_IEC_DINT,
    EPC_UDINT,							// typedef RTS_UI32		RTS_IEC_UDINT,
    EPC_DWORD,							// typedef RTS_UI32		RTS_IEC_DWORD,

    EPC_TIME,							// typedef RTS_UI32		RTS_IEC_TIME,
    EPC_TIME_OF_DAY,					// typedef RTS_UI32		RTS_IEC_TIME_OF_DAY,
    EPC_TOD,							// typedef RTS_UI32		RTS_IEC_TOD,
    EPC_DATE_AND_TIME,					// typedef RTS_UI32		RTS_IEC_DATE_AND_TIME,
    EPC_DT,								// typedef RTS_UI32		RTS_IEC_DT,
    EPC_DATE,							// typedef RTS_UI32		RTS_IEC_DATE,

    EPC_LINT,							// typedef RTS_I64		RTS_IEC_LINT,
    EPC_ULINT,							// typedef RTS_UI64		RTS_IEC_ULINT,
    EPC_LWORD,							// typedef RTS_UI64		RTS_IEC_LWORD,
    EPC_LTIME							// typedef RTS_UI64		RTS_IEC_LTIME,
}E_PLC_TYPE;

typedef union{
    unsigned char u8v;
    unsigned short u16v;
    unsigned int   u32v;
    unsigned long long u64v;
}T_PLC_VAL;


class CPLCVar
{
public:

    CPLCVar(char * str, unsigned int u32size){
        clear();
        name = std::string(str);
        size = u32size;
    }
    CPLCVar(){
        clear();
    }

    void operator=(const CPLCVar & c){
        name = c.name;
        etype = c.etype;
        size  = c.size;
        val   = c.val;
        wr    = c.wr;
    }

    std::string name;
    E_PLC_TYPE etype;
    unsigned int size;
    T_PLC_VAL val;
    bool     wr;  // false: Read, true：write
private:
    void clear(){
        name.clear();
        etype = EPC_INVALID;
        val.u64v = 0;
        wr = false;
    }
};


class __declspec(dllexport) CPLCHandleApi
{
public:
    CPLCHandleApi(std::string  ini);
    CPLCHandleApi();
    ~CPLCHandleApi();

    bool StartProc();
    bool StopProc();
    bool GetVarInfo(int index, CPLCVar * pVar);
    int  TrigWriteAll();
    int  TrigReadAll();
    unsigned int GetAllVariantSymbols();
    int    GetPlcHandleState();

    void errlog(std::string errstr);
    bool readlog;

};

extern "C" void * GetPLCHandleApiCls();
#endif
