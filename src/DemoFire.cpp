//
// Created by corwin on 5/1/19.
//

#include "DemoFire.h"

// Initializes the random number generator.
void DemoFire::_initRng() {
    this->_rnd_gen = std::minstd_rand(std::random_device{}());
    this->_rnd_dist = std::uniform_real_distribution<double>(0.0, 1.0);
}

// Generates a random number.
double DemoFire::_rnd() {
    return this->_rnd_dist(this->_rnd_gen);
}

// Gets a random color from our palette.
size_t DemoFire::_rndColor() {
    size_t color_idx = floor(this->_rnd() * PALETTE_SIZE);
    return color_idx;
}

// Initializes our fire.
// At its core our fire generator is basically cellular automata. So we keep those cells in a vector of size
// WIDTH * HEIGHT. While we could use multidimensional arrays or a 2d vector the original algorithm uses some old-school
// pointer math to propagate the flames, so in this implementation we're using a 1d vector of length WIDTH * HEIGHT.
// This method accepts a boolean to determine if we should use a single white line of pixels as our source, or if we
// should use randomly colored pixels as our source. From there it iterates over the vector and pre-fills it with our
// starting colors.
void DemoFire::_initFire(const bool random_seed) {
    this->_fireCells = std::vector<size_t>(this->_fireSize);

    // Fill vector with defaults
    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            if (y == this->_height - 1) // Bottom row is white (max palette index).
                if (random_seed)
                    this->_fireCells[y * this->_width + x] = this->_rndColor();
                else
                    this->_fireCells[y * this->_width + x] = PALETTE_SIZE - 1;

            else // Rest of cells are black (palette index 0).
                this->_fireCells[y * this->_width + x] = 0;
        }
    }
}

// Our constructor. Stores our parameters and bootstraps our rng and cells.
DemoFire::DemoFire(const size_t w, const size_t h, const bool random_seed = false) {
    this->_width = w;
    this->_height = h;
    this->_fireSize = w * h;

    this->_initRng();

    this->_initFire(random_seed);
}

// Parses our vector of cells and produces an image. This variant returns a new image.
sf::Image DemoFire::getImage() {
    sf::Image img;
    img.create(this->_width, this->_height);

    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            size_t palette_idx = this->_fireCells[y * this->_width + x];
            sf::Color pixel_color = fire_palette[palette_idx];
            img.setPixel(x, y, pixel_color);
        }
    }

    return img;
}

// Parses our vector of cells and produces an image. This variant writes to an existing image by reference.
void DemoFire::getImage(sf::Image &img) {
    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            size_t palette_idx = this->_fireCells[y * this->_width + x];
            sf::Color pixel_color = fire_palette[palette_idx];
            img.setPixel(x, y, pixel_color);
        }
    }
}

// This simple iterates over our cells and calls our actual update function.
void DemoFire::doFire() {
    // Starting at horizontal line 1 prevents overwriting the bottom source line of pixels and prevents an integer
    // underflow from occuring later in spreadFire
    for (int y = 1; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            this->spreadFire(y * this->_width + x);
        }
    }
}

// This is where the flames happen! This logic is mostly cribbed directly from the source material.
void DemoFire::spreadFire(size_t src_idx) {
    // src_idx: this is the location of the current cell in this->_fireCells
    size_t pixel = this->_fireCells[src_idx]; // pixel: the actual color value of the src pixel.

    if (pixel == 0) { // Black
        // If our pixel is already black then we propagate the value down one row.
        src_idx = src_idx - this->_width;
        this->_fireCells[src_idx] = 0;
    } else {
        // rnd_idx: this is a random index within 3 pixels.
        // The way this works is as follows:
        // 1) We get a random number between 0.0 and 1.0 inclusive.
        // 2) We multiply that by 3 to get a number between 0.0 and 3.0 inclusive.
        // 3) We floor that value to get an integer.
        // 4) We logically AND that value with 3 which clamps our value.
        size_t rnd_idx = (size_t) floor(this->_rnd() * 3.0) & (uint) 3;
        // We then use this random index to offset our destination value. We add 1 here to avoid negative indices.
        size_t dst = src_idx - rnd_idx + 1;
        // We move down one row
        size_t dst_idx = dst - this->_width;
        // Finally we set the pixel value to either 1 less than the current color, or the current color.
        this->_fireCells[dst_idx] = pixel - (rnd_idx & (size_t) 1);
    }
}

// This draws a checkerboard in our color palette gradient.
// This was used earlier in development for testing various things.
void DemoFire::drawCheck() {
    bool is_color_pixel = false;
    int color_index = 0;

    for (int y = 0; y < (this->_height - 1); y++) {
        for (int x = 0; x < this->_width; x++) {
            if (is_color_pixel)
                this->_fireCells[y * this->_width + x] = (PALETTE_SIZE - 1);
            else
                this->_fireCells[y * this->_width + x] = color_index;

            // Every 8th flip colour
            if (!(x % 8)) {
                is_color_pixel = !is_color_pixel;
            }
        }

        // Every 8th flip colour
        if (!(y % 8)) {
            color_index = ++color_index % PALETTE_SIZE;
            is_color_pixel = !is_color_pixel;
        }
    }
}

// Resizes and re-initializes our simulation.
void DemoFire::resize(size_t w, size_t h) {
    this->_width = w;
    this->_height = h;
    this->_fireSize = w * h;

    this->_initFire(false);
}

