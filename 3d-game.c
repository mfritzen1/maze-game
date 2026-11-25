#include <stdio.h>
#include "raylib.h"
#include "rcamera.h"

#define MAZE_FILE "mazes/maze-1.txt"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define MAZE_SCALE 3

int game3d()
{
	FILE *mazeDataSp;   // Stream pointer to the maze file
	int currentChar;    // character currently being read from file stream
	int mazeHeight = 0; // number of characters tall that the maze is
	int mazeWidth = 0;  // number of characters wide that the maze is
	int firstLine = 1;  // "boolean" indicating whether we're reading the first line of the maze or
	float mazeScalingFactor = (float)MAZE_SCALE;

	Camera camera = { 0 };
	camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
	camera.target = (Vector3) { 0.0f, 2.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	int cameraMode = CAMERA_FIRST_PERSON;

	if ((mazeDataSp = fopen(MAZE_FILE, "r")) == NULL)
	{
		printf("Error opening maze file\n");
		return 1;
	}

	while ((currentChar = fgetc(mazeDataSp)) != EOF)
	{
		if (firstLine > 0)
		{
			mazeWidth++;
		}

		if (currentChar == '\n')
		{
			mazeHeight++;
			firstLine = 0;
		}
	}

	mazeWidth--;
	mazeHeight++;

	rewind(mazeDataSp);

	char maze[mazeWidth][mazeHeight];

	for (int i = 0; i < mazeHeight; i++)
	{
		for (int j = 0; j < mazeWidth; j++)
		{
			maze[i][j] = fgetc(mazeDataSp);
			if (maze[i][j] == 'S')
			{
				camera.position = (Vector3){ (float)i * mazeScalingFactor, 2.0f, (float)j * mazeScalingFactor  };
			}
		}
		fgetc(mazeDataSp); // disregard newline
	}

	if (fclose(mazeDataSp) == EOF)
	{
		printf("Error closing maze file\n");
		return 1;
	}

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Maze Game");
	DisableCursor();
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		ClearBackground(RAYWHITE);
		UpdateCamera(&camera, cameraMode);
		BeginDrawing();
		BeginMode3D(camera);
		DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f * mazeScalingFactor, 32.0f * mazeScalingFactor }, LIGHTGRAY); // Draw ground
		for (int i = 0; i < mazeWidth; i++)
		{
			for (int j = 0; j < mazeHeight; j++)
			{
				if (maze[i][j] == '#')
				{
					DrawCube((Vector3){ (float)i * mazeScalingFactor, 0.0f, (float)j * mazeScalingFactor }, mazeScalingFactor, mazeScalingFactor * 3, mazeScalingFactor, DARKGRAY);
					DrawCubeWires((Vector3){ (float)i * mazeScalingFactor, 0.0f, (float)j * mazeScalingFactor }, mazeScalingFactor, mazeScalingFactor * 3, mazeScalingFactor, BLACK);
				}
				else if (maze[i][j] == 'S')
				{
					DrawCube((Vector3){ (float)i * mazeScalingFactor, 0.0f, (float)j * mazeScalingFactor }, mazeScalingFactor, 0.1f, mazeScalingFactor, BLUE);
				}
				else if (maze[i][j] == 'E')
				{
					DrawCube((Vector3){ (float)i * mazeScalingFactor, 0.0f, (float)j * mazeScalingFactor }, mazeScalingFactor, 0.1f, mazeScalingFactor, GREEN);
				}
			}
		}
		EndMode3D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
