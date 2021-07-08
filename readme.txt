

INB381 Assignment 1:












Student Name:		Jackson Powell
Student ID:			08600571
Date:				19/9/2014


Statement of Completeness:

Student Name:		Jackson Powell
Student ID:		08600571
Date:			19/9/2014
I completed all tasks in the assignment; this includes all tasks along with their entire features to the best of my ability and knowledge. I did not come across any problems that I couldn’t work around.
* There are a couple questions from tutorials that I just couldn’t understand and tried to attempt but ended up not being able to.
Description of Program code:
To run the application, simply open it in visual studio 2010 (what I used) and run I didn’t include the .exe debug file in case there were security issues that arose.
New Feature:
The feature I implemented was the use of a projection camera and the ability to manipulate the environment to such extents that give the “game” a bit of depth in a sense that you can view and interact from all angles using keyboard characters (see below). This should be included because the original projection was too basic and didn’t really offer the visual enhancements that 3D can provide.
==> My program can be run in Visual Studio 2010, C++ environment (or just using code hopefully).
==> Shouldn’t require any libraries besides freeGLUT and glew, and if it does they are included already.
Shaders:
The shaders are extremely basic in that they use the object’s world position to colourise the object vertices. When the shaders are passed a red value (for colour picking of objects using mouse) then they are given that red colour, i.e. r = 1 g = 0 b = 0 for every vertex so the colour pick can pick it up.

Controls:
* Click an object with left mouse button to increase it’s speed (spheres only).
* Click an object with right mouse button to decrease speed (until stand still, spheres only).
* Click and drag to new place with squares to initiate the change of vertical bounce positions.
* ‘q’ keyboard button exits the program.
* W, a, s, d (keyboard) all act as controllers for the scene’s bounds, i.e. w (up) increases the height of the two squares environment space, s (down) decreases to a limit, s decreases the size of the environment horizontally and finally d increases it. These are parts of my new feature I implemented.
* Z and x (left and right) are used to turn the environment in a sense that you are looking at it from a different angle, these keys move the camera around horizontally in a circle, always looking at the centre of the environment.
Screenshots (progress pictures too):

Finished:



Different projections/camera angles:



Moved squares apart:


Without shader colouring.

