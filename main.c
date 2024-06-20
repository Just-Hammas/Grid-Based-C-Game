#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define PLAYER_SIZE 50
#define ORB_SIZE 50

SDL_Color textColor = {255, 255, 255}; // WHITE color


// Structure to represent an orb
typedef struct {
    int x;
    int y;
    int active; // Flag to indicate if the orb is active (visible)
} Orb;

// Function to respawn an orb at a random position
void respawnOrb(Orb* orb) {
    orb->x = rand() % (WIDTH - ORB_SIZE); // Random x position within the screen width
    orb->y = 80 + rand() % (HEIGHT - 80 - ORB_SIZE); // Random y position within the screen height
    orb->active = 1; // Make the orb active (visible)
}

int main(int argc, char* args[]) {

    // Random number generator
    srand(time(NULL));

    // S D L ' S  B U L L S H * T //

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Quit();
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Quit();
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL2 Grid Movement", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // L O A D I N G  T H E  G R A P H I C A L  C O M P O N E N T S //

    // Background
    SDL_Surface* backgroundSurface = IMG_Load("background.png");
    if (!backgroundSurface) {
        printf("Unable to load background image! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        printf("Unable to create texture from background image! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Player
    SDL_Surface* loadedSurface = IMG_Load("player.png");
    if (!loadedSurface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", "player.png", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (!playerTexture) {
        printf("Unable to create texture from %s! SDL Error: %s\n", "player.png", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Orb
    loadedSurface = IMG_Load("item.png");
    if (!loadedSurface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", "item.png", IMG_GetError());
        SDL_DestroyTexture(playerTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Texture* orbTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (!orbTexture) {
        printf("Unable to create texture from %s! SDL Error: %s\n", "item.png", SDL_GetError());
        SDL_DestroyTexture(playerTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    // Font
    TTF_Font* font = TTF_OpenFont("ARCADECLASSIC.ttf", 35);
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyTexture(playerTexture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(orbTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }


    // I N I T I A L I Z I N G  P O S I T I O N S //
    // Orb
    Orb orb;
    respawnOrb(&orb);

    // Player
    int playerX = WIDTH/2 - PLAYER_SIZE / 2;
    int playerY = HEIGHT/2 - PLAYER_SIZE / 2;
    int timer = 60000; // Timer
    int score = 0;

    int quit = 0; //Main Loop FLag

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            // User presses a key
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        if (playerY > 75) {playerY -= PLAYER_SIZE;}
                        break;
                    case SDLK_s:
                        if (playerY < HEIGHT - PLAYER_SIZE) {playerY += PLAYER_SIZE;}
                        break;
                    case SDLK_a:
                        if (playerX > 0) {playerX -= PLAYER_SIZE;}
                        break;
                    case SDLK_d:
                        if (playerX < WIDTH - PLAYER_SIZE) {playerX += PLAYER_SIZE;}
                        break;
                    case SDLK_q:
                        quit = 1;
                        break;
                }
            }

        }

        // Check for collision between player and orb
        if (playerX < orb.x + ORB_SIZE && playerX + PLAYER_SIZE > orb.x &&
            playerY < orb.y + ORB_SIZE && playerY + PLAYER_SIZE > orb.y) {
            respawnOrb(&orb);
            score += 100;  // Increase score by 100
            //timer += 1000;
        }

        // Update timer
        timer -= 1; // Decrease by 16 milliseconds (60 frames per second)

        // If timer reaches 0, display final score
        if (timer <= 0) {
            // Render final score
            char finalScoreText[20];
            snprintf(finalScoreText, 40, "Final Score: %d", score);
            SDL_Color textColor = {255, 255, 255};
            SDL_Surface* finalScoreSurface = TTF_RenderText_Solid(font, finalScoreText, textColor);
            SDL_Texture* finalScoreTexture = SDL_CreateTextureFromSurface(renderer, finalScoreSurface);
            SDL_Rect finalScoreRect = {WIDTH / 2 - finalScoreSurface->w / 2, HEIGHT / 2 - finalScoreSurface->h / 2, finalScoreSurface->w, finalScoreSurface->h};
            SDL_RenderCopy(renderer, finalScoreTexture, NULL, &finalScoreRect);
            SDL_FreeSurface(finalScoreSurface);
            SDL_DestroyTexture(finalScoreTexture);
            SDL_RenderPresent(renderer);
            SDL_Delay(3000); // Delay for 3 seconds before quitting
            quit = 1;        // Quit the game
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Render player
        SDL_Rect playerRect = {playerX, playerY, PLAYER_SIZE*2, PLAYER_SIZE*2};
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

        // Render orb if it's active
        if (orb.active) {
            SDL_Rect orbRect = {orb.x, orb.y, ORB_SIZE, ORB_SIZE};
            SDL_RenderCopy(renderer, orbTexture, NULL, &orbRect);
        }

        // Render score
        char scoreText[20];
        snprintf(scoreText, 20, "Score  %d", score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);
        if (!scoreSurface) {
            printf("Failed to render score text! SDL_ttf Error: %s\n", TTF_GetError());
            quit = 1;
            break;
        }
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        if (!scoreTexture) {
            printf("Failed to create score texture! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(scoreSurface);
            quit = 1;
            break;
        }
        SDL_Rect scoreRect = {WIDTH - scoreSurface->w - 10, 10, scoreSurface->w, scoreSurface->h};
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        // Render timer
        char timerText[10];
        snprintf(timerText, 10, "Timer  %.1f", (float)timer / 1000);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, timerText, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Show final score when timer reaches 0
    char finalScoreText[20];
    snprintf(finalScoreText, 20, "Final Score  %d", score);
    SDL_Surface* finalScoreSurface = TTF_RenderText_Solid(font, finalScoreText, textColor);
    if (finalScoreSurface) {
        SDL_Texture* finalScoreTexture = SDL_CreateTextureFromSurface(renderer, finalScoreSurface);
        if (finalScoreTexture) {
            SDL_Rect finalScoreRect = {(WIDTH - finalScoreSurface->w) / 2, (HEIGHT - finalScoreSurface->h) / 2, finalScoreSurface->w, finalScoreSurface->h};
            SDL_RenderCopy(renderer, finalScoreTexture, NULL, &finalScoreRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(3000); // Delay for 3 seconds before quitting
            SDL_DestroyTexture(finalScoreTexture);
        } else {
            printf("Failed to create final score texture! SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(finalScoreSurface);
    } else {
        printf("Failed to render final score text! SDL_ttf Error: %s\n", TTF_GetError());
    }

    // End SDL
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(orbTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    IMG_Quit();

    TTF_Quit();
    TTF_CloseFont(font);

    return 0;
}