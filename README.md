# Battleship
Project: "Battleship in a day"

Note:
This game was made in less than 10 hours.

# How to Build

To build using the GCC MinGW 64bit compiler, run the build script provided. You need to have your compiler bin folder path configured.

Other compiler support is unavailable, as this project relies on prebuilt Windows 64bit static libraries.

# Launching the Game

This active battleship game was made so quickly that no menu was created for it.
To launch the game, you must pass one of the following parameters to the program:

In order to launch the game as a host, pass the `host` command when launching the game.

In order to connect to a host waiting for a player to join, pass the `connect <ip>` command when launching the game. *(where \<ip\> is the host machine address)*

# Playing the Game

The gameplay is similar to Battleship, however it is not turn based.

You must select a ship, then attack the other player by clicking in an enemy territory square

Hits and misses are displayed in the squares that are clicked on for both players both ways.

With a selected ship, you are able to move the ship by using the arrow keys.

The player to lose all their ships first loses the game. There is currently no victory screen.

Currently the game play is limited to this interaction.
