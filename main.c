#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef FPS
#define FPS 60
#endif

const int TIME_FOR_EACH_FRAME = 1000 / FPS;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int running = 1;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* bTexture = NULL;
SDL_Rect bSourceRect;
SDL_Rect bDestinationRect;

SDL_Texture* animationTexture = NULL;
SDL_Rect animationSourceRect;
SDL_FRect animationsDestinationRect;

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
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if(!window) // error check
    {
        printf("Could not create window: %s\n", SDL_GetError());
        running = 0;
        return 1;
    }

    // create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) // error check
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        running = 0;
        return 1;        
    }

    // load the ballou .bmp image and create the texture
    SDL_Surface* tempSurface = SDL_LoadBMP("assets/cat.bmp");
    bTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // get dimensions of texture and store them to source rect
    // 2nd and 3d null because it returns information we dont need at this point
    SDL_QueryTexture(bTexture, NULL,NULL, &bSourceRect.w, &bSourceRect.h);
    bDestinationRect.x = 0;
    bDestinationRect.y = 0;
    bDestinationRect.h = bSourceRect.h;
    bDestinationRect.w = bSourceRect.w;


    // load the animation image and create the texture
    tempSurface = IMG_Load("assets/animation-alpha.png");
    animationTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    animationSourceRect.w = 128;
    animationSourceRect.h = 82;
    animationsDestinationRect.w = 128;
    animationsDestinationRect.h = 82;

    return 0;
}

void input()
{
    // listen for input
    SDL_Event currentEvent;

    while(SDL_PollEvent(&currentEvent))
    {
        SDL_EventType eventType = currentEvent.type;
        switch (eventType)
        {
        case SDL_QUIT:
            running = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(currentEvent.button.button == SDL_BUTTON_LEFT)
            {
                //printf("left mouse button clicked\n");
            }
            break;
        case SDL_MOUSEMOTION:
            int mouseX = currentEvent.motion.x;
            int mouseY = currentEvent.motion.y;
            //printf("Mouse moved to: %d, %d\n", mouseX, mouseY);
            break;
        default:
            break;
        }
    }

    const Uint8* keyboardState = SDL_GetKeyboardState(0);
    if(keyboardState[SDL_SCANCODE_SPACE] == 1)
    {
        printf("Space is pressed\n");
    }
}

int once = 0;
void update(float deltaTime)
{
    // move the destination rectnagle (move the cat)
    animationsDestinationRect.x += 200.0f * deltaTime;

    if(animationsDestinationRect.x > SCREEN_WIDTH && once == 0)
    {
        double timeInSeconds = SDL_GetTicks() / 1000.f;
        printf("Finished at %lf seconds\n", timeInSeconds);
        once = 1; 
    }

    // update part
    animationSourceRect.x = 128 * ((SDL_GetTicks() / 75) % 6);
}

void render()
{
    // render part
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderClear(renderer);

    //SDL_RenderCopy(renderer, bTexture, &bSourceRect, &bDestinationRect);

    SDL_RenderCopyF(renderer, animationTexture, &animationSourceRect, &animationsDestinationRect);


    SDL_RenderPresent(renderer);
}

void cleanUp()
{
    SDL_DestroyTexture(bTexture);
    SDL_DestroyTexture(animationTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    if (init() != 0) return 1;

    Uint32 deltaLastTime = SDL_GetTicks(); // set a starting value for the first loop

    while(running)
    {
        Uint32 frameStart = SDL_GetTicks();
        input();

        // calculate delta time
        Uint32 deltaCurrentTime = SDL_GetTicks();
        float deltaTime = (deltaCurrentTime - deltaLastTime) / 1000.0f;
        deltaLastTime = deltaCurrentTime;

        update(deltaTime);
        render();
        Uint32 frameEnd = SDL_GetTicks();
        Uint32 timeToRenderFrame = frameEnd - frameStart;
        if(timeToRenderFrame < TIME_FOR_EACH_FRAME)
        {
            SDL_Delay(TIME_FOR_EACH_FRAME - timeToRenderFrame);
        }
    }

    cleanUp();
}