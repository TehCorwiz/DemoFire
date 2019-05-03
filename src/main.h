//
// Created by corwin on 5/3/19.
//

#ifndef DEMOFIRE_MAIN_H
#define DEMOFIRE_MAIN_H

#include <cmath>
#include <cstdio>
#include "DemoFire.h"

/* Simulation constants */

// HEIGHT: The value was chosen due to how the original algo works. PALETTE_SIZE (defined in DemoFire.h) affects how
// high the flames are drawn. Thus we need a value that isn't too short or too tall. The original value, 168 looks
// really good with some but not too much vertical space to allow for little `sparks`. However, since the resolution is
// dependent on the PALETTE_SIZE in the first place, I'm adopting a calculated calue in order to support additional
// resolutions down the road.
constexpr static size_t HEIGHT = 4 * DemoFire::PALETTE_SIZE;
// WIDTH: The value uses the above to choose a width that results in a 16:9 ratio which is the current most common
// screen size.
const static size_t WIDTH = floor(HEIGHT * (16.0 / 9.0));
// SCALE: This value set the default scale of the window. Since HEIGHT is externally dependent it's easier to draw at
// the above HEIGHT * WIDTH resolution and just scale up the entire texture.
const static size_t SCALE = 4;

// TARGET_TICK_RATE: This is our target FPS (more or less) down the road I may split ticks from frames and run the sim
// at either a higher or lower tick rate, but keep FPS locked to some common value, 30 or 60).
const static uint TARGET_TICK_RATE = 28;
// SECOND_NS: The number of nanoseconds in a second.
const static uint SECOND_NS = 1000000;
// TICK_NS: The number of nanoseconds per tick for our chosen rate.
const static uint TICK_NS = SECOND_NS / TARGET_TICK_RATE;


/* Work objects */
sf::Event event; // used to hold data about triggered events. SFML example code had this as a global.

DemoFire demo_fire(WIDTH, HEIGHT, false); // Construct DemoFire object to run the sim.
sf::Image fire_image; // Construct Image to write pixels onto.
sf::Texture fire_texture; // Constructs Texture onto which we can draw our Image.
sf::RectangleShape screen_rect; // Constructs a rectangle which takes our texture and can be draw to our window.

#endif //DEMOFIRE_MAIN_H
