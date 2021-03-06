#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "bitmap_image.hpp"

int percent;

// project for experimental psychology

void orange_filter(bitmap_image &image) {
    const unsigned int height = image.height();
    const unsigned int width  = image.width();
    printf("image dimensions = %d %d\n", (int)height, (int)width);

    bitmap_image filtered(width, height);

    for(std::size_t y = 0; y < height; ++y)
    {
        for(std::size_t x = 0; x < width; ++x)
        {
            rgb_t colour;

            image.get_pixel(x, y, colour);

            if(colour.red >= 5 && colour.red <= 55 &&
               colour.green >= 65 + percent / 10 - 10 && colour.green <= 240 &&
               colour.blue >= 5 && colour.blue <= 55)   {
            filtered.set_pixel(x, y, make_colour(255, 255, 255));
            }
        }
    }

    image = filtered;
    //image.save_image("orange_filter.bmp");
}

void dfs(int x, int y, int width, int height, std::vector<std::vector<bool> >& visited,
         std::vector<std::pair<int, int> >& points) {

    visited[x][y] = true;
    int r = 5 + percent / 5.5; // zasieg
    for(int i = -r; i <= r; i++) {
        for(int j = -r; j <= r; j++) {
            if(i*i + j*j > r*r)
                continue;
            //if(abs(i) + abs(j) != 1)
            //    continue;
            if(x + i >= width || x + i < 0 || y + j >= height || y + j < 0)
                continue;
            if(visited[x + i][y + j])
                continue;
            dfs(x + i, y + j, width, height, visited, points);
        }
    }
    points.push_back(std::make_pair(x, y));
}

void point_filter(bitmap_image &image) {
    const unsigned int height = image.height();
    const unsigned int width  = image.width();

    //bitmap_image filtered(image); // tworzy canvas z plamami
    bitmap_image filtered(width, height); // tworzy pusty canvas

    std::vector<std::vector<bool> > visited (width , std::vector<bool> (height, 0));
    for(size_t y = 0; y < height; ++y) {
        for(size_t x = 0; x < width; ++x) {
            rgb_t colour;
            image.get_pixel(x, y, colour);
            visited[x][y] = (colour == make_colour(0, 0, 0));
        }
    }

    std::vector<std::vector<std::pair<int, int>>> balls;

    // REMEMBER ABOUT $ulimit -s unlimited (limited stack size will result in crash)
    for(size_t y = 0; y < height; ++y) {
        for(size_t x = 0; x < width; ++x) {
            if(!visited[x][y]) {
                std::vector<std::pair<int, int> > points;
                dfs(x, y, width, height, visited, points);
                std::cout << "size of compund = " << points.size() << "\n";
                if(points.size() > 2 + percent / 10) { // the size of pingpong ball in pixels
                    balls.push_back(points);
                }
            }
        }
    }
    std::cout << "found " << balls.size() << " ping pong balls\n";

    // draw balls where the pingpongs are
    for(auto points : balls) {
        long long mean_x = 0, mean_y = 0;
        for(auto p : points) {
            mean_x += p.first;
            mean_y += p.second;
        }
        mean_x /= (long long)points.size();
        mean_y /= (long long)points.size();
        // drawing full circle with radius r and in gravity center of all points
        int r = 6;
        for(int x = std::max(0LL, mean_x - r); x < std::min((long long)width, mean_x + r); x++) {
            for(int y = std::max(0LL, mean_y - r); y < std::min((long long)height, mean_y + r); y++) {
                if((x - mean_x) * (x - mean_x) + (y - mean_y) * (y - mean_y) < r * r)
                filtered.set_pixel(x, y, make_colour(255, 255, 255));
            }
        }
    }

    image = filtered;
    //image.save_image("ball_filter.bmp");
}

int main(int argc, char* argv[]) { // filename, frame, total number of frames
    if(argc < 4) {
        std::cout << "please enter file name\n";
        return -1;
    }

    std::string file(argv[1]);
    percent = 100 * atoi(argv[2]) / atoi(argv[3]);

    bitmap_image image(file);

    if (!image) {
       std::cout << "Error - Failed to open " << file.c_str() << "\n";
       return -1;
    }

    orange_filter(image);
    point_filter(image);

    for(int i = 0; i < (int)std::string(".bmp").size(); i++)
        file.pop_back();
    file.append("_converted.bmp");



    image.save_image(file);



    std::cout << "successfully converted and saved as " << file << "\n";
    printf("[%d percent]\n", percent);
    return 0;
}
