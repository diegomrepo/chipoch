/* 
 * TODO 
 * Complete sound support.
 *
 * */

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const double ChromaticRatio = 1.059463094359295264562;
const double Tao = 6.283185307179586476925;

Uint32 sampleRate = 48000;
Uint32 frameRate = 60;
Uint32 floatStreamLength = 1024;

typedef struct {
	float *waveform;
	Uint32 waveformLength;
	double volume;
	double pan;
	double frequency;
	double phase;
} voice;
