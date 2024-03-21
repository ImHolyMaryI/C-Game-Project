#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

void renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h);
SDL_Texture* createTextTexture(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color);

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        SDL_Log("Unable to initialize SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Road To Heaven", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }

    // Load the background image
    SDL_Surface *backgroundSurface = IMG_Load("../assets/images/IntroScreen.png");
    if (!backgroundSurface) {
        SDL_Log("Failed to load background image: %s", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Load font
    TTF_Font *font = TTF_OpenFont("../assets/fonts/OpenSans/OpenSans-Bold.ttf", 24); // Change font path and size as needed
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }

    // Create text textures for buttons
    SDL_Color textColor = {255, 255, 255, 255}; // White color
    SDL_Texture *exitButtonTexture = createTextTexture(renderer, "Exit", font, textColor);
    SDL_Texture *newGameButtonTexture = createTextTexture(renderer, "New Game", font, textColor);
    if (!exitButtonTexture) {
        SDL_Log("Failed to create text texture for exit button");
        TTF_CloseFont(font);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }

    // Main event loop
    SDL_Event event;
    bool isRunning = true;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        if (mouseX >= 430 && mouseX <= 530 && mouseY >= 400 && mouseY <= 450) {
                            isRunning = false; // Exit the application
                        }
                    }
                    break;
            }
        }

        // Clear renderer and render background
        SDL_RenderClear(renderer);
        renderTexture(renderer, backgroundTexture, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Render the full window size

        // Render buttons
        renderTexture(renderer, newGameButtonTexture, 405, 235, 150, 65); // Position and size of the button
        renderTexture(renderer, exitButtonTexture, 443, 400, 75, 50); // Position and size of the button

        // Present renderer
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(exitButtonTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h) {
    SDL_Rect dstRect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}

SDL_Texture* createTextTexture(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) {
        SDL_Log("Failed to render text surface: %s", TTF_GetError());
        return NULL;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return textTexture;
}
