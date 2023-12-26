#include <flappy.h>

Flappy::Flappy(int x, int y, int size) {
    this->x = x;
    this->y = y;
    this->s = size;
}

void Flappy::update() {
    this->y += this->vy;
    this->vy += this->g;
}

void Flappy::bounce() { this->vy = BOUNCE_VEL; }