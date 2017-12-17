#include "marimba.h"

Marimba::Marimba(string newType, string inputSound){
	setLowestNote(33);
	setNoteRange(51);
	setType(newType);
	setSound(inputSound);
	setName("marimba"); //verwijs naar eigen naam?, print naam van object?
	
	printInfo();
}