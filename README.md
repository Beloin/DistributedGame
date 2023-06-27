# Project

A distributed `Small Game` using OpenGL and Linux native Socket connections.

## The Game

A simple triangle controller game. Each player will be a triangle running in a 500x500 screen. The player can move up, down to the left and right `(🡑 🡓 🡐 🡒)`. The important thing here is the distributed part, where there's not going to have a main server, every user will connect to each other and talk his place to everyone.

To achieve this Goal the programmer has two ways to implement it:
1. Centralized Option:
    - One of the players will be a coordenator and each moviment will be sent to it. So the coordenator have to send every client the changed position.
    - This opion includes having a voting algorithm (Bully or ring) in order to choose the next coordenator if one of them fails to respond.
    - This option will help when having a shared resource.
1. Full distribuited option:
    - Wont have any coordenator, each change in places should be spread across every client (see distributed algorithms).
    - The places should be acted as a resource, so it can be the same across the clients.
    - Will be slower, but wont need the voting algorithm.

For both the implementations will be needed a "start" point. Which means a connecting point to enter to that distribuited group.

## The implementation

Each `player` will have a server code and a client code, so it can reach out to other players and communicate it's position.
