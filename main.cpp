#include <SDL2/SDL.h>
#include <flappy.h>
#include <iostream>
#include <pipes.h>
#include <random>
#include <renderer.h>

const int WIDTH = 640, HEIGHT = 480;

bool gameOver(Flappy *flappy, Pipes *pipes) {
    if (flappy->y >= HEIGHT - flappy->s / 2) {
        flappy->y = HEIGHT - flappy->s / 2;
        return true;
    }
    Pipe pipe = pipes->queue[0];
    if (pipe.x - pipes->pipeWidth / 2 <= flappy->x && pipe.x + pipes->pipeWidth / 2 >= flappy->x && pipe.y - pipes->holeGap / 2 >= flappy->y - flappy->s / 2) {
        return true;
    } else if (pipe.x - pipes->pipeWidth / 2 <= flappy->x && pipe.x + pipes->pipeWidth / 2 >= flappy->x && pipe.y + pipes->holeGap / 2 <= flappy->y + flappy->s / 2) {
        return true;
    } else if (pipe.x - pipes->pipeWidth / 2 <= flappy->x + flappy->s / 2 && pipe.x + pipes->pipeWidth / 2 >= flappy->x - flappy->s / 2 && pipe.y - pipes->holeGap / 2 >= flappy->y) {
        return true;
    } else if (pipe.x - pipes->pipeWidth / 2 <= flappy->x + flappy->s / 2 && pipe.x + pipes->pipeWidth / 2 >= flappy->x - flappy->s / 2 && pipe.y + pipes->holeGap / 2 <= flappy->y) {
        return true;
    } else if ((pipe.x - pipes->pipeWidth / 2 - flappy->x) * (pipe.x - pipes->pipeWidth / 2 - flappy->x) + (pipe.y - pipes->holeGap / 2 - flappy->y) * (pipe.y - pipes->holeGap / 2 - flappy->y) <=
               (flappy->s / 2) * (flappy->s / 2)) {
        return true;
    } else if ((pipe.x - pipes->pipeWidth / 2 - flappy->x) * (pipe.x - pipes->pipeWidth / 2 - flappy->x) + (pipe.y + pipes->holeGap / 2 - flappy->y) * (pipe.y + pipes->holeGap / 2 - flappy->y) <=
               (flappy->s / 2) * (flappy->s / 2)) {
        return true;
    } else if ((pipe.x + pipes->pipeWidth / 2 - flappy->x) * (pipe.x + pipes->pipeWidth / 2 - flappy->x) + (pipe.y - pipes->holeGap / 2 - flappy->y) * (pipe.y - pipes->holeGap / 2 - flappy->y) <=
               (flappy->s / 2) * (flappy->s / 2)) {
        return true;
    } else if ((pipe.x + pipes->pipeWidth / 2 - flappy->x) * (pipe.x + pipes->pipeWidth / 2 - flappy->x) + (pipe.y + pipes->holeGap / 2 - flappy->y) * (pipe.y + pipes->holeGap / 2 - flappy->y) <=
               (flappy->s / 2) * (flappy->s / 2)) {
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    // initialize SDL2 and SDL2_ttf
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_GetError();
    }
    if (TTF_Init() == -1) {
        TTF_GetError();
    }

    // create window
    SDL_Window *window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    // load assets
    std::string bgpaths[3] = {"assets/textures/background.bmp", "assets/textures/buildings1.bmp", "assets/textures/buildings2.bmp"};
    std::string flappypath = "assets/textures/flappy.bmp";
    std::string pipepaths[2] = {"assets/textures/pipeup.bmp", "assets/textures/pipedown.bmp"};
    TTF_Font *font = TTF_OpenFont("assets/font/Instruction Bold.ttf", 40);

    // create bird and obstacles
    Flappy *flappy = new Flappy(WIDTH / 4, HEIGHT / 2, 60);
    Pipes *pipes = new Pipes(WIDTH, HEIGHT, 3, WIDTH / 2, 150, 120, 480);

    // initialize custom renderer
    Renderer *renderer = new Renderer(window, WIDTH, HEIGHT, bgpaths, 3, flappy, flappypath, pipes, pipepaths, font);

    // record scores
    int highScore = 0;
    int score = 0;

    // program loop
    bool windowOpen = true;
    bool running = false;
    bool keydown = false;
    while (windowOpen) {
        SDL_Delay(20);
        // render starting scene
        renderer->renderClear();
        renderer->renderBackground();
        renderer->renderFlappy();
        renderer->renderPipes();
        renderer->updateBackground();
        renderer->renderScore(score, highScore);
        renderer->renderMessage("Press SPACE to play");
        renderer->renderPresent();

        // check if game has been started
        SDL_Event event;
        while (SDL_PollEvent(&event) > 0) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                windowOpen = false;
            }
            if (event.key.keysym.sym == SDLK_SPACE) {
                flappy->bounce();
                running = true;
                keydown = true;
            }
        }

        // game loop
        while (running) {
            // delay to slow down simulation
            SDL_Delay(20);

            // detect and handle events
            SDL_Event event;
            while (SDL_PollEvent(&event) > 0) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    windowOpen = false;
                }
                if (!keydown && event.key.keysym.sym == SDLK_SPACE) {
                    keydown = true;
                    flappy->bounce();
                }
                if (event.type == SDL_KEYUP) {
                    keydown = false;
                }
            }

            // perform updates
            flappy->update();
            pipes->update();
            renderer->updateBackground();

            // update score if pipe is passed
            if (pipes->queue[0].x + pipes->pipeWidth / 2 >= flappy->x - flappy->s / 2 && pipes->queue[0].x + pipes->pipeWidth / 2 - PIPE_VELOCITY < flappy->x - flappy->s / 2) {
                score++;
            }

            // check if game is over
            if (gameOver(flappy, pipes)) {
                highScore = std::max(score, highScore);
                renderer->renderClear();
                renderer->renderBackground();
                renderer->renderFlappy();
                renderer->renderPipes();
                renderer->renderScore(score, highScore);
                renderer->renderPresent();
                SDL_Delay(1000);
                while (SDL_PollEvent(&event) > 0) {
                    // this loop clears events from queue that have been performed in the above delay
                }
                score = 0;
                flappy->y = HEIGHT / 2;
                flappy->vy = 0;
                pipes->reset();
                running = false;
            } else {
                renderer->renderClear();
                renderer->renderBackground();
                renderer->renderFlappy();
                renderer->renderPipes();
                renderer->renderScore(score, highScore);
                renderer->renderPresent();
            }
        }
    }

    // clear memory
    delete renderer;
    delete pipes;
    delete flappy;
    TTF_CloseFont(font);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
