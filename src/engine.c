#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>

#define PI_F 3.14159265f

#define TITLE	"3D Game Engine"
#define WIDTH	768
#define HEIGHT	512

typedef struct {
	float x, y, z;
} vec3;

typedef struct
{
	float m[4][4];
} mat4x4;

struct triangle {
	vec3 v1, v2, v3;
};

void MultiplyMatrixVector(vec3 i, mat4x4 m, vec3 *o)
{
	o->x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o->y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o->z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = o->z = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
	if (w != 0.0f) {
		o->x /= w;
		o->y /= w;
		o->z /= w;
	}
}

int
main(int argc, char *argv[])
{

	if (SDL_Init(SDL_INIT_VIDEO) != EXIT_SUCCESS) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	SDL_Window *win = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, 0);
	if (win == NULL) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(
		win, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_SetRenderDrawColor(ren, 30, 30, 30, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren, 200, 0, 200, SDL_ALPHA_OPAQUE);

	struct triangle my_mesh[] = {
		// SOUTH
		{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f, 0.0f } },
		{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f}, { 1.0f, 0.0f, 0.0f } },
		// EAST
		{ { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f, 1.0f } },
		// NORTH
		{ { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
		// WEST
		{ { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f}, { 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } },
		// TOP
		{ { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 0.0f } },
		// BOTTOM
		{ { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 0.0f } },
		{ { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f } }
	};

	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 70.0f;
	float fAspectRatio = (float)HEIGHT / (float)WIDTH;
	float fFovRad = 1.0f / tanf(fFov * 0.5f * PI_F / 180.0f);

	mat4x4 matProj;
	matProj.m[0][0] = fAspectRatio * fFovRad;
	matProj.m[1][1] = fFovRad;
	matProj.m[2][2] = fFar / (fFar - fNear);
	matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj.m[2][3] = 1.0f;
	matProj.m[3][3] = 0.0f;

	for (int i = 0; i < 12; i++) {
		struct triangle triProjected, triTranslated;

		triTranslated = my_mesh[i];
		triTranslated.v1.z += 3.0f;
		triTranslated.v2.z += 3.0f;
		triTranslated.v3.z += 3.0f;

		triTranslated.v1.x -= 1.5f;
		triTranslated.v2.x -= 1.5f;
		triTranslated.v3.x -= 1.5f;

		triTranslated.v1.y += 0.8f;
		triTranslated.v2.y += 0.8f;
		triTranslated.v3.y += 0.8f;



		MultiplyMatrixVector(triTranslated.v1, matProj, &triProjected.v1);
		MultiplyMatrixVector(triTranslated.v2, matProj, &triProjected.v2);
		MultiplyMatrixVector(triTranslated.v3, matProj, &triProjected.v3);

		// Scale into view
		triProjected.v1.x += 1.0f; triProjected.v1.y += 1.0f;
		triProjected.v2.x += 1.0f; triProjected.v2.y += 1.0f;
		triProjected.v3.x += 1.0f; triProjected.v3.y += 1.0f;

		triProjected.v1.x *= 0.5f * (float)WIDTH;
		triProjected.v1.y *= 0.5f * (float)HEIGHT;
		triProjected.v2.x *= 0.5f * (float)WIDTH;
		triProjected.v2.y *= 0.5f * (float)HEIGHT;
		triProjected.v3.x *= 0.5f * (float)WIDTH;
		triProjected.v3.y *= 0.5f * (float)HEIGHT;


		SDL_RenderDrawLine(ren, triProjected.v1.x, triProjected.v1.y, triProjected.v2.x, triProjected.v2.y);
		SDL_RenderDrawLine(ren, triProjected.v2.x, triProjected.v2.y, triProjected.v3.x, triProjected.v3.y);
		SDL_RenderDrawLine(ren, triProjected.v3.x, triProjected.v3.y, triProjected.v1.x, triProjected.v1.y);

	}

	SDL_RenderPresent(ren);

	SDL_Delay(3000);

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return EXIT_SUCCESS;
}
