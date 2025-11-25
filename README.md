# C++ Graphics Engine: Built From Scratch

C++ Graphics Engine is a graphics engine written on C++ fully from scratch for building desktop applications and games on C++ which is still in progress.
**Key Features:**

* **Interface:** Simple, flexible and user-friendly programming interface.
* **Rendering:** Rendering basic 2D/3D shapes, images, and text.
* **Media & Input:** Audio configuration, camera control and manual input handling.
* **Simulation:** Functional physics simulation program.

I started this project in Summer 2019 by the pure excitement and challenge of building my own game development programs fully from scratch.
# Features
Firstly, user must creates the WINDOW class with user-input x, y, width, and height.<br />
The user has simulategame() function which is a main loop function where all user code is written. In the beggining of main loop user must update the window with the refresh_screen(color, window) function.<br />
Every graphics unit should be called as object and after the object is created user should write object_name.draw() and object will be drawn.<br />
Then, vector, and ellipse shapes have two classes of fvector, dvector and ellipsef, ellipsed classes respectively. In dvector and ellipsed classes, when shapes are drawn, the user can access some variables that are really important.<br />
For example, in dvector and ellipsed classes there is an array of x and y positions of all pixels that shape contains. Those classes are often used in other classes like in triangles, rectangles and 2D Text.<br />
On the other side, fvectors and ellipsef classes are faster because they don't save such information. Engine now can draw 300 million pixels in one second <br />
# Rectangles
Actually the first shape I have done is rectangle because of its simplicity.<br />
How does my engine draw rectangles?<br />
It creates a line of width user inputted and draws that line from upper height to a lower one.<br />
![cpp1](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/60bf005b-a6c4-4d63-bce1-c14e10356754)

