# [Final Project](https://github.com/OrenZfira/Graphics_Course)
## Oren Zfira and Sheer Maoz

The Project consists of the following features:
1. Shapes
    - Adding Shapes
    - Translation and Rotation
    - Changing Shape Material
    - Transparency 
    - Scaling (*Bonus*)
    - Reset Location/Rotation (*Bonus*)
2. Layers
    - Adding Layers
    - Adding Shapes/Cameras to Layers
    - Hiding/Showing Layers
3. Cameras
    - Adding Cameras (as 3D shapes *Bonus*)
    - Moving Cameras
    - Switching Cameras
    - Setting Animation Camera
    - Orthographic/Prespective view
    - Reset Location (*Bonus*)
4. Animation
    - During Animation only shapes move according to the Bezier Curve on right side of screen
    - Play/Pause
    - Animation Duration Change
    - Switch to Animation Camera
5. Picking
    - Single Picking
        - Rotation in Left Side of Screen, Translation in Right Side of Screen
        - All Menu Operations
        - Scrolling - Moves on Z axis
        - Zoom on Object
    - Multi Picking
        - Right Side - Nothing
        - Translation
        - All Menu Operations Except Reset Shape
    - Highlight Picked Objects
6. Other
    - Change Background
    - Changeable Bezier Curve
    - Fog Turn On/Off (*Bonus*)
    - Change Fog Colour (*Bonus*)

Controls:
1. Mouse
    - Left button - Single Pick
        - Long Press to rotate in the left screen
        - Long Press to move points in right screen
    - Right Mouse - Multi Pick
        - Long Press to select, picked after release
        - When Picked, Move mouse to translate
    - Scroll
        - When picked - Translation along Z axis
        - Zoom camera
2. Keyboard
    - Up/Down/Left/Right - Rotate Current Camera
    - F/B/U/D/L/R - Move Current Camera
    - Z - Zoom on Picked Object
