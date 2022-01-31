//
// BensSim.cpp -- Sim800 specific support for GuardianAngel
//
// BSla, 5 dec 2021,

// Please select the corresponding model

// #define SIM800L_IP5306_VERSION_20190610
// #define SIM800L_AXP192_VERSION_20200327
#define SIM800C_AXP192_VERSION_20200609
// #define SIM800L_IP5306_VERSION_20200811

#include <string.h>
#include "platform.hpp"
#include "DateTime.hpp"

#define DEBUG
static bool debug = false;
static bool debugSpecial = false;

#include <SIM800.h>
#include "BensSim.hpp"

#define MODEM_PWRKEY 4
#define MODEM_POWER_ON 25
#define MODEM_TX 27
#define MODEM_RX 26
#define LED_GPIO 12
#define LED_ON LOW
#define LED_OFF HIGH

#define SerialMon Serial // for debugging

// Set serial for AT commands (to the module)
#define SerialAT Serial1

bool BensSim::initDone = false;
char BensSim::pincode[20];
SIM800 *BensSim::pSIM;
bool BensSim::smsReady = false;
bool BensSim::callReady = false;
int BensSim::addDST = 0;
int BensSim::highestID = 0;
std::list<std::shared_ptr<BensMessage>> BensSim::messages;

//--------------------------------------
BensSim::BensSim(SIM800 *sim, char *simPin)
{
    if (!initDone)
    {
        initDone = true;
        smsReady = false;
        callReady = false;
        addDST = 0;
        pSIM = sim;
        strncpy(pincode, simPin, 19);
    }
}

//---------------------------
void BensSim::begin()
{
    setupGSM(pincode);
    // getAllSMSes ();
}

//---------------------------
void BensSim::loop()
{
    getAllSMSes();
}

//-------------------------------------
void BensSim::setupModem()
// setup SIM800 chip
{
#ifdef MODEM_RST
    // Keep reset high
    pinMode(MODEM_RST, OUTPUT);
    digitalWrite(MODEM_RST, HIGH);
#endif

    pinMode(MODEM_PWRKEY, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);

    // Turn on the Modem power first
    digitalWrite(MODEM_POWER_ON, HIGH);

    // Pull down PWRKEY for more than 1 second according to manual requirements
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);

    // Initialize the indicator as an output
    pinMode(LED_GPIO, OUTPUT);
    digitalWrite(LED_GPIO, LED_OFF);

    // give modem time to wake up
    delay(4000);
}

//-----------------------------
bool BensSim::isOK()
{
    return (pSIM->reply("OK"));
}

//----------------------
void BensSim::reportIfError(const char *msg)
{
    if (pSIM->isError())
    {
        SerialMon.print("-------ERROR--------\n");
        SerialMon.print(msg);
        SerialMon.print(" yielded: ");
        SerialMon.print(pSIM->getBuffer());
        SerialMon.print("\n--------------------\n");
    }
}

//----------------------
void BensSim::handleUnsolicitedMessage(char *buffer)
{
    char *token = strtok(buffer, "\r\n");
    while (token != NULL)
    {
        if (strcmp(token, "Call Ready") == 0)
        {
            callReady = true;
        }
        else if (strcmp(token, "SMS Ready") == 0)
        {
            smsReady = true;
        }
        else if (strcmp(token, "*PSUTTZ: ") == 0)
        {
            token += 8;
            // DateTime dt (token, addDST);
        }
        else
        {
            sscanf(token, "DST: %d", &addDST);
            if ((addDST < 0) || (addDST > 2))
                addDST = 0;
        }

        token = strtok(NULL, "\r\n");
    }
}

//--------------------
void BensSim::getTime(int &h, int &m, int &s)
{
    LOG_ENTRY();
    int addDST = 0;
    pSIM->formatReply(true);
    LOG("Sending +CCLK");
    pSIM->clock(GET);
    if (pSIM->reply("+CCLK"))
    {
        char *buffer = strstr(pSIM->getBuffer(), "+CCLK: \"");
        char *token = buffer + strlen("+CCLK: \"");
        LOG("token: %s", token);
        std::string myTime(token);
        LOG("myTime: %s", myTime.c_str());
        DateTime dt(myTime);
        LOG("dt: %s", dt.toString().c_str());
        dt.getHMS(h, m, s);
        LOG("h: %d, m: %d, s: %d", h, m, s);
    }
    else
    {
        LOG("No +CCLK reply received.");
    }
    LOG("+CCLK reply received.");

    // token = strtok(NULL, "\r\n");
    LOG_EXIT();
}

//----------------------
void BensSim::handleUnsolicited()
{
    if (!pSIM->available())
        return;
    pSIM->formatReply(false);
    while (pSIM->available())
    {
        if (pSIM->reply("TIMEOUT"))
        {
            pSIM->clearBuffer();
            break;
        }

        handleUnsolicitedMessage(pSIM->getBuffer());
        pSIM->clearBuffer();
    }
    pSIM->formatReply(true);
}

//----------------------
bool BensSim::sayHello()
// returns: T if success
{
    bool success = true;
    pSIM->clearBuffer();

    pSIM->setTimeout(500); // reply timeout short

    const int maxi = 20;
    int i = 0;
    do
    {
        char at[3];
        strcpy(at, "AT");
        pSIM->print(at);
        if (isOK())
            break;
    } while (i++ < maxi);

    if (i >= maxi)
    {
        SerialMon.print("\n****** ERROR: sayHello: no OK reply\n");
        success = false;
    }
    pSIM->resetTimeout(); // normal timeout
    return success;
}

//----------------------
bool BensSim::completelyReady()
{
    return smsReady && callReady;
}