# Vectors
How does my engine draw vectors?<br />
Firstly, it checks what type of vector it is(straight or not).<br />
If vector is not straight then it calculates the slope of a vector by formula m=(y2-y1)/(x2-x1). Then, it calculates the direction to which the resulting vector should be facing by comparing the (x2-x1) and (y2-y1).<br />
In loop where vector is being drawn there are x and y cycles. In x_cycle engine draws only pixels which differ in x, in y_cycle engine draws pixels which differ only in y.<br />
If in for-loop where there is i as a loop variable then if i divided by 2 yields 1 then it is x_cycle/y_cycle and if i divided by 2 yields 0 then it is again x_cycle/y_cycle. Which cycle is first depends on |x2-x1| and |y2-y1|.<br />
After that, if |x2-x1| > |y2-y1| then it stores slope to variable slope_x, and x_y_outweight equals false and if |x2-x1| < |y2-y1| then to slope_y and x_y_outweight equals true. slope_x and slope_y are initially 1.0f<br />
And the basic principle is to check if in x_cycle x_y_outweight is false, then program starts to draw initially with x_cycle and it draws amount of pixels equivalent to slope_x.<br />
Then, in y_cycle if x_y_outweight is false program draws only one pixel and then goes again to x_cycle.<br />
If |x2-x1| < |y2-y1| and x_y_outweight is true, then it does everything oppositely where in x_cycle engine draws only one pixel and in y_cycle it draws slope_y amount of pixels.<br />
![cpp2](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/99dfebb2-fe3d-4380-a13a-9267dd55072d)
# Triangles
To create triangle, user must input the length of first side(a), the length of second side(b), the length of third side(c), height of the triangle, and x, y positions of triangle.<br />
C is casually the line in the bottom of default triangle which connects a and b.<br />
Firstly it visualizes the triangle by drawing a,b,c depending on which is side is bigger.<br />
Then, it is filled from the point where a and b intersect (let's call it D) by creating vectors from D to every pixel of vector C.<br />
![cpp5](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/f7df8fca-84f1-4a17-9989-c57234abcec8)

# Ellipses
Also, a difficult one, so the main method of creating ellipses is by using sin(t)*radius_width and cos(t)*radius_height to get the position of all pixels the raw ellipse must be built of.<br />
t - is the variable which is initially 360 and is decreasing by 1 every time in loop. (converted to radians before using in sin and cos functions)<br />
Ellipses can also be drawn not fully but partially. For example, only half of it, or from 90 to 270 degrees. User can also define this information(start_circum, end_circum) to draw ellipse partially.<br />
start_circum 0 means the top center pixel which is drawn first.<br />
To fill ellipse, engine connects every pixel of right side of ellipse with the central vertical line of the ellipse and then from left side of ellipse with the central vertical line.<br />
Why not just instantly from left side to right side?<br />
Because ellipses can be drawn partially, for example, an ellipse with a start_circum of 0 and an end_circum of 220 will not work with this approach.<br />
So it firsly fills all the left side of ellipse with central vertical line and then the right side with central vertical line.<br />
![cpp3](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/2f52b510-c66b-4fc6-859d-7fc0c4bff027)
![cpp4](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/5f97c45d-6a62-4bfe-99eb-e5da64bb374d)
# 2D Text
The 2D Text object is created by dividing the user-input text into chars (letters) and drawing them separately. Every char is drawn using Arial font image and dividing it into tile set of characters.<br />
Text class supports drawing all English alphabet letters, numbers and special characters.<br />
![cpp6new](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/c59dd3a7-c6b6-4987-8a12-275594ee7693)

# Images, Sprites
Here I used the stb_image library. It gets every pixel's color of the image in RGB format. Then, my engine draws those pixels as a rectangle and creates an image.<br />
Then if the user wants to resize and move an image, the user creates a Sprite class. Resizing works by increasing or decreasing size of the pixels.<br />
Sprite class also has many filters like red, green, blue and grayscale. Alpha color is used to ignore drawing pixels with specified color provided by user. Now sprite can be represented as a tilemap. <br />
There is setting which lets user to crop the image from x1 to x2 and y1 to y2 and use as tiles.<br />
![cpp7](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/b3536f74-5371-4b33-875a-27c7147f06b8)
![cpp8](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/164c592a-9643-4348-a961-2a011d9a0c85)

Here, the sprite is decreased by 2 in width and height, and a grayscale filter is applied.
# Rotation
All mentioned objects can be rotated. Rotation works with functions:<br />
x = (cos(angle)⋅(x-origin_x)-sin(angle)⋅(y-origin_y))+origin_x<br />
y = (sin(angle)⋅(x-origin_x)+cos(angle)⋅(y-origin_y))+origin_y<br />
x and y is the position of the pixel and origin_x, and origin_y are values of x and y around which pixel should be rotated.<br />
Rotation has different pivots, so it can rotate object around its center, around its starting position, ending position, and around other point.<br />
Object can be rotated clockwise and counterclockwise.<br />
To rotate an object my engine rotates all pixels of that object around a point using mentioned formula.<br />
![2023-09-30 18-42-31 (online-video-cutter com) (1)](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/0c383c03-890a-4064-a687-feb8213a69db)
# Dynamic Windows
Also I created Dynamic Windows class which can create new windows, change their size, position, name, and options at runtime. Each window can be coded independently. It can create up to 100 windows.<br />
I used Windows.h library for implementing this.<br />
# Physics
My engine also has basic Physics simulation. It can simulate object being thrown at a specific angle, initial velocity, wind resistance, wind angle, air resistance, substance resistance, altitude, air humidity.<br />
Firstly, user must create a PhysicsComponent class and call ApplyPhysicsComponent() which requires two variables. First is the mass of the object and second is the Object itself. Then, user calls StartPhysicsSimulation function to start<br />
the simulation and after it call ApplyPhysicsTransformation().<br />
Then, user inputs all required variables and defines types of air resistance and wind resistance.<br />
There are three types of air resistance: no air resistance, default air resistance(like in our world), user-inputted air resistance(it is more like substance resistance, for example object is falling in water, oil or something else).<br />
And also there are three types of wind resistance: no wind, wind with constant velocity, accelerating or decelerating wind.<br />
Furthermore, user can set air density by calling SetAirDensityParametersHeight which requires air type(no air, dry air, humid air), temperature, altitude, specific humidity and relative humidity.<br />
This simulation works as following:<br />
We have velocity x and velocity y(Velocity for x and y coordinates).<br />
For velocity y:<br />
Here we have four forces acting on object: air resistance force, force due to gravity,  wind resistance y force, throwing object force y.<br />
The formula for air resistance is<br /> ![alt text](https://i.ibb.co/zRWDxYr/formula-ar.png)<br />
Here Fd is air resistance force. A - area of the object. V - velocity. P - is density of air(substance) and Cd is a special coefficent of the object depending on its structure, area and many other factors.<br />
If we divide the following equation by mass we get acceleration and then convert it to velocity.<br />
After that we substract given air(substance) velocity from gt(velocity due to gravity).<br />
Then we add y velocity of throwing object and wind resistance velocity.<br />
For velocity x:<br />
Here we have two forces acting on object: throwing object force x and wind resistance force x.<br />
In conclusion, we convert every x and y force to velocity. <br />
We add to y coordinate of the object the full y velocity and we add to x coordinate of the object the full x velocity.<br />
![2023-09-30 18-16-14 (online-video-cutter com) (2)](https://github.com/Tima-a/CppGraphicsEngine/assets/107584760/82158179-85f9-4052-9c71-8b6ff09eebe4)
Here, an object with a mass of 1 kg is thrown at 25 degrees, with an initial velocity of 20 m/s, and accelerating wind is acting from 90 degrees (from the right side of the screen to the left) with a wind acceleration of 4 m/s^2. The air is dry at an altitude of 10,000 meters. All these values can be modified which impact air resistance and wind resistance.
# Audio
Now any type of audio can be played, especially  .mp3 format. Audio class has many functions as to stop the current audio track, change its volume(left and right also), speed, get treble, bass, bit depth and sample frequency. <br />
Audio class works with mci library which I made much more simple to use and added more functionality.<br />
# Input
Improved Windows keyboard input and made it more simple and user-friendly with all VKL codes replaced by characters they represent. Added more functions such as if any key is pressed, key is up, pressed, released. <br />
# Camera
To create a 2D camera user must create a camera class object with current position x, position y, and render width and height. If no camera is active, a default black screen will be shown. <br />
# Buttons
User can create buttons by instantiating button class object and specifying region which button covers(x1,x2,y1,y2) and which action should be done to activate(press a specific key or mouse button). <br />
# Utility
There are dozens of handy functions in utility.cpp which are primarily designed for rendering.cpp and game.cpp. <br />
The one of the most important goals in my engine was to not use any copied code, powerful libraries like iostream, string, STL and etc. <br />
So, I designed various algorithms to convert int to const char* and vice versa, get substring of const char*, check if they are equal. This functions are  used in text class and other vital areas with advanced floor and trunc techniques and many other very useful functions. <br />




