#ifndef PIPES_H
#define PIPES_H

#include <ctime>
#include <deque>
#include <random>

const int PIPE_VELOCITY = 5;

class Pipe {
    public:
        int x, y; // coordinates of the center point betweek 2 pipes
        Pipe(int x, int y);
};

class Pipes {
    public:
        int npipes, pipeWidth, pipeHeight, pipeGap, holeGap;
        int windowWidth, windowHeight;
        std::deque<Pipe> queue;
        Pipes(int windowWidth, int windowHeight, int npipes, int pipeGap, int holeGap, int pipeWidth, int pipeHeight);
        void update(); // moves pipes, also generates a new pipe and removes the frontmost pipe if needed
        void reset();  // resets pipes for new game
};

#endif