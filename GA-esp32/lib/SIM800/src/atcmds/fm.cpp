/*
Written by Mortuus Medicus

This follows the newer manual that adds the featues below
*/

#include <SIM800.h>

    /* AT+FMOPEN     6   */    void fmOpen                  (CmdType, const char* = "");
    /* AT+FMCLOSE    7   */    void fmClose                 (CmdType);
    /* AT+FMFREQ     7   */    void fmSetFrequency          (CmdType, const char* = "");
    /* AT+FMVOLUME   7   */    void fmSetVolume             (CmdType, const char* = "");
    /* AT+FMSCAN     8   */    void fmAutoSearchChannel     (CmdType);
    /* AT+FMSIGNAL   8   */    void fmQuerySignalLevel      (CmdType, const char* = "");
    /* AT+FMVALID    9   */    void fmCheckFrequencyValid   (CmdType, const char* = "");

#ifdef FMFUNCS
// ============================================================
void SIM800::fmOpen(CmdType type, const char* str) {
    outBuilder(type, str, P("FMOPEN"));
    print(ioBuffer);
}

// ============================================================
void SIM800::fmClose(CmdType type) {
    outBuilder(type, "", P("FMCLOSE"));
    print(ioBuffer);
}

// ============================================================
void SIM800::fmSetFrequency(CmdType type, const char* str) {
    outBuilder(type, str, P("FMFREQ"));
    print(ioBuffer);
}

// ============================================================
void SIM800::fmSetVolume(CmdType type, const char* str) {
    outBuilder(type, str, P("FMVOLUME"));
    print(ioBuffer);
}

// ============================================================
void SIM800::fmAutoSearchChannel(CmdType type) {
    outBuilder(type, "", P("FMSCAN"));
    print(ioBuffer);
}

// ============================================================
void SIM800::fmQuerySignalLevel(CmdType type, const char* str) {
    outBuilder(type, str, P("FMSIGNAL"));
    print(ioBuffer);
}

// ============================================================
void SIM800::fmCheckFrequencyValid(CmdType type, const char* str) {
    outBuilder(type, str, P("FMVALID"));
    print(ioBuffer);
}

#endif