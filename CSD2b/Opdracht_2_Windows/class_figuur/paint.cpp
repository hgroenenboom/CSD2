#include "figuur.h"
#include "vierkant.h"

int main()
{
  Figuur cirkel1(25,47);
  Figuur cirkel2(6,73);
  Figuur cirkel3(100,20);

  cirkel1.teken();

  Vierkant vierkant1(10,10);
  Vierkant vierkant2(100,100);

  vierkant1.teken();

  return 0;
}

