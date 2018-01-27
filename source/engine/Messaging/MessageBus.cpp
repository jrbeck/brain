#include "MessageBus.h"

namespace EngineXYZ {
  namespace Messaging {
    MessageBus::MessageBus() :
      mNextMailboxId(0),
      mNextChannelId(0)
    {}

    MessageBus::~MessageBus() {
      size_t numMailboxes = mMailboxes.size();
      for (size_t i = 0; i < numMailboxes; ++i) {
        delete mMailboxes[i];
      }

      size_t numChannels = mChannels.size();
      for (size_t i = 0; i < numChannels; ++i) {
        delete mChannels[i];
      }
    }

    // int MessageBus::newChannel();
    // void MessageBus::deleteChannel(int channelId);

    int MessageBus::newMailbox() {
      Mailbox* mailbox = new Mailbox(mNextMailboxId);
      mMailboxes.push_back(mailbox);
      return mNextMailboxId++;
    }

    void MessageBus::deleteMailbox(int mailboxId) {
      size_t numMailboxes = mMailboxes.size();
      for (size_t i = 0; i < numMailboxes; ++i) {
        if (mMailboxes[i]->mId == mailboxId) {
          if (i == numMailboxes - 1) {
            mMailboxes.pop_back();
            return;
          }
          std::swap(mMailboxes[i], mMailboxes[numMailboxes - 1]);
          mMailboxes.pop_back();
          return;
        }
      }
    }

    // int MessageBus::subscribeToChannel(int mailboxId, int channelId);
    // void MessageBus::unsubscribeFromChannel(int mailboxId, int channelId);

    int MessageBus::sendMessage(int mailboxId, const Message& message) {
      Mailbox* mailbox = findMailbox(mailboxId);
      if (mailbox == nullptr) {
        return MESSAGING_COULD_NOT_FIND_SUBSCRIBER;
      }
      mailbox->sendMessage(message);
      return 0;
    }

    // int MessageBus::sendMessageOnChannel(int channelId, const Message& message);

    Message* MessageBus::readNextMessage(int mailboxId) {
      Mailbox* mailbox = findMailbox(mailboxId);
      if (mailbox == nullptr) {
        return nullptr;
      }
      return mailbox->readNextMessage();
    }

    void MessageBus::deleteReadMessages() {
      size_t numMailboxes = mMailboxes.size();
      for (size_t i = 0; i < numMailboxes; ++i) {
        mMailboxes[i]->deleteReadMessages();
      }
    }

    Mailbox* MessageBus::findMailbox(int id) {
      size_t numMailboxes = mMailboxes.size();
      for (size_t i = 0; i < numMailboxes; ++i) {
        if (mMailboxes[i]->mId == id) {
          return mMailboxes[i];
        }
      }
      return nullptr;
    }
  }
}
