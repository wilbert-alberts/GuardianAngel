/*
Written by Mortuus Medicus

This follows the newer manual that adds the featues below
*/
/*
█████████████████████████████████████████████████████
████   ATcmd         Manpage        API Method   ████   Count: 7 commands
█████████████████████████████████████████████████████
*/
public:
    /* AT+FMOPEN     6   */    void fmOpen                  (CmdType, const char* = "");
    /* AT+FMCLOSE    7   */    void fmClose                 (CmdType);
    /* AT+FMFREQ     7   */    void fmSetFrequency          (CmdType, const char* = "");
    /* AT+FMVOLUME   7   */    void fmSetVolume             (CmdType, const char* = "");
    /* AT+FMSCAN     8   */    void fmAutoSearchChannel     (CmdType);
    /* AT+FMSIGNAL   8   */    void fmQuerySignalLevel      (CmdType, const char* = "");
    /* AT+FMVALID    9   */    void fmCheckFrequencyValid   (CmdType, const char* = "");