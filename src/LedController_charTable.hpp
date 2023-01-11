#pragma once
#include "LedController_byterow.hpp"
namespace sakurajin {

    class charTable:public ByteRow<128> {
      public:
        charTable(byte newdata[128]) noexcept:ByteRow<128>(newdata) {};

        charTable(
            byte n0,   byte n1,   byte n2,   byte n3,   byte n4,   byte n5,   byte n6,   byte n7,   byte n8,   byte n9,
            byte n10,  byte n11,  byte n12,  byte n13,  byte n14,  byte n15,  byte n16,  byte n17,  byte n18,  byte n19,
            byte n20,  byte n21,  byte n22,  byte n23,  byte n24,  byte n25,  byte n26,  byte n27,  byte n28,  byte n29,
            byte n30,  byte n31,  byte n32,  byte n33,  byte n34,  byte n35,  byte n36,  byte n37,  byte n38,  byte n39,
            byte n40,  byte n41,  byte n42,  byte n43,  byte n44,  byte n45,  byte n46,  byte n47,  byte n48,  byte n49,
            byte n50,  byte n51,  byte n52,  byte n53,  byte n54,  byte n55,  byte n56,  byte n57,  byte n58,  byte n59,
            byte n60,  byte n61,  byte n62,  byte n63,  byte n64,  byte n65,  byte n66,  byte n67,  byte n68,  byte n69,
            byte n70,  byte n71,  byte n72,  byte n73,  byte n74,  byte n75,  byte n76,  byte n77,  byte n78,  byte n79,
            byte n80,  byte n81,  byte n82,  byte n83,  byte n84,  byte n85,  byte n86,  byte n87,  byte n88,  byte n89,
            byte n90,  byte n91,  byte n92,  byte n93,  byte n94,  byte n95,  byte n96,  byte n97,  byte n98,  byte n99,
            byte n100, byte n101, byte n102, byte n103, byte n104, byte n105, byte n106, byte n107, byte n108, byte n109,
            byte n110, byte n111, byte n112, byte n113, byte n114, byte n115, byte n116, byte n117, byte n118, byte n119,
            byte n120, byte n121, byte n122, byte n123, byte n124, byte n125, byte n126, byte n127
        ) noexcept {
            _data[0] = n0;
            _data[1] = n1;
            _data[2] = n2;
            _data[3] = n3;
            _data[4] = n4;
            _data[5] = n5;
            _data[6] = n6;
            _data[7] = n7;
            _data[8] = n8;
            _data[9] = n9;

            _data[10] = n10;
            _data[11] = n11;
            _data[12] = n12;
            _data[13] = n13;
            _data[14] = n14;
            _data[15] = n15;
            _data[16] = n16;
            _data[17] = n17;
            _data[18] = n18;
            _data[19] = n19;

            _data[20] = n20;
            _data[21] = n21;
            _data[22] = n22;
            _data[23] = n23;
            _data[24] = n24;
            _data[25] = n25;
            _data[26] = n26;
            _data[27] = n27;
            _data[28] = n28;
            _data[29] = n29;

            _data[30] = n30;
            _data[31] = n31;
            _data[32] = n32;
            _data[33] = n33;
            _data[34] = n34;
            _data[35] = n35;
            _data[36] = n36;
            _data[37] = n37;
            _data[38] = n38;
            _data[39] = n39;

            _data[40] = n40;
            _data[41] = n41;
            _data[42] = n42;
            _data[43] = n43;
            _data[44] = n44;
            _data[45] = n45;
            _data[46] = n46;
            _data[47] = n47;
            _data[48] = n48;
            _data[49] = n49;

            _data[50] = n50;
            _data[51] = n51;
            _data[52] = n52;
            _data[53] = n53;
            _data[54] = n54;
            _data[55] = n55;
            _data[56] = n56;
            _data[57] = n57;
            _data[58] = n58;
            _data[59] = n59;

            _data[60] = n60;
            _data[61] = n61;
            _data[62] = n62;
            _data[63] = n63;
            _data[64] = n64;
            _data[65] = n65;
            _data[66] = n66;
            _data[67] = n67;
            _data[68] = n68;
            _data[69] = n69;

            _data[70] = n70;
            _data[71] = n71;
            _data[72] = n72;
            _data[73] = n73;
            _data[74] = n74;
            _data[75] = n75;
            _data[76] = n76;
            _data[77] = n77;
            _data[78] = n78;
            _data[79] = n79;

            _data[80] = n80;
            _data[81] = n81;
            _data[82] = n82;
            _data[83] = n83;
            _data[84] = n84;
            _data[85] = n85;
            _data[86] = n86;
            _data[87] = n87;
            _data[88] = n88;
            _data[89] = n89;

            _data[90] = n90;
            _data[91] = n91;
            _data[92] = n92;
            _data[93] = n93;
            _data[94] = n94;
            _data[95] = n95;
            _data[96] = n96;
            _data[97] = n97;
            _data[98] = n98;
            _data[99] = n99;

            _data[100] = n100;
            _data[101] = n101;
            _data[102] = n102;
            _data[103] = n103;
            _data[104] = n104;
            _data[105] = n105;
            _data[106] = n106;
            _data[107] = n107;
            _data[108] = n108;
            _data[109] = n109;

            _data[110] = n110;
            _data[111] = n111;
            _data[112] = n112;
            _data[113] = n113;
            _data[114] = n114;
            _data[115] = n115;
            _data[116] = n116;
            _data[117] = n117;
            _data[118] = n118;
            _data[119] = n119;

            _data[120] = n120;
            _data[121] = n121;
            _data[122] = n122;
            _data[123] = n123;
            _data[124] = n124;
            _data[125] = n125;
            _data[126] = n126;
            _data[127] = n127;
        };

        virtual byte getChar(char c) const noexcept {
            byte index = ((byte)c)%128;
            return this->at(index);
        }
    };


}