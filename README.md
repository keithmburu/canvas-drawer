# canvas-drawer

Implements a simple drawing api

![](images/art/art-3.png)\

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
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

1. Point\
![](images/test/point.png)\
2. Line\
![](images/test/line-color-interpolation.png)\
3. Triangle\
![](images/test/triangle.png)\

## Composite shapes

1. Circle\
![](images/test/circle.png)\
2. Rectangle\
![](images/test/rectangle.png)\
3. Star\
![](images/test/star.png)\
4. Snowflake\
![](images/test/snowflake-4.png)\
5. Rose\
![](images/test/rose.png)\

## Results

![](images/art/art-1.png)\
![](images/art/art-2.png)\
![](images/art/art-4.png)\
![](images/art/art-5.png)\
![](images/art/art-6.png)\
![](images/art/art-7.png)\

