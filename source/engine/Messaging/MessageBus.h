#pragma once

#include <algorithm>
#include <vector>

#include "Message.h"
#include "Mailbox.h"

#define MESSAGING_COULD_NOT_FIND_SUBSCRIBER (1)

// TODO: move this somewhere reasonable
enum MESSAGE_TYPES {
  MESSAGE_TYPE_CONTROLLER_QUIT,
  MESSAGE_TYPE_ENTER_TRAINING,
  MESSAGE_TYPE_ENTER_LOBBY
};

namespace EngineXYZ {
  namespace Messaging {

    struct Channel {
      std::vector<int> mMailboxIds;
    };

    class MessageBus {
    public:
      MessageBus();
      ~MessageBus();

      // int newChannel();
      // void deleteChannel(int channelId);

      int newMailbox();
      void deleteMailbox(int mailboxId);

      // int subscribeToChannel(int mailboxId, int channelId);
      // void unsubscribeFromChannel(int mailboxId, int channelId);

      int sendMessage(int mailboxId, const Message& message);
      // int sendMessageOnChannel(int channelId, const Message& message);

      Message* readNextMessage(int mailboxId);
      void deleteReadMessages();

    private:
      Mailbox* findMailbox(int id);

      int mNextMailboxId;
      int mNextChannelId;

      std::vector<Mailbox*> mMailboxes;
      std::vector<Channel*> mChannels;
    };
  }
}
