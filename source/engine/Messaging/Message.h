#pragma once

#include "cstdio"

namespace EngineXYZ {
  namespace Messaging {

    class Message {
    public:
      Message() {};
      ~Message() {};

      void print();

      int mSenderId;
      int mType;
      int mIntValue;
      bool mIsRead;
    };
  }
}
