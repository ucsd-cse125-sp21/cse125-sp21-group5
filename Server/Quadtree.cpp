#include "Quadtree.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;

Quadtree::Quadtree(Collider boundary, int capacity) {
    this->boundary = boundary;
    this->capacity = capacity;
    this->objects = vector<Collider*>();
    hasDivided = false;
}


// Inserts a new Collider into the tree 
void Quadtree::insert(Collider* p) {
    // check if the quadtree can house the point, if it can't, don't do anything
    //TODO subclass/sueprclass 
    if (!this->boundary.contains(p)) {
        return;
    }
    // if capacity of the quadtree is not full
    if (this->objects.size() < this->capacity) {
        //std::cout << "Regular insertion" << std::endl;
        this->objects.push_back(p);
        //cout << "objects size at " << this->objects.size() << endl;
    }
    // if it is full
    else {
       // std::cout << "SHOULD NOT COME HERE " << std::endl;
        // check if it has been divided before 
        if (!this->hasDivided) {
            //cout << "subdividing" << endl;
            subdivide();
            this->hasDivided = true;
        }
        //recurisvely call insert in order to put the points in the sub-quadtrees
        this->one->insert(p);
        this->two->insert(p);
        this->three->insert(p);
        this->four->insert(p);
        this->five->insert(p);
        this->six->insert(p);
        this->seven->insert(p);
        this->eight->insert(p);
    }
}

// function that subdivides the current quadtree/map into smaller subquadtrees of
// four equal sizes 
void Quadtree::subdivide() {
    glm::vec3 c = this->boundary.cen;
    glm::vec3 d = this->boundary.dim / 4.0f;

    //cout << "new dim is " << glm::to_string(d) << endl;
    /*
   +--------+
  /   4  2 /|
 /  8  6  / |
+--------+  |
|        |  |
|    3  1|  +
|        | /
|  7  5  |/
+--------+
    */
    Collider one = Collider(glm::vec3(c.x - d.x, c.y - d.y, c.z - d.z), d*2.0f);
    this->one = new Quadtree(one, capacity);

    Collider two = Collider(glm::vec3(c.x - d.x, c.y - d.y, c.z + d.z), d * 2.0f);
    this->two = new Quadtree(two, capacity);

    Collider three = Collider(glm::vec3(c.x - d.x, c.y + d.y, c.z - d.z), d * 2.0f);
    this->three = new Quadtree(three, capacity);

    Collider four = Collider(glm::vec3(c.x - d.x, c.y + d.y, c.z + d.z), d * 2.0f);
    this->four = new Quadtree(four, capacity);

    Collider five = Collider(glm::vec3(c.x + d.x, c.y - d.y, c.z - d.z), d * 2.0f);
    this->five = new Quadtree(five, capacity);

    Collider six = Collider(glm::vec3(c.x + d.x, c.y - d.y, c.z + d.z), d * 2.0f);
    this->six = new Quadtree(six, capacity);

    Collider seven = Collider(glm::vec3(c.x + d.x, c.y + d.y, c.z - d.z), d * 2.0f);
    this->seven = new Quadtree(seven, capacity);

    Collider eight = Collider(glm::vec3(c.x + d.x, c.y + d.y, c.z + d.z), d * 2.0f);
    this->eight = new Quadtree(eight, capacity);
}


// TODO: need to make this generic and not just use BoxCollider 
vector<Collider*> Quadtree::query(Collider* range, vector<Collider*>& found) {
    if (found.empty()) {

    }
    // if the range is not within the boundary, just return 
    if (this->boundary.check_collision(range) == glm::vec3(0.0f)) {
        return found;
    }
    // if they do intersect
    else {
        // look for all the points in this quadtree 
        for (auto& point : this->objects) {
            //std::cout << "lwh of range being queried is " << range->length << " " <<
                //range->width << " " << range->height << std::endl;
            // checks if the point is within the boundary of the range
            // we're looking for 
            
            // TODO: check for superclass?
            if (range->check_collision(point) != glm::vec3(0.0f)) {
                found.push_back(point);
            }
        }
        // checks if the quadtree has any child quadtrees 
        if (this->hasDivided) {
            this->one->query(range, found);
            this->two->query(range, found);
            this->three->query(range, found);
            this->four->query(range, found);
            this->five->query(range, found);
            this->six->query(range, found);
            this->seven->query(range, found);
            this->eight->query(range, found);
        }
    }
    return found;
}

// Quadtree3D::~Quadtree3D(){
//     destructorHelper(this);
// }

// void Quadtree3D::destructorHelper(Quadtree3D * qt){
//     // if it has no children, just delete itself
//     if(!this->topLeft && !this->topRight 
//         && !this->bottomLeft && this->bottomRight){
//             delete this;
//     }
//     // deletes everything else recurisvely 
//     if(this->topLeft){
//         destructorHelper(this->topLeft);
//     }
//     if(this->topRight){
//         destructorHelper(this->topRight);
//     }
//     if(this->bottomLeft){
//         destructorHelper(this->bottomLeft);
//     }
//     if(this->bottomRight){
//         destructorHelper(this->bottomRight);
//     }

//     // deletes itself after it has deleted its children
//     delete this;
// }