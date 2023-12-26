#ifndef FLAPPY_H
#define FLAPPY_H

const int BOUNCE_VEL = -11;
const int GRAVITY = 1;

class Flappy {
    private:
        int g = GRAVITY; // acceleration due to gravity

    public:
        int x, y;            // coordinates of the center of the flappy sprite
        int vy = BOUNCE_VEL; // upward velocity of the flappy sprite
        int s;               // side length of the flappy sprite
        Flappy(int x, int y, int size);
        void update();
        void bounce();
};

#endif