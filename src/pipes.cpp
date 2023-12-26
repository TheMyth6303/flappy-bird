#include <pipes.h>

Pipe::Pipe(int x, int y) {
    this->x = x;
    this->y = y;
}

Pipes::Pipes(int windowWidth, int windowHeight, int npipes, int pipeGap, int holeGap, int pipeWidth, int pipeHeight) {
    this->npipes = npipes;
    this->pipeGap = pipeGap;
    this->pipeWidth = pipeWidth;
    this->pipeHeight = pipeHeight;
    this->holeGap = holeGap;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < npipes; i++) {
        this->queue.push_back(Pipe(windowWidth + pipeWidth / 2 + pipeGap * i, std::rand() % (windowHeight - holeGap - 79) + holeGap / 2 + 40));
    }
}

void Pipes::update() {
    for (int i = 0; i < this->npipes; i++) {
        queue[i].x -= PIPE_VELOCITY;
    }
    if (this->queue[0].x <= -this->pipeWidth / 2) {
        this->queue.pop_front();
        this->queue.push_back(Pipe(queue[npipes - 2].x + pipeGap, std::rand() % (this->windowHeight - this->holeGap - 79) + this->holeGap / 2 + 40));
    }
}

void Pipes::reset() {
    for (int i = 0; i < npipes; i++) {
        this->queue[i].x = this->windowWidth + this->pipeWidth / 2 + this->pipeGap * i;
        this->queue[i].y = std::rand() % (this->windowHeight - this->holeGap - 79) + this->holeGap / 2 + 40;
    }
}