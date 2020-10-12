# Voxel

Attempt at a game engine.

The purpose of this project is not to create a useful product but simply to challenge myself and to learn more. To that end, I have limited myself to only using the C standard library plus the Xlib headers (necessary to create a window and draw graphics on most Unix systems) and OpenCL, which is used to run code on GPUs.

I achieved the goal of making a simple but relatively fun and only mildly glitchy game with it, as you can see in [this video](https://www.youtube.com/watch?v=Y8x7WgBPV1Q&t=23s)

The game pictured is not in the current repo as it was used as a test of what needed to change, and the answer was just about everything. So it is no longer supported by the current version, but still exists in the official [v0.0.1 commit](https://github.com/Digit112/Voxel/releases/tag/v0.0.1) and can be compiled and played on Unix systems using the X Window System by linking Xlib with the -lX11 option.

The project is on hiatus. When I get back around to it I intend to add texturing, and shortly after that specular and normal map support.
