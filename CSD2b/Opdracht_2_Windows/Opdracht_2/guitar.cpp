#include "guitar.h"

Guitar::Guitar(string newType, string inputSound){
	setLowestNote(40);
	setNoteRange(40);
	setType(newType);
	setSound(inputSound);
	setName("guitar"); //verwijs naar eigen naam?, print naam van object?
	
	printInfo();

}