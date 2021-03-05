#pragma once

#include "LedController_charTable.hpp"

namespace sakurajin {
    // the opcodes for the MAX7221 and MAX7219
    class MAX72XX {
      private:
        charTable table;

        static const MAX72XX& getInstance() noexcept {
            static MAX72XX instance = MAX72XX();
            return instance;
        }

        MAX72XX() noexcept;

      public:
        static const unsigned int OP_NOOP = 0;
        static const unsigned int OP_DIGIT0 = 1;
        static const unsigned int OP_DIGIT1 = 2;
        static const unsigned int OP_DIGIT2 = 3;
        static const unsigned int OP_DIGIT3 = 4;
        static const unsigned int OP_DIGIT4 = 5;
        static const unsigned int OP_DIGIT5 = 6;
        static const unsigned int OP_DIGIT6 = 7;
        static const unsigned int OP_DIGIT7 = 8;
        static const unsigned int OP_DECODEMODE = 9;
        static const unsigned int OP_INTENSITY = 10;
        static const unsigned int OP_SCANLIMIT = 11;
        static const unsigned int OP_SHUTDOWN = 12;
        static const unsigned int OP_DISPLAYTEST = 15;

        static byte getChar(char c) noexcept {
            return getInstance().table.getChar(c);
        }

        static const charTable& getTable() noexcept {
            return getInstance().table;
        }
    };

}
