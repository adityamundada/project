Title of the project : 2048 game

Name                 : Aditya Mundada

MIS ID               : 111403001


/*****************************

Description of the work done

*****************************/


This program tries to implement the 2048 game. 
2048 is played on a 4x4 grid, with numbered tiles that slide smoothly when a player moves them using the four arrow keys. Every turn, a new tile will randomly appear in an empty spot on the board with a value of either 2 or 4 (2 has more probability). Tiles slide as far as possible in the chosen direction until they are stopped by either another tile or the edge of the grid. If two tiles of the SAME number collide while moving, they will merge into a tile with the total value of the two tiles that collided. The resulting tile cannot merge with another tile again in the same move.
A scoreboard keeps the track of the user's score. the user's score starts at zero, and is incremented whenever two tiles combine, by the value of the new tile. The game is won when a tile with a value of 2048 appears on the board.

In this project, an unsigned char 4x4 matrix is taken. It is initialized with zeroes. The logic for UP was written. RIGHT, LEFT, DOWN then are implemented in terms of UP by rotating the board. All the required calculations are then performed based on user's input. The input is buffered.



