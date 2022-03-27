# OpenGLProject
My OpenGL project. Feel free to suggest improvements/new features. All assets and libraries used are free to use, and the code from the Tutorial I used in the beginning is as well.

# Compilation
Go in the build folder and use cmake to build.
Required libraries are [OpenGL](https://www.opengl.org/), [SDL](https://www.libsdl.org/), [Glew](http://glew.sourceforge.net/) and [assimp](https://github.com/assimp/assimp)

I have not yet tested this on Windows.

# Sources
[Tutorial (German)](https://www.youtube.com/watch?v=yRYHly3bl2Q&list=PLStQc0GqppuWBDuNWnkQ8rzmyx35AINyt&index=1)

[Glm](https://github.com/g-truc/glm)

[StbImage/StbTrueType](https://github.com/nothings/stb)

[Assets](https://maxparata.itch.io/voxel-mechas)

# Performance
On my machine (Intel 11th Gen i5 with iris Xe integrated graphics) the Quadruped Tank model can be loaded roughly 300 times until my framerate starts going below 60 FPS consistently. The model has 24,804 indices, or 8,268 triangles. 300 * 8,268 = 2,480,400 triangles per frame
