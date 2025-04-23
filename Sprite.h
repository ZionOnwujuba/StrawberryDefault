#ifndef SPRITE_H
#define SPRITE_H
#include <stdint.h>

class Sprite {
    public:
    int x;
    int y;
    int vx;
    int vy;
    bool blocking;
    int w;
    int h;
    int hitW;
    int hitH;
    int health;
    const uint16_t* currBitmap;
    const uint16_t* idleBitmap;
    
    Sprite(int x, int y, int w, int h, int hitW, int hitH, const uint16_t* currImage);
    void Place(int w, int h);
    void Move(int vx, int vy, bool jumpSwitchOn, const uint16_t* jumpImage, Sprite& other);
    void Hit(Sprite& other, const uint16_t* hitImage); // TODO
    bool Collison(Sprite& other);
    void Block(const uint16_t* blockImage, bool blocking);
};

#endif

