#include <stdio.h>
#include <SDL2/SDL.h>

int running = 1;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* myTexture = NULL;

SDL_Rect sourceRect;
SDL_Rect destinationRect;

int init()
{
    // initialize SDL subsystems
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if(result < 0)
    {
        printf("Error in initialization of SDL subsystems: %s\n", SDL_GetError());
        return 1;
    }
    
    // create the window and check errors
    window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    if(!window) // error check
    {
        printf("Could not create window: %s\n", SDL_GetError());
        running = 0;
        return 1;
    }

    // create the renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) // error check
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        running = 0;
        return 1;        
    }

    return 0;
}

int main()
{
    if (init() != 0) return 1;

    // load the .bmp image and create the texture
    SDL_Surface* tempSurface = SDL_LoadBMP("assets/cat.bmp");
    myTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // get dimensions of texture and store them to source rect
    // 2nd and 3d null because it returns information we dont need at this point
    SDL_QueryTexture(myTexture, NULL,NULL, &sourceRect.w, &sourceRect.h);
    destinationRect.x = 0;
    destinationRect.y = 0;
    destinationRect.h = sourceRect.h;
    destinationRect.w = sourceRect.w;

    while(running)
    {
        // listen for input
        SDL_Event currentEvent;
        if(SDL_PollEvent(&currentEvent))
        {
            SDL_EventType eventType = currentEvent.type;
            switch (eventType)
            {
            case SDL_QUIT:
                running = 0;
                break;
            
            default:
                break;
            }
        }

        // render part
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, myTexture, &sourceRect, &destinationRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}