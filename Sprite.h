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
    int hitCounter;
    int health;
    int totalHits;
    const uint16_t* currBitmap;
    const uint16_t* idleBitmap;
    bool isLeft;
    bool isRight;
    
    Sprite(int x, int y, int w, int h, int hitW, int hitH, const uint16_t* currImage);
    void Place(int w, int h);
    void Move(int vx, int vy, bool jumpSwitchOn, const uint16_t* leftImage,const uint16_t* rightImage, Sprite& other);
    void Hit(Sprite& other, const uint16_t* hitImageLeft, const uint16_t* hitImageRight, const uint16_t* otherSpriteHitImageLeft, const uint16_t* otherSpriteHitImageRight); // TODO
    bool Collison(Sprite& other);
    void Block(const uint16_t* blockImageLeft,const uint16_t* blockImageRight, bool blocking);
    void SpecialMove(const uint16_t* SpecialMove);
    void OrbMove(Sprite &other, const uint16_t* otherSpriteHitImageLeft,const uint16_t* blackBMP, int vx);
};

#endif

