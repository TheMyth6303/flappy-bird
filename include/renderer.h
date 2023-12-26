#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <flappy.h>
#include <iostream>
#include <pipes.h>

class Renderer {
    private:
        SDL_Renderer *renderer;        // SDL_Renderer for rendering
        int windowWidth, windowHeight; // height and width of window on which renderer is created
        int nbglayers;                 // number of layers to be rendered
        SDL_Texture **bgTextures;      // pointer to *texture type array
        SDL_Rect ***bgRects;           // pointer to 2d array of bg rects for rendering
        Flappy *flappy;                // flappy object
        SDL_Texture *flappyTexture;    // flappy texture
        Pipes *pipes;                  // pipes object
        SDL_Texture *pipeupTexture;    // pipe up texture
        SDL_Texture *pipedownTexture;  // pipe down texture
        TTF_Font *font;

    public:
        Renderer(SDL_Window *window, int windowWidth, int windowHeight, std::string *bgpaths, int nbglayers, Flappy *flappy, std::string flappypath, Pipes *pipes, std::string *pipepaths,
                 TTF_Font *font);
        ~Renderer();
        void updateBackground();                    // update the positions of the background images
        void renderClear();                         // clear renderer
        void renderBackground();                    // render the background to the window
        void renderFlappy();                        // render flappy to the window
        void renderPipes();                         // render pipes
        void renderScore(int score, int highScore); // render score
        void renderMessage(std::string message);    // render message
        void renderPresent();                       // present renderer
};

#endif