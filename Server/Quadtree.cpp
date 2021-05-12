#include "Quadtree.h"
#include <iostream>
using namespace std;

// Inserts a new SphereCollider into the tree 
// Non-Functional
/*void Quadtree::insert(SphereCollider* p) {
    // check if the quadtree can house the point, if it can't, don't do anything
    if (!this->boundary.contains(p)) {
        return;
    }
    // if capacity of the quadtree is not full
    if (this->objects.size() < this->capacity) {
        this->objects.push_back(p);
    }
    // if it is full
    else {
        // check if it has been divided before 
        if (!this->hasDivided) {
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
}*/

// Inserts a new BoxCollider into the tree 
void Quadtree::insert(BoxCollider* p) {
    // check if the quadtree can house the point, if it can't, don't do anything
    //TODO subclass/sueprclass 
    if (!this->boundary.contains(p)) {
        return;
    }
    // if capacity of the quadtree is not full
    if (this->objects.size() < this->capacity) {
        //std::cout << "Regular insertion" << std::endl;
        this->objects.push_back(p);
        //std::cout << "lwh of box being inserted at " << p->length << " " <<
           // p->width << " " << p->height << std::endl;
    }
    // if it is full
    else {
       // std::cout << "SHOULD NOT COME HERE " << std::endl;
        // check if it has been divided before 
        if (!this->hasDivided) {
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
    glm::vec3 c = this->boundary.center;
    BoxCollider b = this->boundary;
    vector<BoxCollider*> p1, p2, p3, p4, p5, p6, p7, p8;
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
    BoxCollider one = BoxCollider(glm::vec3(c.x - b.length / 2, c.y - b.width / 2, c.z - b.height / 2), 
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->one = new Quadtree(one, capacity, p1);

    BoxCollider two = BoxCollider(glm::vec3(c.x - b.length / 2, c.y - b.width / 2, c.z + b.height / 2),
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->two = new Quadtree(two, capacity, p2);

    BoxCollider three = BoxCollider(glm::vec3(c.x - b.length / 2, c.y + b.width / 2, c.z - b.height / 2),
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->three = new Quadtree(three, capacity, p3);

    BoxCollider four = BoxCollider(glm::vec3(c.x - b.length / 2, c.y + b.width / 2, c.z + b.height / 2),
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->four = new Quadtree(four, capacity, p4);

    BoxCollider five = BoxCollider(glm::vec3(c.x + b.length / 2, c.y - b.width / 2, c.z - b.height / 2),
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->five = new Quadtree(five, capacity, p5);

    BoxCollider six = BoxCollider(glm::vec3(c.x + b.length / 2, c.y - b.width / 2, c.z + b.height / 2),
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->six = new Quadtree(six, capacity, p6);

    BoxCollider seven = BoxCollider(glm::vec3(c.x + b.length / 2, c.y + b.width / 2, c.z - b.height / 2),
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->seven = new Quadtree(seven, capacity, p7);

    BoxCollider eight = BoxCollider(glm::vec3(c.x + b.length / 2, c.y + b.width / 2, c.z + b.height / 2),
        glm::vec3(b.length / 2, b.width / 2, b.height / 2));
    this->eight = new Quadtree(eight, capacity, p8);
}


// TODO: need to make this generic and not just use BoxCollider 
vector<BoxCollider*> Quadtree::query(BoxCollider* range, vector<BoxCollider*>& found) {
    if (found.empty()) {

    }
    // if the range is not within the boundary, just return 
    glm::vec3 isIntersect = this->boundary.intersects(range);
    if (isIntersect.x == 0.0f && isIntersect.y == 0.0f && isIntersect.z == 0.0f)
    {
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
            if (range->contains(point)) {
                found.push_back(point);
            }
        }
        // checks if the quadtree has any child quadtrees 
        if (this->hasDivided) {
            std::cout << "should not print" << std::endl;
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