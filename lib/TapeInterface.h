#ifndef TATLIN_TAPEINTERFACE_H
#define TATLIN_TAPEINTERFACE_H

#include <cstdint>

class TapeInterface {
    virtual void Rewrite(int32_t num) = 0;
    virtual int32_t Read() = 0;
    virtual void Forward() = 0;
    virtual void Back() = 0;
};

#endif //TATLIN_TAPEINTERFACE_H
