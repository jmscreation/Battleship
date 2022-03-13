# Battleship
Project: "Battleship in a day"

Note:
This game was made in less than 10 hours.

# How to Build

This comes with a pre-configured build script. In order to build this project, you need to first install into the repository some dependency libraries. This can be done very quickly using the [Dependency Tracker](https://github.com/jmscreation/dependency-tracker).
With the Dependency Tracker configured, run the `deps -update` in this repository to automatically update all dependencies.

Now for the build process, you need to use the GCC MinGW 64bit compiler. You can run the build script provided once you have your compiler bin folder path properly configured.

Other compiler support is unavailable because this project relies on prebuilt Windows 64bit static libraries *which were compiled with MinGW64 MSVCRT*

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

Currently the game play is quite boring and limited to this basic interaction.
