#include <renderer.h>

Renderer::Renderer(SDL_Window *window, int windowWidth, int windowHeight, std::string *bgpaths, int nbglayers, Flappy *flappy, std::string flappypath, Pipes *pipes, std::string *pipepaths,
                   TTF_Font *font) {
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->font = font;

    // for background
    this->nbglayers = nbglayers;
    this->bgTextures = new SDL_Texture *[nbglayers];
    for (int i = 0; i < nbglayers; i++) {
        SDL_Surface *surface = SDL_LoadBMP(bgpaths[i].c_str());
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
        this->bgTextures[i] = SDL_CreateTextureFromSurface(this->renderer, surface);
        SDL_FreeSurface(surface);
    }
    this->bgRects = new SDL_Rect **[nbglayers];
    for (int i = 0; i < nbglayers; i++) {
        this->bgRects[i] = new SDL_Rect *[2];
    }
    for (int i = 0; i < nbglayers; i++) {
        this->bgRects[i][0] = new SDL_Rect;
        this->bgRects[i][0]->x = 0;
        this->bgRects[i][0]->y = 0;
        this->bgRects[i][0]->w = windowWidth;
        this->bgRects[i][0]->h = windowHeight;
        this->bgRects[i][1] = new SDL_Rect;
        this->bgRects[i][1]->x = windowWidth;
        this->bgRects[i][1]->y = 0;
        this->bgRects[i][1]->w = windowWidth;
        this->bgRects[i][1]->h = windowHeight;
    }

    // for flappy
    this->flappy = flappy;
    SDL_Surface *surface = SDL_LoadBMP(flappypath.c_str());
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
    this->flappyTexture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_FreeSurface(surface);

    // for pipes
    this->pipes = pipes;
    SDL_Surface *surfacePipeup = SDL_LoadBMP(pipepaths[0].c_str());
    SDL_SetColorKey(surfacePipeup, SDL_TRUE, SDL_MapRGB(surfacePipeup->format, 255, 0, 255));
    this->pipeupTexture = SDL_CreateTextureFromSurface(this->renderer, surfacePipeup);
    SDL_FreeSurface(surfacePipeup);
    SDL_Surface *surfacePipedown = SDL_LoadBMP(pipepaths[1].c_str());
    SDL_SetColorKey(surfacePipedown, SDL_TRUE, SDL_MapRGB(surfacePipedown->format, 255, 0, 255));
    this->pipedownTexture = SDL_CreateTextureFromSurface(this->renderer, surfacePipedown);
    SDL_FreeSurface(surfacePipedown);
};

Renderer::~Renderer() {
    SDL_DestroyRenderer(this->renderer);

    // for background
    for (int i = 0; i < this->nbglayers; i++) {
        SDL_DestroyTexture(this->bgTextures[i]);
    }
    delete this->bgTextures;
    for (int i = 0; i < this->nbglayers; i++) {
        delete this->bgRects[i][0];
        delete this->bgRects[i][1];
        delete this->bgRects[i];
    }
    delete this->bgRects;

    // for flappy
    SDL_DestroyTexture(flappyTexture);

    // for pipes
    SDL_DestroyTexture(pipeupTexture);
    SDL_DestroyTexture(pipedownTexture);
}

void Renderer::updateBackground() {
    for (int i = 0; i < nbglayers; i++) {
        this->bgRects[i][0]->x = (this->bgRects[i][0]->x + 2 * this->windowWidth - i - 1) % (2 * this->windowWidth) - this->windowWidth;
        this->bgRects[i][1]->x = (this->bgRects[i][1]->x + 2 * this->windowWidth - i - 1) % (2 * this->windowWidth) - this->windowWidth;
    }
}

void Renderer::renderClear() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
}

void Renderer::renderBackground() {
    for (int i = 0; i < nbglayers; i++) {
        SDL_RenderCopy(renderer, this->bgTextures[i], NULL, this->bgRects[i][0]);
        SDL_RenderCopy(renderer, this->bgTextures[i], NULL, this->bgRects[i][1]);
    }
}

void Renderer::renderFlappy() {
    SDL_Rect rect;
    rect.x = this->flappy->x - this->flappy->s / 2;
    rect.y = this->flappy->y - this->flappy->s / 2;
    rect.h = this->flappy->s;
    rect.w = this->flappy->s;
    SDL_RenderCopy(this->renderer, this->flappyTexture, NULL, &rect);
}

void Renderer::renderPipes() {
    for (int i = 0; i < this->pipes->npipes; i++) {
        Pipe pipe = this->pipes->queue[i];
        SDL_Rect rectUp;
        rectUp.x = pipe.x - this->pipes->pipeWidth / 2;
        rectUp.y = pipe.y - this->pipes->pipeHeight - this->pipes->holeGap / 2;
        rectUp.w = this->pipes->pipeWidth;
        rectUp.h = this->pipes->pipeHeight;
        SDL_RenderCopy(this->renderer, this->pipeupTexture, NULL, &rectUp);

        SDL_Rect rectDown;
        rectDown.x = pipe.x - this->pipes->pipeWidth / 2;
        rectDown.y = pipe.y + this->pipes->holeGap / 2;
        rectDown.w = this->pipes->pipeWidth;
        rectDown.h = this->pipes->pipeHeight;
        SDL_RenderCopy(this->renderer, this->pipedownTexture, NULL, &rectDown);
    }
}

void Renderer::renderScore(int score, int highScore) {
    // score
    SDL_Rect scoreRect = {this->windowWidth - 160, 20, 140, 25};
    std::string scoreStr = std::to_string(score);
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(this->font, ("Score:" + std::string(3 - scoreStr.length(), ' ') + scoreStr).c_str(), {0, 0, 0});
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_RenderCopy(this->renderer, scoreTexture, NULL, &scoreRect);
    SDL_DestroyTexture(scoreTexture);
    // highScore
    SDL_Rect hscoreRect = {this->windowWidth - 160, 50, 140, 25};
    std::string hscoreStr = std::to_string(highScore);
    SDL_Surface *hscoreSurface = TTF_RenderText_Solid(this->font, ("Best: " + std::string(3 - hscoreStr.length(), ' ') + hscoreStr).c_str(), {0, 0, 0});
    SDL_Texture *hscoreTexture = SDL_CreateTextureFromSurface(renderer, hscoreSurface);
    SDL_FreeSurface(hscoreSurface);
    SDL_RenderCopy(this->renderer, hscoreTexture, NULL, &hscoreRect);
    SDL_DestroyTexture(hscoreTexture);
}

void Renderer::renderMessage(std::string message) {
    SDL_Rect rect = {3 * this->windowWidth / 16, 5 * this->windowHeight / 6 - 60, 5 * this->windowWidth / 8, this->windowHeight / 6};
    SDL_Surface *surface = TTF_RenderText_Solid(this->font, message.c_str(), {0, 0, 0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(this->renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void Renderer::renderPresent() { SDL_RenderPresent(this->renderer); }