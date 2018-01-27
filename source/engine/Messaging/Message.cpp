#include "Message.h"

namespace EngineXYZ {
  namespace Messaging {
    void Message::print() {
      printf("Message ---------\n");
      printf("sender: %d\n", mSenderId);
      printf("type: %d\n", mType);
      printf("int value: %d\n", mIntValue);
      printf("---------\n");
    }
  }
}
