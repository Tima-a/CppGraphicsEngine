# C++ Graphics Engine

C++ Graphics Engine is a graphics, physics engine written on C++ fully from scratch for building desktop applications and games on C++ which is still in progress.<br />
**Key Features:**

* **Interface:** Simple, flexible and user-friendly programming interface.
* **Rendering:** Rendering basic 2D/3D shapes, images, and text.
* **Media & Input:** Audio configuration, camera control and manual input handling.
* **Physics Simulation:** Functional physics simulation program.

I started this project in Summer 2019 by the pure excitement and challenge of building my own game development programs fully from scratch. The project was done in my early stages of programming.

# Rectangles
![cpp1](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/60bf005b-a6c4-4d63-bce1-c14e10356754)

# Vectors
![cpp2](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/99dfebb2-fe3d-4380-a13a-9267dd55072d)
# Triangles
![cpp5](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/f7df8fca-84f1-4a17-9989-c57234abcec8)

# Ellipses
![cpp3](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/2f52b510-c66b-4fc6-859d-7fc0c4bff027)
![cpp4](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/5f97c45d-6a62-4bfe-99eb-e5da64bb374d)
# 2D Text
The 2D Text object is created by dividing the user-input text into chars and drawing them separately. Every char is drawn using Arial font image and dividing it into tile set of characters.<br />
Text class supports drawing all English alphabet letters, numbers and special characters.<br />
![cpp6new](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/c59dd3a7-c6b6-4987-8a12-275594ee7693)

# Images, Sprites
For importing image stb image library was used to retrieve each pixel of the image in RGB format. To resize, move an image a Sprite class has to be initalized, there are many filters.
![cpp7](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/b3536f74-5371-4b33-875a-27c7147f06b8)
![cpp8](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/164c592a-9643-4348-a961-2a011d9a0c85)

Here, the sprite is half in width and height, and a grayscale filter is applied.
# Rotation
All mentioned objects can be rotated. <br>
Rotation works with two main equations:

$$x = cos(angle) * (x_{origin})-sin(angle) * (y-y_{origin})+x_{origin}$$

$$y = sin(angle) * (x-x_{origin}) + cos(angle) * (y-y_{origin})+y_{origin}$$

x and y is the position of the rotating pixel, origin_x, and origin_y are pivot point around which pixel is rotated.<br>
Rotation has different pivots, so it can rotate object around its center, around its starting position, ending position, and around any arbitrary point.<br>
Object can be rotated clockwise and counterclockwise.<br>
To rotate an object my engine rotates all pixels of that object around a point using mentioned formula.<br>
![2023-09-30 18-42-31 (online-video-cutter com) (1)](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/0c383c03-890a-4064-a687-feb8213a69db)
# Dynamic Windows
Also I created Dynamic Windows class which can create new windows, change their size, position, name, and options at runtime. Each window can be coded independently. It can create up to 100 windows.<br />
I used Windows.h library for implementing this.<br />
# Physics
My engine also has basic Physics simulation. It can simulate object being thrown at a specific angle, initial velocity, wind resistance, wind angle, air resistance, substance resistance, altitude, air humidity. Firstly, user must create a PhysicsComponent class and call ApplyPhysicsComponent() which requires two variables. First is the mass of the object and second is the object itself. Then, user calls StartPhysicsSimulation() to start the simulation and after it call ApplyPhysicsTransformation(). Then, user inputs all required variables and defines types of air resistance and wind resistance. There are three types of air resistance: no air resistance, default air resistance, user-inputted air resistance(substance resistance, for example object is falling in water, oil).
Furthermore, there are three types of wind resistance: no wind, wind with constant velocity, accelerating or decelerating wind. Furthermore, user can set air density by calling SetAirDensityParametersHeight which requires air type(no air, dry air, humid air), temperature, altitude, specific humidity and relative humidity. The formula for air resistance is
![alt text](https://i.ibb.co/zRWDxYr/formula-ar.png) <br>
Here $F_d$ is air resistance force, A is area of the object, v - velocity, p - is density of air(substance) and $C_d$ is a special coefficent of the object which depends on its structure, area and many other factors.<br />
![2023-09-30 18-16-14 (online-video-cutter com) (2)](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/82158179-85f9-4052-9c71-8b6ff09eebe4)
Here, an object with a mass of 1 kg is thrown at 25 degrees, with an initial velocity of 20 m/s, and accelerating wind is acting from 90 degrees (from the right side of the screen to the left) with a wind acceleration of 4 m/s^2. The air is dry at an altitude of 10,000 meters. All these values can be modified which impact air resistance and wind resistance.
# Utility
There are dozens of handy functions in utility.cpp which are primarily designed for rendering.cpp and game.cpp. The one of the most important goals in my engine was to not use any existing code, powerful libraries like iostream, string, STL and etc.
So, I designed various algorithms to convert int to const char* and vice versa, get substring of const char*, check if they are equal. This functions are used in text class and other vital areas with advanced floor and trunc techniques and many other very useful functions.
# Audio
Any type of audio can be played, especially .mp3 format. Audio class has many functions as to stop the current audio track, change its volume(left and right also), speed, get treble, bass, bit depth and sample frequency.
Audio class works with mci library which I made much more simple to use and added more functionality.<br />
# Input
Improved Windows keyboard input and made it more simple and user-friendly with all VKL codes replaced by characters they represent. Added more functions such as if any key is pressed, key is up, pressed, released. <br />
# Camera
To create a 2D camera user must create a camera class object with current position x, position y, and render width and height. If no camera is active, a default black screen will be shown. <br />
# Buttons
User can create buttons by instantiating button class object and specifying region which button covers and which action should be done to activate(press a specific key or mouse button). <br />
# Author
Teymur Aghayev
Email: teymur.aghayev@stud.vilniustech.lt




