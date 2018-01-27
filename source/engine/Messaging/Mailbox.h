#pragma once

#include <vector>

#include "Message.h"

namespace EngineXYZ {
  namespace Messaging {

   class Mailbox {
   public:
      Mailbox(int id);
      ~Mailbox();

      void sendMessage(const Message& message);
      Message* readNextMessage();
      void deleteReadMessages();

      int mId;
      std::vector<Message*> mMessages;
    };
  }
}
