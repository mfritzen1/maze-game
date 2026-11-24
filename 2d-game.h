#include <stdio.h>
#include <math.h>
#include "raylib.h"

int game2d();
float stickMovementAfterDeadzone(float stick, float deadzone);
int getSimpleStickDirection(float leftStickX, float leftStickY);
