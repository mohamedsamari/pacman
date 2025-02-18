# pacman
pacman project build by C-language

## Overview

**Pacman** is a game project inspired by the classic arcade game. This documentation provides an overview of the main functions, logic, file structure, and graphical elements, including AI behavior for ghosts.

---

## Main Function and Logic

### Main Function

The **main function** serves as the entry point for the Pacman game. It manages initialization, the game loop, and cleanup.

1. **Initialization:**
   - Load assets (images, sounds, fonts).
   - Set up the game window and frame rate.
   - Initialize variables (player position, ghost AI, score, etc.).

2. **Game Loop:**
   - Process user input (movement, pause, restart).
   - Update game objects (Pacman, ghosts, pellets, and power-ups).
   - Detect and handle collisions (Pacman vs. walls, ghosts, pellets).
   - Render the game frame.

3. **Cleanup:**
   - Release dynamically allocated memory.
   - Close the game window properly.

---

## Scores File

The **scores file** keeps track of player scores and high scores. It typically operates as follows:

- **Saving Scores:**
  - Append the player's score to a file (`scores.txt`).
  - Store the player’s name and score in a structured format.

- **Loading Scores:**
  - Read scores from the file at startup.
  - Sort and display the highest scores.

- **Resetting Scores:**
  - Optionally clear all stored scores.

---

## Graphics and Logic Files

The project is divided into separate files for graphics and logic.

1. **Graphics File - `graphics.c`:**
   - Handles rendering Pacman, ghosts, and the maze.
   - Manages animations and sprite updates.

2. **Logic File - `logic.c`:**
   - Implements ghost AI (random movement, chase mode, scatter mode).
   - Handles state transitions and timers.
   - Manages collision detection.

---

## Conclusion

This documentation outlines the key components of the Pacman project, covering game logic, score management, and separate modules for graphics and AI. With this structure, developers can expand and improve the game efficiently.

---
