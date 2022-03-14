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

        // Cast ray to center of screen
        Point2F rayOrigin(mouse);
        Vector2F rayVector(rayOrigin, {originX, originY});
        for (int i = 0; i < 8; i++) {
            circleRGBA(renderer, mouse.x, mouse.y, 4, 255, 0, 0, 255);
            const std::pair<Point2F, Point2F> *collisionSegment;
            float scaleFactor = 1;
            bool collision = false;
            for (auto &s : segments) {
                float res = math::getIntersection(rayOrigin, rayVector, s.first, s.second);
                if (res > 0 && res < scaleFactor) {
                    collision = true;
                    scaleFactor = res;
                    collisionSegment = &s;
                }
            }
            if (collision) {
                Point2F intersection(rayOrigin.x + (rayVector.x * scaleFactor),
                                    rayOrigin.y + (rayVector.y * scaleFactor));
                circleRGBA(renderer, intersection.x, intersection.y, 4, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, rayOrigin.x, rayOrigin.y, intersection.x, intersection.y);

                rayVector = math::getReflection(rayVector, collisionSegment->first, collisionSegment->second);
                rayOrigin = intersection;
            }
            else {
                Point2F intersection(rayOrigin.x + (rayVector.x * scaleFactor),
                                    rayOrigin.y + (rayVector.y * scaleFactor));
                circleRGBA(renderer, intersection.x, intersection.y, 4, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, rayOrigin.x, rayOrigin.y, intersection.x, intersection.y);
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}