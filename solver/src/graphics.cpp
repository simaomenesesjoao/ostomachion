#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <vector>
#include <iostream>

class Graphics {
    
public:
    // Constructor to initialize SDL2
    Graphics() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL2 could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            exit(1);
        }
        // Create a window
        width = 700;
        height = 700;
        offset = 50;
        window = SDL_CreateWindow("Polygon Example",
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  width, height, SDL_WINDOW_SHOWN);

        if (!window) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            exit(1);
        }

        // Create a renderer for the window
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(1);
        }
    }

    // Destructor to clean up SDL2 resources
    ~Graphics() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // Function to get the color based on x in range [0, 1]
    SDL_Color getColorFromX(float x) {
        SDL_Color color;

        // Ensure x is clamped between 0 and 1
        x = std::min(1.0f, std::max(0.0f, x));

        if (x <= 0.5f) {
            // Interpolate from red to green (x goes from 0 to 0.5)
            color.r = static_cast<Uint8>(255 * (1 - 2 * x));  // Red decreases as x increases
            color.g = static_cast<Uint8>(255 * (2 * x));      // Green increases as x increases
            color.b = 0;  // Blue is 0
        } else {
            // Interpolate from green to blue (x goes from 0.5 to 1)
            color.r = 0;                                      // Red is 0
            color.g = static_cast<Uint8>(255 * (2 - 2 * x));  // Green decreases as x increases
            color.b = static_cast<Uint8>(255 * (2 * x - 1));  // Blue increases as x increases
        }

        color.a = 255;  // Full opacity
        return color;
    }


    // Method to draw a polygon given a vector of points
    void drawPolygonFrame(const std::vector<std::vector<double>>& points_double) {


        // Set the polygon color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for(int i=-2; i<3; i++){
            std::vector<SDL_Point> points;
            for(auto& point: points_double){
                int x = static_cast<int>(point.at(0)/12.0 * (width-2*offset) + offset)+i;
                int y = height - static_cast<int>(point.at(1)/12.0 * (height-2*offset) + offset);
                //std::cout << "converting points " << x << " " << y << "\n";
                
                points.push_back({x, y});
            }
            if(points.size() < 3) 
                return;

            // Draw the polygon
            SDL_RenderDrawLines(renderer, points.data(), points.size());
            SDL_RenderDrawLine(renderer, points.back().x, points.back().y,
                                points.front().x, points.front().y);  // Close the polygon
        }


    }

    // Method to draw a polygon given a vector of points
    void drawFilledPolygon(const std::vector<std::vector<double>>& points_double, const SDL_Color& color) {


        std::vector<SDL_Point> points;
        for(auto& point: points_double){
            int x = static_cast<int>(point.at(0)/12.0 * (width-2*offset) + offset);
            int y = height - static_cast<int>(point.at(1)/12.0 * (height-2*offset) + offset);
            //std::cout << "converting points " << x << " " << y << "\n";
            
            points.push_back({x, y});
        }

        if(points.size() < 3) 
            return;

        // Convert SDL_Point to arrays for SDL2_gfx
        Sint16* vx = new Sint16[points.size()];
        Sint16* vy = new Sint16[points.size()];

        for (size_t i = 0; i < points.size(); ++i) {
            vx[i] = points[i].x;
            vy[i] = points[i].y;
        }
        // Set polygon color (green)

        filledPolygonRGBA(renderer, vx, vy, points.size(), color.r, color.g, color.b, 255);  // Green filled polygon



        delete[] vx;
        delete[] vy;
    }


    void draw_state(const std::vector<std::vector<std::vector<double>>>& poly_list, 
                    const std::vector<std::vector<double>>& frame){

        // Set render draw color to black for the background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);


        for(unsigned i=0; i<poly_list.size(); i++){
            auto poly = poly_list.at(i);
            SDL_Color color = getColorFromX(i*1.0/(poly_list.size()-1));
            drawFilledPolygon(poly, color);
        }

        drawPolygonFrame(frame);


        SDL_RenderPresent(renderer); // Present the rendered content

        // Event loop to handle closing the window
        bool running = true;
        SDL_Event e;
        while (running) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
            }
        }
    }

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    unsigned width, height, offset;
};

// int main() {
//     // Define a polygon (a simple triangle in this case)
//     std::vector<std::vector<double>> polygon1 = {{1, 1},{3, 1},{2, 3}};
//     std::vector<std::vector<double>> polygon2 = {{4, 4},{3, 1},{2, 3}};
//     std::vector<std::vector<double>> polygon3 = {{4, 4},{3, 7},{2, 3}};
//     std::vector<std::vector<double>> polygon4 = {{4, 4},{3, 7},{9, 3}};

//     // Create the Graphics object and draw the polygon
//     Graphics graphics;
//     // graphics.drawPolygon(polygon);

    
//     graphics.draw_state({polygon1, polygon2, polygon3}, polygon4);

//     return 0;
// }
