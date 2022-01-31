// BensSim.hpp -- Bens specific routines for handling a SIM800
//
#ifndef _BENS_SIM_HPP
#define _BENS_SIM_HPP

#include <platform.hpp>

#include <SIM800.h>
#include "BensMessage.hpp"
#include "BensMessageFactory.hpp"
#include <list>

class BensSim
{
public:
   BensSim(SIM800 *sim, char *simPin);
   void begin();
   void loop();
   std::list<std::shared_ptr<BensMessage>> getMessages()
   {
      LOG_ENTRY();
      getAllSMSes();
      LOG_EXIT();
      return messages;
   }
   void getTime(int &h, int &m, int &s);

private:
   void setupModem();
   bool isOK();
   void reportIfError(const char *msg);
   void handleUnsolicitedMessage(char *buffer);
   void handleUnsolicited();
   bool sayHello();
   bool completelyReady();
   void setupGSM(const char *pincode);
   void sendSMS(char *recipientNumber, char *sms);
   void storeMessage(char *sender, char *sentAt, char *message);
   bool handleSMSintro(char *line, char **status, char **sender, char **sentAt);
   void extractAllMessages(char *buffer);
   void getAllSMSes();

   static bool initDone;
   static std::list<std::shared_ptr<BensMessage>> messages;
   static MessageID highestID;
   static char pincode[20];
   static SIM800 *pSIM;
   static bool smsReady;
   static bool callReady;
   static int addDST;
};

#endif
