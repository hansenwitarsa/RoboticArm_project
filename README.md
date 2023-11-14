# RoboticArm_project
A project involving controlling a 6 DOF robotic arm (EPSON C3)
This project was done by writing code in C++, and using a specific software to control the robot arm.
In front of the robotic arm, lies a 3x4 grid with either red/green square. On the top left square there is an object. The robotic arm goal is to pick the object and put it to the bottom right square. However, the robotic arm endpoint cannot move through the red squares.

The flow of the project:
- A web camera is put beside the robot to capture the grid.
- Using OpenCV and Python, it recognizes red and green colour.
- Since the grid is 3x4, a 3x4 matrix will be produced by inputting 1 for red colour and 0 for green colour.
- Then, I developed an A* path planning algorithm in C++ (script folder). The input is 3x4 matrix with binary numbers. And the output is the path containing coordinates from (0,0) to (2,3). (I consider the starting square as (0,0) and the goal square as (2,3)).
- The path coordinates are then inputted to another code in C++ inside the software and the output is the real world coordinates where the robotic arm endpoint has to move. (Unfortunately, I cannot recover the codes inside the software).
- There is a gripper attached to the endpoint that can be controlled to open or close to grab the object. (When practicing the project, the gripper worked, but at demonstration time, the gripper has mechanical problem, so it could not move).

A summary of the project can be found in the preprint folder.
Demonstration can be found in the presentation folder.
