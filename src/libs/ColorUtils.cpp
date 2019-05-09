//
// Created by corwin on 5/8/19.
//

#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include "ColorUtils.h"

HsvColor ColorUtils::rgb2hsv(const sf::Color rgb) {
    HsvColor hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0) {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

sf::Color ColorUtils::hsv2rgb(const HsvColor hsv) {
    sf::Color rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> (uint) 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> (uint) 8))) >> (uint) 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> (uint) 8))) >> (uint) 8;

    switch (region) {
        case 0:
            rgb.r = hsv.v;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = hsv.v;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = hsv.v;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}

size_t interpolateLinear(const double v1, const double v2, const double mu) {
    return ceil(v1 * (1 - mu) + v2 * mu);
}

size_t interpolateCosine(const double y1, const double y2, const double mu) {
    double mu2;

    mu2 = (1 - cos(mu * M_PI)) / 2;
    return ceil(y1 * (1 - mu2) + y2 * mu2);
}

sf::Color ColorUtils::lerpColorHsv(sf::Color c0, sf::Color c1, const double t,
                                   const InterpolationFunction::InterpolationFunction func = InterpolationFunction::Linear) {
    if (t == 0) return c0;
    else if (t == 1) return c1;

    const auto a = rgb2hsv(c0);
    const auto b = rgb2hsv(c1);

    size_t (*f_pointer)(double, double, double);

    switch (func) {
        case InterpolationFunction::Linear:
            f_pointer = interpolateLinear;
            break;
        case InterpolationFunction::Cosine:
            f_pointer = interpolateCosine;
            break;
        default:
            f_pointer = interpolateLinear;
            break;
    }

    auto out = HsvColor{
            f_pointer(a.h, b.h, t),
            f_pointer(a.s, b.s, t),
            f_pointer(a.v, b.v, t)
    };

    return hsv2rgb(out);
}

sf::Color ColorUtils::lerpColorRgb(sf::Color c0, sf::Color c1, const double t,
                                   InterpolationFunction::InterpolationFunction func = InterpolationFunction::Linear) {
    if (t == 0) return c0;
    else if (t == 1) return c1;

    size_t (*f_pointer)(double, double, double);

    switch (func) {
        case InterpolationFunction::Linear:
            f_pointer = interpolateLinear;
            break;
        case InterpolationFunction::Cosine:
            f_pointer = interpolateCosine;
            break;
        default:
            f_pointer = interpolateLinear;
            break;
    }

    auto out = sf::Color(
            f_pointer(c0.r, c1.r, t),
            f_pointer(c0.g, c1.g, t),
            f_pointer(c0.b, c1.b, t)
    );

    return out;
}