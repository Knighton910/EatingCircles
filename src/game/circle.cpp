#include "oxygine-framework.h"
#include <functional>
#include "circle.h"
#include <iostream>
using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations. 
//It is important on mobile devices with limited memory and you would load/unload them

static Resources gameResources;


Circle::Circle(int s, int x, int y, Color color):
    velocity(0,0),
    size(s),
    position( Vector2(x, y) )
  {
  if( !gameResources.getUseLoadCounter() )
    gameResources.loadXML("res.xml");
  setColor(color);
  setPosition(position);
  setInputEnabled(false);
  setUserData(0);
  setAnimFrame(gameResources.getResAnim("circle"));
  setScale(size/60);
  delta_size = 0;
  setAbilitiesFromColor();
}

Circle::Circle(Vector2 vect):
    velocity(0,0),
    size(rand() % 57 + 4),
    position( vect )
  {
  setColor(Color(rand()%200 + 55, rand() % 200 + 55, rand() % 200 + 55));
  setPosition(position);
  setInputEnabled(false);
  setUserData(0);
  setAnimFrame(gameResources.getResAnim("circle"));
  setScale(size/60);
  delta_size = 0;
  setAbilitiesFromColor();
}

Circle::~Circle(){
}

void Circle::accelerate(Vector2 ys, double time){
  velocity += ys;
  position = position + velocity;
  velocity.x -=  velocity.x * size / 250;
  velocity.y -= velocity.y * size / 250;
  setPosition(position);
  loseMass();
}

void Circle::move(Vector2 delt){
  position = position + delt;
  setPosition(position);
}

void Circle::update_gui(){
  setScale(size/60);
  setColor(color);
}

double Circle::getSize(){
  return size;
}

Vector2 Circle::getVelocity(){
  return velocity;
}

void Circle::eatCircle(spCircle& circle){
    color.r = (color.r * size + circle->getColor().r * circle->getSize()) 
      / (size + circle->getSize());
    color.g = (color.g * size + circle->getColor().g * circle->getSize()) 
      / (size + circle->getSize());
    color.b = (color.b * size + circle->getColor().b * circle->getSize()) 
      / (size + circle->getSize());
    size += circle->size / size;
    setAbilitiesFromColor();
    update_gui();
}


Vector2 Circle::getCenter(){
  return position + Vector2(size,size);
}

bool Circle::is_in_rect(Vector2 start, Vector2 end){
  if(position.x < start.x || position.x > end.x ||
     position.y < start.y || position.y > end.y) return false;
  else
    return true;
}

void Circle::setAbilitiesFromColor(){
  power = color.r - 55;
  agility = color.g - 55;
  mana = color.b - 55;
}

void Circle::loseMass(){
  double change_size = size * ((double)(200 - power)) / 2000000.0;
  delta_size += change_size;
  size -= change_size;
  if(delta_size > 0.5){
    delta_size = 0;
    update_gui();
  }
}
