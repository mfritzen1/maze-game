#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define MAZE_FILE "mazes/maze-1.txt"
#define BLOCK_SIZE 50

float stickMovementAfterDeadzone(float stick, float deadzone);
int getSimpleStickDirection(float leftStickX, float leftStickY);

int main()
{
  FILE *mazeDataSp;                 // Stream pointer to the maze file
  int currentChar;                  // character currently being read from file stream
  int mazeHeight = 0;               // number of characters tall that the maze is
  int mazeWidth = 0;                // number of characters wide that the maze is
  int firstLine = 1;                // "boolean" indicating whether we're reading the first line of the maze or a later line, used to save some operations by only incrementing mazeWidth on the first row
  int playerX;                      // X coordinate of the player
  int playerY;                      // Y coordinate of the player
  int startX;                       // X coordinate of the starting position
  int startY;                       // Y coordinate of the starting position
  int endX;                         // X coordinate of the maze exit
  int endY;                         // Y coordinate of the maze exit
  int gotoX;                        // X coordinate that the player is trying to move to
  int gotoY;                        // Y coordinate that the player is trying to move to
  int gamepad = 0;                  // id for gamepad
  float leftStickX;                 // x coordinate of gamepad left stick
  float leftStickY;                 // y coordinate of gamepad left stick
  float leftStickDeadzone = 0.1f;   // deadzone for gamepad sticks
  int simpleStickDirection;         // the simple direction the stick is pointing (up, down, left, right)
  bool stickAlreadyPressed = false; // boolean indicating whether the stick is continuously pressed

  if ((mazeDataSp = fopen(MAZE_FILE, "r")) == NULL)
  {
    printf("Error opening maze file\n");
    return 1;
  }

  // read the maze file to determine the number of rows and columns it has, used to define the maze array below
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

  mazeWidth--;  // removing one for the EOL character
  mazeHeight++; // adding one because I think it's not reading a \n on the last line?

  rewind(mazeDataSp); // reset to beginning of file

  char maze[mazeWidth][mazeHeight];

  // read the maze file again to populate a maze array with coords for the walls, player starting position, and maze exit
  // I don't like reading the maze file a second time, but I'm not a good enough C programmer yet to know how to do it without restricting the size of the maze
  for (int i = 0; i < mazeHeight; i++)
  {
    for (int j = 0; j < mazeWidth; j++)
    {
      maze[i][j] = fgetc(mazeDataSp);
      if (maze[i][j] == 'S')
      {
        playerX = j;
        playerY = i;
        startX = j;
        startY = i;
      }
      else if (maze[i][j] == 'E')
      {
        endX = j;
        endY = i;
      }
    }
    fgetc(mazeDataSp); // fuck newline characters, we don't need them anymore
  }

  if (fclose(mazeDataSp) == EOF)
  {
    printf("Error closing maze file\n");
    return 1;
  }

  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

  InitWindow(mazeWidth * BLOCK_SIZE, (mazeHeight * BLOCK_SIZE) + 170, "Maze Game");

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(WHITE);

    DrawText("Move with arrow keys", 10, 10, 30, DARKGRAY);
    DrawText("Reset with r", 10, 50, 30, DARKGRAY);
    DrawText(TextFormat("Current position: %d, %d", playerX, playerY), 10, 90, 30, DARKGRAY);

    for (int i = 0; i < mazeHeight; i++)
    {
      for (int j = 0; j < mazeWidth; j++)
      {
        if (maze[j][i] == '#') // idk why the coords need to be backwards on maze
        {
          DrawRectangle(i * BLOCK_SIZE, j * BLOCK_SIZE + 170, BLOCK_SIZE, BLOCK_SIZE, BLACK);
        }
        else if (i == playerX && j == playerY)
        {
          DrawRectangle(i * BLOCK_SIZE, j * BLOCK_SIZE + 170, BLOCK_SIZE, BLOCK_SIZE, BLUE);
        }
        else if (i == endX && j == endY)
        {
          DrawRectangle(i * BLOCK_SIZE, j * BLOCK_SIZE + 170, BLOCK_SIZE, BLOCK_SIZE, GREEN);
        }
      }
    }

    leftStickX = stickMovementAfterDeadzone(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X), leftStickDeadzone);
    leftStickY = stickMovementAfterDeadzone(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y), leftStickDeadzone);

    if (leftStickX == 0.0f && leftStickY == 0.0f)
    {
      stickAlreadyPressed = false;
    }

    simpleStickDirection = getSimpleStickDirection(leftStickX, leftStickY);

    if (IsKeyPressed(KEY_RIGHT) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (simpleStickDirection == 2 && !stickAlreadyPressed))
    {
      gotoX = playerX + 1;
      if (maze[playerY][gotoX] != '#' && gotoX >= 0 && gotoX < mazeWidth)
      {
        playerX = gotoX;
      }
      stickAlreadyPressed = true;
    }
    else if (IsKeyPressed(KEY_LEFT) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (simpleStickDirection == 4 && !stickAlreadyPressed))
    {
      gotoX = playerX - 1;
      if (maze[playerY][gotoX] != '#' && gotoX >= 0 && gotoX < mazeWidth)
      {
        playerX = gotoX;
      }
      stickAlreadyPressed = true;
    }
    else if (IsKeyPressed(KEY_UP) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP) || (simpleStickDirection == 1 && !stickAlreadyPressed))
    {
      gotoY = playerY - 1;
      if (maze[gotoY][playerX] != '#' && gotoY >= 0 && gotoY < mazeHeight)
      {
        playerY = gotoY;
      }
      stickAlreadyPressed = true;
    }
    else if (IsKeyPressed(KEY_DOWN) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || (simpleStickDirection == 3 && !stickAlreadyPressed))
    {
      gotoY = playerY + 1;
      if (maze[gotoY][playerX] != '#' && gotoY >= 0 && gotoY < mazeHeight)
      {
        playerY = gotoY;
      }
      stickAlreadyPressed = true;
    }

    if (IsKeyPressed(KEY_R) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
    {
      playerX = startX;
      playerY = startY;
    }

    if (playerX == endX && playerY == endY)
    {
      DrawText("YOU WIN!", 10, 130, 30, RED);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

/* Returns the stick movement value, if it is outside of the deadzone
 * Returns 0 if it is within the deadzone
 */
float stickMovementAfterDeadzone(float stick, float deadzone)
{
  if (stick > -deadzone && stick < deadzone)
  {
    return 0.0f;
  }
  return stick;
}

/* Returns an int indicating a simple direction (left, right, up, down that the gamepad stick is pressed
 * 1 = up
 * 2 = right
 * 3 = down
 * 4 = left
 * 0 = unknown
 */
int getSimpleStickDirection(float leftStickX, float leftStickY)
{
  float absX = fabsf(leftStickX);
  float absY = fabsf(leftStickY);

  if (absX > absY)
  {
    if (leftStickX > 0)
    {
      return 2; // go right
    }
    return 4; // go left
  }
  else if (absX < absY)
  {
    if (leftStickY > 0)
    {
      return 3; // go down
    }
    return 1; // go up
  }

  return 0;
}
