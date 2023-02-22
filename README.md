# canvas-drawer

Implements a simple drawing api

<img src="images/art/art-3.png" width="640">

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

## Supported primitives

1. Point
<img src="images/test/point.png" width="100">
2. Line
<img src="images/test/line-color-interpolation.png" width="100">
3. Triangle
<img src="images/test/triangle.png" width="100">

## Composite shapes

1. Circle
<img src="images/test/circle.png" width="100">
2. Rectangle
<img src="images/test/rectangle.png" width="100">
3. Star
<img src="images/test/star.png" width="100">
4. Rose
<img src="images/test/rose.png" width="100">
5. Snowflake
<img src="images/test/snowflake-5.png" width="100">

## Features

1. Outlined shapes
<img src="images/test/outlined-triangle.png" width="100">
<img src="images/test/outlined-circle.png" width="100">
<img src="images/test/outlined-rectangle.png" width="100">
<img src="images/test/outlined-star.png" width="100">
<img src="images/test/outlined-snowflake.png" width="100">
2. Blend modes - Replace, add, difference, or average
<img src="images/test/blend-modes.png" width="100">
3. Variable line width
<img src="images/test/line-width.png" width="100">

## Results

<img src="images/art/art-1.png" width="640">
<img src="images/art/art-2.png" width="640">
<img src="images/art/art-4.png" width="640">
<img src="images/art/art-5.png" width="640">
<img src="images/art/art-6.png" width="640">
<img src="images/art/art-7.png" width="640">

