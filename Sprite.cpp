#include "Sprite.h"
#include "../inc/ST7735.h"

Sprite::Sprite(int x, int y, int w, int h, int hitW, int hitH, const uint16_t *idleImage){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->health = 10;
    this->hitW = hitW;
    this->hitH = hitH;
    this->currBitmap = idleImage;
    this->idleBitmap = idleImage;
    this->blocking = false;
    this->hitCounter = 0;
    this->totalHits = 0;
    this->isLeft = false;
    this->isRight = false;
}



void Sprite::Move(int vx, int vy, bool jumpSwitchOn, const uint16_t* leftImage,const uint16_t* rightImage, Sprite& other){
    this->vx = vx;
    this->vy = vy;
    this->x += vx;
    if(this->x > (128-(this->w))){
        this->x = (128-(this->w)); // Screen width is 128, sprite width is 18, 128-18 = 118
    } else if(this->x < 0){
        this->x = 0;
    } 
         if(jumpSwitchOn){
            
            this->y += vy;      
        } 
        
    if(Collison(other)){
        /*
                other.x += vx;
        if(other.x > (128-(other.w))){
        other.x = (128-(other.w)); // Screen width is 128, sprite width is 18, 128-18 = 118
        this->x = other.x-(this->w);
    } else if(other.x < 0){
        other.x = 0;
        this->x = (other.x + other.hitW);
    } 
        */
        if(!(this->y < (other.y- (this->h)/2 - 3) && this->y < (other.y- (this->h)/2 + 3))){
            this->x += -1*vx;
        } else {
            this->y += -1*vy;
        }

    
    }else{
        if(!(this->blocking) && (currBitmap == leftImage || currBitmap == rightImage)){
        if(vx < 0){
            currBitmap = leftImage;
            this->isLeft = true;
            this->isRight = false;
        } 
         if(vx > 0){
           currBitmap = rightImage;
           this->isRight = true;
           this->isLeft = false;
        }
        } 

    

    }

    ST7735_DrawBitmap(this->x, this->y, this->currBitmap, this->w,this->h);
    
}

void Sprite::Place(int w, int h){
    ST7735_DrawBitmap(this->x, this->y, this->currBitmap, w,h);


}

bool Sprite::Collison(Sprite& other){
    if((this->x + this->w + this->hitW) >= other.x && (this->x +this->w+ this->hitW) <= (other.x + other.w) && this->y == other.y){
        return true;
    }

    if((this->x - this->hitW) <= other.x+other.w && (this->x - this->hitW) >= (other.x) && this->y == other.y){
        return true;
    }


    return false;
}

void Sprite::Hit(Sprite& other, const uint16_t* hitImageLeft, const uint16_t* hitImageRight, const uint16_t* otherSpriteHitImageLeft, const uint16_t* otherSpriteHitImageRight){
    this->hitW = 2;
    //currBitmap = punchingBitmap
    //Draw
    const uint16_t* activeImage = hitImageRight;
    this->isLeft = false;
    this->isRight = true;
    if(this->vx < 0){
        activeImage = hitImageLeft;
        this->isLeft = true;
        this->isRight = false;
    }
    const uint16_t* activeOtherImage = otherSpriteHitImageRight;
    other.isLeft = false;
    other.isRight = true;
    if(other.vx < 0){
        activeOtherImage = otherSpriteHitImageLeft;
        other.isLeft = true;
        other.isRight = false;
    }
    this->currBitmap = activeImage;
    ST7735_DrawBitmap(this->x, this->y, this->currBitmap, this->w,this->h);
    if(Collison(other) && !(other.blocking) && ((other.x > this->x) && this->isRight || (other.x < this->x) && this->isLeft)){
        other.health -= 1;
        this->hitCounter++;
        this->totalHits++;
        other.currBitmap = activeOtherImage;
        ST7735_DrawBitmap(other.x, other.y, other.currBitmap, other.w,other.h);
    }
    //currBitmap = idle bitmap
   
    //Draw
    this->hitW = 0;
}
void Sprite::Block(const uint16_t* blockImageLeft,const uint16_t* blockImageRight, bool blocking){
    const uint16_t* activeImage = blockImageRight;
    this->isLeft = false;
    this->isRight = true;
    if(this->vx < 0){
        activeImage = blockImageLeft;
        this->isLeft = true;
        this->isRight = false;
    }
    if(blocking){
        this->blocking = true;
        this->currBitmap = activeImage;
        ST7735_DrawBitmap(this->x, this->y, activeImage, this->w,this->h);
    } else if (currBitmap == activeImage) {
        this->blocking = false;
        this->currBitmap = this->idleBitmap;
        //ST7735_DrawBitmap(this->x, this->y, this->idleBitmap, this->w,this->h);

    } else {
        this->blocking = false;
    }
    

}

void Sprite::SpecialMove(const uint16_t* SpecialMove){
    this->currBitmap = SpecialMove;
    ST7735_DrawBitmap(this->x, this->y, SpecialMove, this->w,this->h);
    this->hitCounter = -1;
}

void Sprite::OrbMove(Sprite &other, const uint16_t* otherSpriteHitImageLeft,const uint16_t* blackBMP, int vx){
    if(Collison(other)){
        other.health -= 5;
        other.currBitmap = otherSpriteHitImageLeft;
        ST7735_DrawBitmap(other.x, other.y, other.currBitmap, other.w,other.h);
        ST7735_DrawBitmap(this->x, this->y, blackBMP, this->w,this->h);
        this->x = -1;
    } else {
        ST7735_DrawBitmap(this->x, this->y, this->idleBitmap, this->w,this->h);
        this->x += vx;
    }
}