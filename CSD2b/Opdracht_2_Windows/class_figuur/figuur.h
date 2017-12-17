#ifndef _FIGUUR_H_
#define _FIGUUR_H_

#include <iostream>

class Figuur
{
public:
  Figuur(int x, int y);
  ~Figuur();

  void teken();
  void verplaats();
  void set_snelheid(int snelheid);

private:
  int x,y;
  int snelheid;
};

#endif // _FIGUUR_H_
