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
    //BoxCollider boundary;

    //// TODO: need to make this generic 
    //vector<BoxCollider*> objects;
    //int capacity;
    //bool hasDivided;

    //// Child quadtrees 
    //Quadtree* one;
    //Quadtree* two;
    //Quadtree* three;
    //Quadtree* four;
    //Quadtree* five;
    //Quadtree* six;
    //Quadtree* seven;
    //Quadtree* eight;

    //// Constructor for the quadtree/map 
    //Quadtree(BoxCollider boundary, int capacity, vector<BoxCollider*> objects) {
    //    this->boundary = boundary;
    //    this->capacity = capacity;
    //    this->objects = objects;
    //    hasDivided = false;
    //}

    ////destructors & helpers 
    ////~Quadtree3D();
    ////void destructorHelper(Quadtree3D * qt);

    //void insert(BoxCollider* p);
    //void insert(SphereCollider* p);
    //void insert(Collider* p);
    //void subdivide();
    //// Can query a box? 
    //vector<BoxCollider*> query(BoxCollider* range, vector<BoxCollider*>& found);
};