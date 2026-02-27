#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;

class roof {
private:
  static int objectCount;
  static int lastId;
  int id;
  string material;
  float angle;
  string color;

public:
  roof(string material, string color) { 
    init(material, 0.f, color); 
  }
  roof(string material, float angle, string color) {
    init(material, angle, color);
  }
  ~roof() {
    --objectCount;
  }

private:
  void init(string material, float angle, string color) {
    setMaterial(material);
    this->angle = angle;
    setColor(color);
    setId();
    ++objectCount;
  }

public:
  void setMaterial(string material) {
    this->material = material;
  }
  void setAngle(float angle) {
    if (angleValid(angle))
      this->angle = angle;
    else
      throw std::invalid_argument("Exception in roof: angle is not valid!");
  }
  void setColor(string color) {
    this->color = color;
  }

private:
  void setId() {
    ++lastId;
    id = getLastId();
  }

public:
  int getId() {
    return id;
  }
  static int getLastId() {
    return lastId;
  }
  static int getObjectCount() {
    return objectCount;
  }
  string getMaterial() {
    return material;
  }
  float getAngle() {
    return angle;
  }
  string getColor() {
    return color;
  }

public:
  string toString() {
    std::stringstream ss;
    ss << getId() << " " << getMaterial() << " " << getAngle() << " "
       << getColor();
    return ss.str();
  }

public:
  static bool angleValid(float angle) {
    return angle >= 0.f && angle < 90.f;
  }
};
int roof::lastId = 0;
int roof::objectCount = 0;

int main() {
  try {
    roof r0("asphalt", "black");
    roof *r1 = new roof("metal", 15.f, "red");

    // test1
    if (r1->getId() == 2 && 
        r1->getLastId() == 2 && 
        r1->getObjectCount() == 2 &&
        r1->getMaterial() == "metal" &&
        r1->getAngle() == 15.f &&
        r1->getColor() == "red" &&
        r1->toString() == "2 metal 15 red")
      cout << "test1 passed!" << endl;
    else
      cout << "test1 failed!" << endl;

    // test2
    r1->setMaterial("asphalt");
    r1->setAngle(0.f);
    r1->setColor("black");

    if (r1->getMaterial() == "asphalt" && 
        r1->getAngle() == 0.f &&
        r1->getColor() == "black")
      cout << "test2 passed!" << endl;
    else
      cout << "test2 failed!" << endl;

    // test3
    try {
      r1->setAngle(-10.f);
      cout << "test3 failed!" << endl;
    } catch (std::invalid_argument const &ex) {
      if (!strcmp(ex.what(), "Exception in roof: angle is not valid!"))
        cout << "test3 passed!" << endl;
      else
        cout << "test3 failed!" << endl;
    } catch (...) {
      cout << "test3 failed!" << endl;
    }

    // test4
    roof *r2 = new roof("clay", 30.f, "green");
    roof *r3 = new roof("plastic", 40.f, "blue");

    delete r1;

    if (r3->getId() == 4 && 
        (r3->getId() > r2->getId()) &&
        roof::getObjectCount() == 3)
      cout << "test4 passed!" << endl;
    else
      cout << "test4 failed!" << endl;

    delete r3;
    delete r2;
  } catch (...) {
    cout << "Unexpected error occured!" << endl;
  }

  try {
    // test5
    bool beginsZero, increasesWithNew, decreasesWithDelete, endsZero;

    beginsZero = (roof::getObjectCount() == 0);

    roof *r3 = new roof("plastic", 40.f, "blue");
    increasesWithNew = (roof::getObjectCount() == 1);

    roof *r = new roof[3]{roof("asphalt", "black"),
                          roof("metal", 15.f, "red"),
                          roof("clay", 30.f, "green")};
    increasesWithNew = (increasesWithNew && roof::getObjectCount() == 4);

    delete[] r;
    decreasesWithDelete = (roof::getObjectCount() == 1);

    delete r3;
    decreasesWithDelete = decreasesWithDelete && (roof::getObjectCount() == 0);

    endsZero = (roof::getObjectCount() == 0);

    if (beginsZero && increasesWithNew && decreasesWithDelete && endsZero)
      cout << "test5 passed!" << endl;
    else
      cout << "test5 failed!" << endl;
  } catch (...) {
    cout << "Unexpected error occured!" << endl;
  }

  return 0;
}