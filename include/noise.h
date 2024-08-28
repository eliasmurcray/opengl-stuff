#ifndef NOISE_H
#define NOISE_H

#include <stdlib.h>
/**
 * Port of Java code for improved Perlin noise:
 * https://mrl.cs.nyu.edu/~perlin/noise/
 * Original author Ken Perlin
 **/
float noise(float x, float y);

float fbm(float x, float y, int octaves, float persistence);

#endif
