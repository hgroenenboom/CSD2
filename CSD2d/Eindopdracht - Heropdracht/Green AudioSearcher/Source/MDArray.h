#ifndef MDARRAY_H
#define MDARRAY_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <cstdlib>
#include <ctime>

// a multidimensional array with lots of functional methods
template <class T>
class MDArray {
public:
	MDArray(int xSize, int ySize);
	MDArray(AudioBuffer<float>* b);

	~MDArray() {
		for (int i = x - 1; i >= 0; i--) {
			if (a[i] != nullptr) {
				delete[] a[i];
			}
		}
		if (a != nullptr) {
			delete[] a;
		}
	}

	const MDArray* get() {
		return this;
	}

	T** getPointers() {
		return a;
	}

	double getAverage();
	double getAbsoluteAverage();

	void clear(int x = -1, int y = -1);

	void operator *= (float val)
	{
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				a[i][j] *= val;
			}
		}
	}

	void multiply(float val, int x, int y) {
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				a[i][j] *= val;
			}
		}
	}

	T* operator [] (int index)
	{
		return a[index];
	}

	void print(int _x, int _y) {
		for (int c = 0; c < _x; c++) {
			for (int i = 0; i < _y; i++) {
				const float v = a[c][i];
				DBG( String("a[") + String(c) + String("][") + String(i) + String("] = ") + String( v) );
			}
		}
	}

	const int x;
	const int y;
private:
	T** a;
};

#endif