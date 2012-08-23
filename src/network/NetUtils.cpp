
#include <iostream>

#include "NetUtils.hpp"

using std::string;
using std::vector;

namespace MCServer {
namespace Network {

/*
uint32_t codePointFromUtf8(const string &string, size_t &pos) {
#define IS_IN_RANGE(c, f, l)    (((c) >= (f)) && ((c) <= (l)))

    // TODO: since UTF-8 is a variable-length
    // encoding, you should pass in the input
    // buffer's actual byte length so that you
    // can determine if a malformed UTF-8
    // sequence would exceed the end of the buffer...

    uint8_t c1, c2;// *ptr = static_cast<uint8_t *>(p);
    uint32_t uc = 0;
    int seqlen;
    // int datalen = ... available length of p ...;    

    / *
    if( datalen < 1 )
    {
        // malformed data, do something !!!
        return (uint32_t) -1;
    }
    * /

    c1 = string[pos]; // ptr[0];

    if( (c1 & 0x80) == 0 )
    {
        uc = (uint32_t) (c1 & 0x7F);
        seqlen = 1;
    }
    else if( (c1 & 0xE0) == 0xC0 )
    {
        uc = (uint32_t) (c1 & 0x1F);
        seqlen = 2;
    }
    else if( (c1 & 0xF0) == 0xE0 )
    {
        uc = (uint32_t) (c1 & 0x0F);
        seqlen = 3;
    }
    else if( (c1 & 0xF8) == 0xF0 )
    {
        uc = (uint32_t) (c1 & 0x07);
        seqlen = 4;
    }
    else
    {
        // malformed data, do something !!!
        return (uint32_t) -1;
    }

    / *
    if( seqlen > datalen )
    {
        // malformed data, do something !!!
        return (uint32_t) -1;
    }
    * /

    for(int i = 1; i < seqlen; ++i)
    {
        c1 = string[pos + i];

        if( (c1 & 0xC0) != 0x80 )
        {
            // malformed data, do something !!!
            return (uint32_t) -1;
        }
    }

    switch( seqlen )
    {
        case 2:
        {
            c1 = string[pos];

            if( !IS_IN_RANGE(c1, 0xC2, 0xDF) )
            {
                // malformed data, do something !!!
                return (uint32_t) -1;
            }

            break;
        }

        case 3:
        {
            c1 = string[pos];
            c2 = string[pos + 1];

            if( ((c1 == 0xE0) && !IS_IN_RANGE(c2, 0xA0, 0xBF)) ||
                ((c1 == 0xED) && !IS_IN_RANGE(c2, 0x80, 0x9F)) ||
                (!IS_IN_RANGE(c1, 0xE1, 0xEC) && !IS_IN_RANGE(c1, 0xEE, 0xEF)) )
            {
                // malformed data, do something !!!
                return (uint32_t) -1;
            }

            break;
        }

        case 4:
        {
            c1 = string[pos];
            c2 = string[pos + 1];

            if( ((c1 == 0xF0) && !IS_IN_RANGE(c2, 0x90, 0xBF)) ||
                ((c1 == 0xF4) && !IS_IN_RANGE(c2, 0x80, 0x8F)) ||
                !IS_IN_RANGE(c1, 0xF1, 0xF3) )
            {
                // malformed data, do something !!!
                return (uint32_t) -1;
            }

            break;
        }
    }

    for(int i = 1; i < seqlen; ++i)
    {
        uc = ((uc << 6) | (uint32_t)(string[pos + i] & 0x3F));
    }

    pos += seqlen;
    return uc; 
} 

vector<uint16_t> utf8ToUsc2(const string &string) {
    vector<uint16_t> result;
    result.reserve(string.length());
    uint32_t codepoint;
    for (size_t position = 0; position < string.length();) {
        codepoint = codePointFromUtf8(string, position);
        // error
        if (codepoint == (uint32_t) -1) {
            break;
        }
        if (codepoint < 0xFFFE && (codepoint < 0xD800 || codepoint > 0xDFFF)) {
            result.push_back(static_cast<uint16_t>(codepoint));
        }
        else {
            result.push_back(0xFFFD); // replacement character.
            
        }
    }
    return result;
}


string usc2ToUtf8(const vector<uint16_t> &src)
{
    uint16_t ch;
    unsigned int i = 0;
//    char *dest_end = dest + sz;
    string result;

    while (i < src.size()) {
        ch = src[i];
        if (ch < 0x80) {
//            if (dest >= dest_end)
//                return i;
            result += (char)ch;
        }
        else if (ch < 0x800) {
//            if (dest >= dest_end-1)
//                return i;
            result += (ch>>6) | 0xC0;
            result += (ch & 0x3F) | 0x80;
        }
        else if (ch < 0x10000) {
//            if (dest >= dest_end-2)
//                return i;
            result += (ch>>12) | 0xE0;
            result += ((ch>>6) & 0x3F) | 0x80;
            result += (ch & 0x3F) | 0x80;
        }
        else if (ch < 0x110000) {
//            if (dest >= dest_end-3)
//                return i;
            result += (ch>>18) | 0xF0;
            result += ((ch>>12) & 0x3F) | 0x80;
            result += ((ch>>6) & 0x3F) | 0x80;
            result += (ch & 0x3F) | 0x80;
        }
        i++;
    }
//    if (dest < dest_end)
//        *dest = '\0';
    return result;
} */


bool isBigEndian() {
    unsigned char endianTest[] = {1, 0};
    short x = *(reinterpret_cast<short *>(endianTest));
    if (x == 256) {
        return true;
    }
    else if (x == 1) {
        return false;
    }
    else {
        std::cout << "Error: invalid endian: x = " << x << ", must be 256 or 1";
        exit(1);
    }
}

extern const bool bigEndian = isBigEndian();

void swapEndian(uint16_t &data) {
    uint8_t b1, b2;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    data = (b1 << 8) | b2;
}

void swapEndian(int16_t &data) {
    uint8_t b1, b2;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    data = (b1 << 8) | b2;
}

void swapEndian(uint32_t &data) {
    uint8_t b1, b2, b3, b4;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    b3 = (data >> 16) & 0xFF;
    b4 = (data >> 24) & 0xFF;
    data = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

void swapEndian(int32_t &data) {
    uint8_t b1, b2, b3, b4;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    b3 = (data >> 16) & 0xFF;
    b4 = (data >> 24) & 0xFF;
    data = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

void swapEndian(uint64_t &data) {
    uint64_t b[8];
    b[0] = data & 0xFF;
    b[1] = (data >> 8) & 0xFF;
    b[2] = (data >> 16) & 0xFF;
    b[3] = (data >> 24) & 0xFF;
    b[4] = (data >> 32) & 0xFF;
    b[5] = (data >> 40) & 0xFF;
    b[6] = (data >> 48) & 0xFF;
    b[7] = (data >> 56) & 0xFF; 
    data = (b[0] << 56) | (b[1] << 48) | (b[2] << 40) | (b[3] << 32) | (b[4] << 24) | (b[5] << 16) | (b[6] << 8) | b[7];
}

void swapEndian(int64_t &data) {
    uint64_t b[8];
    b[0] = data & 0xFF;
    b[1] = (data >> 8) & 0xFF;
    b[2] = (data >> 16) & 0xFF;
    b[3] = (data >> 24) & 0xFF;
    b[4] = (data >> 32) & 0xFF;
    b[5] = (data >> 40) & 0xFF;
    b[6] = (data >> 48) & 0xFF;
    b[7] = (data >> 56) & 0xFF; 
    data = (b[0] << 56) | (b[1] << 48) | (b[2] << 40) | (b[3] << 32) | (b[4] << 24) | (b[5] << 16) | (b[6] << 8) | b[7];
}

void swapEndian(float &data) {
    union {
        float f;
        unsigned char b[4];
    } dat1, dat2;
    dat1.f = data;
    dat2.b[0] = dat1.b[3];
    dat2.b[1] = dat1.b[2];
    dat2.b[2] = dat1.b[1];
    dat2.b[3] = dat1.b[0];
    data = dat2.f;
}

void swapEndian(double &data) {
    union {
        double d;
        unsigned char b[8];
    } dat1, dat2;
    dat1.d = data;
    dat2.b[0] = dat1.b[7];
    dat2.b[1] = dat1.b[6];
    dat2.b[2] = dat1.b[5];
    dat2.b[3] = dat1.b[4];
    dat2.b[4] = dat1.b[3];
    dat2.b[5] = dat1.b[2];
    dat2.b[6] = dat1.b[1];
    dat2.b[7] = dat1.b[0];
    data = dat2.d;
}


}
}
