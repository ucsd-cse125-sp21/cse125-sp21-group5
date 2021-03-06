#pragma once
#include "Collider.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
using std::vector;

class Quadtree {
public:
    // variables of the quadtree itself 
    Collider boundary;

    // TODO: need to make this generic 
    vector<Collider*> objects;
    int capacity;
    bool hasDivided;

    // Child quadtrees 
    Quadtree* one;
    Quadtree* two;
    Quadtree* three;
    Quadtree* four;
    Quadtree* five;
    Quadtree* six;
    Quadtree* seven;
    Quadtree* eight;

    // Constructor for the quadtree/map 
    Quadtree(Collider boundary, int capacity);

    //destructors & helpers 
    //~Quadtree3D();
    //void destructorHelper(Quadtree3D * qt);

    void insert(Collider* p);
    void subdivide();
    // Can query a box? 
    vector<Collider*> query(Collider* range, vector<Collider*>& found);
};