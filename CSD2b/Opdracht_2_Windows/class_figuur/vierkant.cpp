#include "vierkant.h" 
 
Vierkant::Vierkant(int x, int y) : Figuur(x,y)
{
  std::cout << "Contructor van Vierkant" << std::endl;
}

Vierkant::~Vierkant()
{
  std::cout << "Destructor van Vierkant" << std::endl;
}

void Vierkant::teken()
{
  std::cout << "Teken Vierkant" << std::endl;
}


