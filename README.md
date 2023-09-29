# C++ Graphics Engine
C++ Graphics Engine is a graphics engine written by Teymur Aghayev on C++ for building desktop applications, and games on C++ which is still in progress.
My engine can draw simple shapes like rectangles, triangles, ellipses, vectors. Also it can draw 2D Text and import images as sprites.
It also has basic Physics simulations.
I started writing this engine in summer of 2019.
You can install C++ Graphics engine just by downloading the project and the in Visual Studio open an existing project and select C++ Graphics Engine folder.
# Features
Firstly, user must create a WINDOW class which creates the window with user-inputted x,y,width,height.
The user has simulategame() function which is a main loop function where all user code is written. In the beggining of main loop user must update the window with the refresh_screen(color, window) function.
Every graphics unit should be called as object and after the object is created user should write object_name.draw() and object will be drawn.
Then, vector, and ellipse shapes have two classes of fvector, dvector and ellipsef, ellipsed classes respectively. In dvector and ellipsed classes when shapes are drawn, user can access some variables which are really important.
For example, in dvector and ellipsed classes there is an array of x and y positions of all pixels that shape contains. Those classes are oftenly used in other classes like in triangles, rectangles and 2D Text.
On the other side, fvectors and ellipsef classes are faster because they don't save such information.
# Rectangles
Actually the first shape I have done is rectangle because of its simplicity.
How does my engine draw rectangles?
It creates a line of width user inputted and draws that line from upper height to a lower one.
# Vectors
Was really difficult for me to write, but i still managed to do it.
How does my engine draw vectors?
Firstly, it check what type of vector it is(straight or not).
If vector is not straight then it calculates the slope of a vector by formula m=(y2-y1)/(x2-x1). Then, it calculates the direction to which the resulting vector should be facing by the (x2-x1) and (y2-y1).
In loop where vector is being written there are x and y cycles. In x_cycle engine draws only pixels which differ in x, in y_cycle engine draws pixels which differ only in y.
If in for-loop where there is i as a loop variable then if i divided by 2 yields 1 then it is x_cycle/y_cycle and if i divided by 2 yields 0 then it is again x_cycle/y_cycle. Which cycle is first depends on |x2-x1| and |y2-y1|.
After that, if |x2-x1| > |y2-y1| then it stores slope to variable slope_x, and x_y_outweight equals false and if |x2-x1| < |y2-y1| then to slope_y and x_y_outweight equals true. slope_x and slope_y are initially 1.0f
And the basic principle is to check if in x_cycle x_y_outweight is false, then program starts to draw initially with x_cycle and it draws amount of pixels equivalent to slope_x.
Then, in y_cycle vector if x_y_outweight is false program draws only one pixel and then goes again to x_cycle.
If |x2-x1| < |y2-y1| and x_y_outweight is true, then it does everything oppositely where in x_cycle engine draws only one pixel and in y_cycle it draws slope_y amount of pixels.
# Triangles
To create triangle, user must input the length of first side(a), the length of second side(b), the length of third side(c), height of the triangle, and  x, y positions of triangle.
C is casually the line in the bottom of default triangle which connects a and b.
Firstly it visualizes the triangle by drawing a,b,c depending on which is side is bigger.
Then, it is filled from the point where a and b intersect (let's call it D) by creating vectors from D to every pixel of vector C.
# Ellipses
Also a difficult one, so the main method of creating ellipses is by using sin(t)*radius_width and cos(t)*radius_height to get position of all pixels the ellipse must be built of.
t - is the variable which is initially 360 and is decreasing by 1 every time in loop. (converted to radians before using in sin and cos functions)
Ellipses also can be drawn not fully, but partially. For example, only half of it, or from 90 to 270 degrees. User can also define this information(start_circum, end_circum) to draw ellipse partially.
start_circum 0 means the top center pixel which is drawn first.
To fill ellipse, engine connects every pixel of right side of ellipse with the central vertical line of the ellipse and then from right side of ellipse with the central vertical line.
Why not just instantly from left side to right side?
Because ellipses can be drawn partially, for example ellipse with start_circum of 0 and end_circum of 220 will not work with this approach. So it firsly fills all the left side of ellipse with central vertical line and then the right side with central vertical line.
# 2D Text
Actually, the 2D Text object is created by dividing the user-inputted text to chars(letters) and drawing them separately. Every letter is drawn uniquely. For example, A has one vector facing top-right, one straight vector between them, and a vector facing down-right.
This is done with every character. Also numbers and doubles can be inputted as text.
# Images, Sprites
Here I used stb_image library. It only gets every pixels' color of image in rgb format. Then, my engine draws those pixels as a rectangle and creates an image.
Then if user wants to resize and move an image he creates a Sprite class. Resizing works by increasing or decreasing size of the pixels.
Sprite class also has many filters like red, green, blue and grayscale.
# Rotation
All mentioned objects can be rotated by function
x = (cos(angle)*(x-origin_x)-sin(angle)*(y-origin_y))+origin_x
y = (sin(angle)*(x-origin_x)+cos(angle)*(y-origin_y))+origin_y
origin_x, and origin_y are values of x and y which should be rotated by an angle around 




