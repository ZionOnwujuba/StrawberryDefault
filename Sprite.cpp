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
}



void Sprite::Move(int vx, int vy, bool jumpSwitchOn, const uint16_t* jumpImage, Sprite& other){
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
        if(!(this->blocking)){
        if(vx > 0){
            currBitmap = jumpImage;
        } else {
            currBitmap = this->idleBitmap;
        }
        }

    

    }

    ST7735_DrawBitmap(this->x, this->y, this->currBitmap, this->w,this->h);
    
}

void Sprite::Place(int w, int h){
    ST7735_DrawBitmap(this->x, this->y, this->currBitmap, w,h);

}

bool Sprite::Collison(Sprite& other){
    if((this->x + this->w + this->hitW) >= other.x && (this->x +this->w+ this->hitW) <= (other.x + other.w)){
        return true;
    }

    if((this->x - this->hitW) <= other.x+other.w && (this->x - this->hitW) >= (other.x)){
        return true;
    }


    return false;
}

void Sprite::Hit(Sprite& other, const uint16_t* hitImage){
    this->hitW = 2;
    //currBitmap = punchingBitmap
    //Draw
    ST7735_DrawBitmap(this->x, this->y, hitImage, this->w,this->h);
    if(Collison(other) && !(other.blocking)){
        other.health -= 1;
    }
    //currBitmap = idle bitmap
    ST7735_DrawBitmap(this->x, this->y, this->idleBitmap, this->w,this->h);
    //Draw
    this->hitW = 0;
}
void Sprite::Block(const uint16_t* blockImage, bool blocking){
    if(blocking){
        this->blocking = true;
        ST7735_DrawBitmap(this->x, this->y, blockImage, this->w,this->h);
    } else {
        this->blocking = false;
        //ST7735_DrawBitmap(this->x, this->y, this->idleBitmap, this->w,this->h);

    }
    

}