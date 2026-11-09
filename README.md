# Maze Game
This is a simple maze game written in C with Raylib.

# Dependencies
This project requires Raylib and gcc. See https://github.com/raysan5/raylib and https://gcc.gnu.org/ for installation instructions for your platform. If you know what you're doing you can change `makefile` to use a different compiler.

# Compile & Run
1. Create a directory called `bin`. The `bin` directory is ignored by git.
2. Run `make compile` from within the root directory. This will compile the game.
3. Run `./bin/maze-game` (or the equivalent on your platform).

# Mazes
Mazes are defined in `.txt` files in the `mazes` directory. You can use the mazes included in this repo or create your own.

To create your own mazes, use `#` for walls, `S` for the player's starting position, `E` for the goal, and whitespace for walkable areas. Below is an example:

```
###########
S       # #
# ## ## # #
# #    ## #
######    #
# #   ### #
# # #   # #
#   ###   #
# # # # ###
# # #     #
###E#######
```

Edit `MAZE_FILE` in `main.c` to change which maze is loaded. You can also change `BLOCK_SIZE` to control the size of the grid squares used in the maze if you want. The game will assume that all rows of the maze have the same length because I am lazy.