#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <math/point.hpp>
#include <math/vector.hpp>
#include <math/algorithm.hpp>

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;
constexpr int originX = windowWidth / 2;
constexpr int originY = windowHeight / 2;

int main(int arc, char **argv)
{
    using namespace arctic;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL2 Template", 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool isRunning = true;
    while (isRunning)
    {
        Point2F mouse;

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_WINDOWEVENT:
                if (e.window.event != SDL_WINDOWEVENT_CLOSE)
                    break;
            case SDL_QUIT:
                std::cout << "SDL_Quit event received" << std::endl;
                isRunning = false;
                break;
            case SDL_MOUSEMOTION:
                mouse.x = e.motion.x;
                mouse.y = e.motion.y;
                break;
            default:
                break;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // create circle on mouse
        circleRGBA(renderer, mouse.x, mouse.y, 4, 255, 0, 0, 255);
        // Target point
        circleRGBA(renderer, originX + 200, originY - 200, 2, 0, 255, 0, 255);

        const std::vector<std::pair<Point2F, Point2F>> segments = {
            // bounding box
            {{10, 10}, {windowWidth - 10, 10}},
            {{windowWidth - 10, 10}, {windowWidth - 10, windowHeight - 10}},
            {{windowWidth - 10, windowHeight - 10}, {10, windowHeight - 10}},
            {{10, windowHeight - 10}, {10, 10}},

            {{500, 400}, {550, 450}},
            {{550, 450}, {450, 450}},
            {{450, 450}, {500, 400}},

            {{300, 200}, {300, 100}},
            {{300, 100}, {200, 100}},
            {{200, 100}, {200, 200}},
            {{200, 200}, {300, 200}},

            {{500, 200}, {500, 100}},
            {{500, 100}, {600, 50}},
            {{600, 50}, {400, 50}},
            {{400, 50}, {400, 200}},
            {{400, 200}, {500, 200}},

            {{400, 500}, {200, 300}}

            // {{400, 200}, {500, 200}}

        };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        for (auto &s : segments)
        {
            SDL_RenderDrawLine(renderer, s.first.x, s.first.y, s.second.x, s.second.y);
        }

        // Cast ray to each vertices
        for (auto &v : segments)
        {
            Vector2F rayVector(mouse, v.first);
            circleRGBA(renderer, v.first.x, v.first.y, 4, 255, 0, 0, 255);
            float scaleFactor = 1;
            for (auto &s : segments)
            {
                float res = math::getIntersection(
                    mouse,
                    rayVector,
                    s.first,
                    s.second);
                if (res > 0 && res < scaleFactor)
                {
                    scaleFactor = res;
                    Point2F intersection(mouse.x + (rayVector.x * res),
                                         mouse.y + (rayVector.y * res));
                    printf("Intersection@ %f,%f\n\tscaleFactor: %f\n",
                           intersection.x, intersection.y, res);
                    circleRGBA(renderer, s.first.x, s.first.y, 4, 0, 255, 0, 255);
                    circleRGBA(renderer, s.first.x, s.first.y, 4, 0, 255, 0, 255);
                }
            }

            // get lowest scalefactor
            Point2F intersection(mouse.x + (rayVector.x * scaleFactor),
                                 mouse.y + (rayVector.y * scaleFactor));
            printf("scaleFactor %f\n",
                   scaleFactor);
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            circleRGBA(renderer, v.first.x, v.first.y, 4, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, mouse.x, mouse.y, intersection.x, intersection.y);
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}