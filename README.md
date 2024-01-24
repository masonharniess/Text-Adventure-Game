# Text-Adventure-Game
Coursework assignment for a C++ module at the University of Leicester. 

## Instructions
1. Download repository code.
2. In Terminal, go the the project folder and execute the  command ```./main resources/[map.json]``` where ```[map.json]``` is your preferred map (i.e. ```map1.json```)
3. Once the game has begun, the player can use the following input commands to interact with the environment:
    - ```look``` or ```look around``` to view the information about the room.
    - ```go x``` where ```x``` is an exit of that name within the current room (i.e. ```go east```, ```go left```).
    - ```take x``` where ```x``` is an object of that name within the current room (i.e. ```take gun```).
    - ```kill x``` where ```x``` is an enemy of that name within the current room (i.e. ```kill zombie```).
4. An objective of a map can be one of three things. The goal is specified in the map data file, though the goal is not specified to the player:
    - to kill a certain set of enemies.
    - to collect a certain set of objects.
    - to arrive at a certain room.
