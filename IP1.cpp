#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;

class AngleValidator{
public:
  static bool isValid(float angle) {
    return angle >= 0.f && angle < 90.f;
  }
};

class Roof {
private:
  static int objectCount;
  static int lastId;
  int id;
  string material;
  string color;
  float angle;

public:
  Roof(string material, string color) { 
    init(material, color, 0.f); 
  }
  Roof(string material, string color, float angle) {
    init(material, color, angle);
  }
  ~Roof() {
    --objectCount;
  }

private:
  void init(string material, string color, float angle) {
    setMaterial(material);
    this->color = color;
    setAngle(angle);
    setId();
    ++objectCount;
  }

public:
  void setMaterial(string material) {
    this->material = material;
  }
  void setColor(string color) {
    this->color = color;
  }
  void setAngle(float angle) {
    if (AngleValidator::isValid(angle))
      this->angle = angle;
    else
      throw std::invalid_argument("Exception in Roof: angle is not valid!");
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
};

int Roof::lastId = 0;
int Roof::objectCount = 0;

int main() {
  try {
    {
      Roof r0("asphalt", "black");
      Roof *r1 = new Roof("metal", "red", 15.f);

      // test1
      if (r1->getId() == 2 && 
          r1->getLastId() == 2 && 
          r1->getObjectCount() == 2 &&
          r1->getMaterial() == "metal" &&
          r1->getColor() == "red" &&
          r1->getAngle() == 15.f &&
          r1->toString() == "2 metal 15 red")
        cout << "test1 passed!" << endl;
      else
        cout << "test1 failed!" << endl;

      // test2
      r1->setMaterial("asphalt");
      r1->setColor("black");
      r1->setAngle(0.f);

      if (r1->getMaterial() == "asphalt" && 
          r1->getColor() == "black" &&
          r1->getAngle() == 0.f)
        cout << "test2 passed!" << endl;
      else
        cout << "test2 failed!" << endl;

      // test3
      try {
        r1->setAngle(-10.f);
        cout << "test3 failed!" << endl;
      } catch (std::invalid_argument const &ex) {
        if (!strcmp(ex.what(), "Exception in Roof: angle is not valid!"))
          cout << "test3 passed!" << endl;
        else
          cout << "test3 failed!" << endl;
      } catch (...) {
        cout << "test3 failed!" << endl;
      }

      // test4
      Roof *r2 = new Roof("clay", "green", 30.f);
      Roof *r3 = new Roof("plastic", "blue", 40.f);

      delete r1;

      if (r3->getId() == 4 && 
          (r3->getId() > r2->getId()) &&
          Roof::getObjectCount() == 3)
        cout << "test4 passed!" << endl;
      else
        cout << "test4 failed!" << endl;

      delete r3;
      delete r2;
    }
    // test5
    bool beginsZero, increasesWithNew, decreasesWithDelete, endsZero;

    beginsZero = (Roof::getObjectCount() == 0);

    Roof *r0 = new Roof("plastic", "blue", 40.f);
    increasesWithNew = (Roof::getObjectCount() == 1);

    Roof *r = new Roof[3]{Roof("asphalt", "black"),
                          Roof("metal", "red", 15.f),
                          Roof("clay", "green", 30.f)};
    increasesWithNew = (increasesWithNew && Roof::getObjectCount() == 4);

    delete[] r;
    decreasesWithDelete = (Roof::getObjectCount() == 1);

    delete r0;
    decreasesWithDelete = (decreasesWithDelete && Roof::getObjectCount() == 0);

    endsZero = (Roof::getObjectCount() == 0);

    if (increasesWithNew && 
        decreasesWithDelete && 
        endsZero)
      cout << "test5 passed!" << endl;
    else
      cout << "test5 failed!" << endl;
  } catch (...) {
    cout << "Unexpected error occured!" << endl;
  }

  // memory leak check
  assert(!Roof::getObjectCount());

  return 0;
}
