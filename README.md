Molecule-Viewer
===============

Thesis Modern OpenGL Molecule Viewer

This project is my bachelor's thesis, I didn't thought the code was going to be publicly available so it's very messy code but I'll try to describe some of the structure

Overview
--------
The project aims to be a 3D Molecule renderer using some of the newer features of OpenGL and taking advantage of them by using data structures and programming logic. Since it's mainly and molecule renderer it doesn't really display other kinds of things.

There's an attempt to modularize the code to make it a more general porpouse renderer by cleaning up the code base (using C++11 and some third-party libraries), removing the molecule specific parts of the project, adding some new features(shadows, and antialiasing). You can find my new project [here](https://github.com/fernandomv3/post3D).


Branches
--------

 * The master branch is where the final project was stored
 * The moving-objects branch is a test branch where tessellation shaders where implemented
 * The multidraw branch was a test branch where multidraw OpenGL commands where used
 * The testing branch was mainly my playground for testing anything
