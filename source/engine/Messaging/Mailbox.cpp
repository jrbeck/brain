#include "Mailbox.h"

namespace EngineXYZ {
  namespace Messaging {
    Mailbox::Mailbox(int id) :
      mId(id)
    {}

    Mailbox::~Mailbox() {
      size_t numMessages = mMessages.size();
      for (size_t i = 0; i < numMessages; ++i) {
        delete mMessages[i];
      }
    }

    void Mailbox::sendMessage(const Message& message) {
      Message* newMessage = new Message;
      *newMessage = message;
      newMessage->mIsRead = false;
      mMessages.push_back(newMessage);
    }

    Message* Mailbox::readNextMessage() {
      size_t numMessages = mMessages.size();
      for (size_t i = 0; i < numMessages; ++i) {
        if (!mMessages[i]->mIsRead) {
          mMessages[i]->mIsRead = true;
          return mMessages[i];
        }
      }
      return nullptr;
    }

    void Mailbox::deleteReadMessages() {
      size_t numMessages = mMessages.size();
      for (size_t i = 0; i < numMessages; ++i) {
        if (mMessages[i]->mIsRead) {
          if (i == numMessages - 1) {
            mMessages.pop_back();
          }
          else {
            std::swap(mMessages[i], mMessages[numMessages--]);
            mMessages.pop_back();
          }
        }
      }
    }
  }
}