//---------------------------
void BensSim::setupGSM(const char *pincode)
{
    // Set GSM module baud rate and UART pins
    pSIM->begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

    setupModem();
    sayHello();

    pSIM->deviceError(SET, "2");
    reportIfError("SetDeviceError (2)");
    pSIM->pinCode(GET);
    if (pSIM->reply("SIM PIN"))
    {
        pSIM->pinCode(SET, pincode);
        reportIfError("Set Sim Pin");
    }
    else
    {
        reportIfError("GetSimPin");
    }

    pSIM->clearBuffer();
    while (!completelyReady())
        handleUnsolicited();

    pSIM->smsFormat(SET, "1"); // expanded sms'es
    reportIfError("Set SMS format 1");
}

//---------------------------
void BensSim::sendSMS(char *recipientNumber, char *sms)
{
    const int maxLen = 153;
    char buffer[maxLen + 1];
    int nSMS = 0;
    while ((strlen(sms)) > 0)
    {
        strncpy(buffer, sms, maxLen);
        pSIM->smsSend(recipientNumber, buffer);
        reportIfError("SMS Send");
        sms += strlen(buffer);

        if (++nSMS > 3)
            break;
    }
}

//---------------------------
void BensSim::storeMessage(char *sender, char *sentAt, char *message)
{
    std::string stringSender(sender);
    std::string stringBody(message);
    std::string dtsIn(sentAt);

    DateTime dtSentAt(dtsIn);

    std::shared_ptr<BensMessage> thisMessage = BensMessageFactory::createBensMessage(highestID++, dtSentAt, stringSender, stringBody);
    messages.push_back(thisMessage);

    static int messageNbr = 0;

    if (debugSpecial)
    {
        SerialMon.print("--------------\nStore Message ");
        SerialMon.print(++messageNbr);
        SerialMon.println(":");
        SerialMon.print("From: ");
        SerialMon.println(sender);

        std::string dts = dtSentAt.toString();
        SerialMon.print("Sent at: ");
        SerialMon.println(dts.c_str());

        SerialMon.println("Message:");
        SerialMon.print(message);
        SerialMon.println("-------");
    }
}

//---------------------------------
bool BensSim::handleSMSintro(char *line, char **status, char **sender, char **sentAt)
// returns T: success
{
    bool success = true;
    char *_cmd = NULL;
    char *_status = NULL;
    char *_sender = NULL;
    char *_dooa = NULL;
    char *_sentAt = NULL;

    char *prev = line;
    char *pos = strstr(line, ",");
    if (pos)
    { // do command
        _cmd = prev;
        *pos++ = '\0';
        prev = pos;
    }
    _cmd = _cmd;

    pos = strstr(pos, ",");
    if (pos)
    { // do status
        _status = prev;
        *pos++ = '\0';
        prev = pos;
    }
    pos = strstr(pos, ",");
    if (pos)
    { // do sender
        _sender = prev;
        *pos++ = '\0';
        prev = pos;
    }
    pos = strstr(pos, ",");
    if (pos)
    { // do crap and time of submit
        _dooa = prev;
        *pos++ = '\0';
        _sentAt = pos;
    }
    _dooa = _dooa;

    // Remove quotes from sender
    _sender[strlen(_sender) - 1] = '\0';
    _sender += 1;

    // Remove quotes from sentAt
    _sentAt[strlen(_sentAt) - 1] = '\0';
    _sentAt += 1;

    *status = _status;
    *sender = _sender;
    *sentAt = _sentAt;
    return success;
}

//---------------------------
void BensSim::extractAllMessages(char *buffer)
{
    char *b_store;
    char *line;
    const char *crlf = "\r\n";
    const char *newSms = "+CMGL: ";
    const char *endSms = "OK";
    bool success = false;
    bool inProgress = false;

    char *status;
    char *sender;
    char *dateTime;
    char *message;

    const int bufferSz = 500;
    char messageBuffer[bufferSz];
    strcpy(messageBuffer, "");
    message = messageBuffer;

    line = strtok_r(buffer, crlf, &b_store);
    while (line != NULL)
    {
        if (strstr(line, newSms) == line)
        { // it is an SMS intro line
            if (inProgress)
            {
                // the previous SMS is now complete
                storeMessage(sender, dateTime, message);
                inProgress = false;
                strcpy(messageBuffer, "");
            }
            handleSMSintro(line, &status, &sender, &dateTime);
            inProgress = true;
        }
        else if (strstr(line, endSms) == line)
        { // all SMS'es done
            success = true;
            break;
        }
        else if (strstr(line, "AT+CMGL=") == line)
        {
            // skip this line
        }
        else
        { // it is the first or next line of an SMS message
            strncat(messageBuffer, line, bufferSz - strlen(buffer) - 5);
            strcat(messageBuffer, "\n");
        }
        // read next line
        line = strtok_r(NULL, crlf, &b_store);
    }

    if (success)
    {
        // we must still handle the final SMS
        if (inProgress)
        {
            // the previous SMS is now complete
            storeMessage(sender, dateTime, message);
            inProgress = false;
        }
    }
}

//-----------------------
void BensSim::getAllSMSes()
{

    pSIM->clearBuffer();

    if (debug)
        SerialMon.println("Handle unsolicited messages");
    handleUnsolicited();

    if (debug)
        SerialMon.println("Getting list of all SMS'es (smsList)...");
    pSIM->formatReply(false);
    pSIM->smsList(SET, "\"ALL\"");
    reportIfError("smsList");

    if (isOK())
    {
        if (debug)
            SerialMon.println("Extract messages");
        extractAllMessages(pSIM->getBuffer());
        if (debug)
            SerialMon.println("extract done");
    }
}