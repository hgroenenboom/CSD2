#include <iostream>
#include "figuur.h"

Figuur::Figuur(int x, int y)
{
  std::cout << "Constructor van Figuur" << std::endl;
  this->x = x;
  this->y = y;
}

Figuur::~Figuur()
{
  std::cout << "Destructor van Figuur" << std::endl;
}

void Figuur::teken()
{
  std::cout << "Teken Figuur op positie " << 
    x << "," << y << std::endl;
}

void Figuur::verplaats()
{
  std::cout << "Verplaats Figuur" << std::endl;
}

void Figuur::set_snelheid(int snelheid)
{
  this->snelheid = snelheid;
}


