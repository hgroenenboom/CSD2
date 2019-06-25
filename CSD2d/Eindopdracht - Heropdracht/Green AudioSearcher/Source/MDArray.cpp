#include "MDArray.h"
#include "../JuceLibraryCode/JuceHeader.h"

template <class T>
MDArray<T>::MDArray(int xSize, int ySize)
	: x(xSize), y(ySize)
{
	a = new T*[xSize];
	for (int i = 0; i < xSize; i++) {
		a[i] = new T[ySize];
	}
}

template <class T>
MDArray<T>::MDArray(AudioBuffer<float>* b)
	: x( b->getNumChannels() ), y( b->getNumSamples() )
{
	a = new T*[x];
	for (int i = 0; i < x; i++) {
		a[i] = new T[y];
	}

	for (int i = 0; i < x; i++) {
		auto rp = b->getReadPointer(i);

		for (int j = 0; j < y; j++) {
			a[i][j] = rp[j];
		}
	}
}

template <class T>
double MDArray<T>::getAverage() {
	float average = 0.0f;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			average += a[i][j];
		}
	}

	average /= x*y;
	return average;
}

template <class T>
double MDArray<T>::getAbsoluteAverage() {
	float absoluteAverage = 0.0f;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			absoluteAverage += abs( a[i][j] );
		}
	}

	absoluteAverage /= x * y;
	return absoluteAverage;
}

template <class T>
void MDArray<T>::clear(int x, int y) {
	if (x == -1) {
		x = this->x;
	}
	if (y == -1) {
		y = this->y;
	}

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			a[i][j] = 0.0;
		}
	}
}

template class MDArray<int>; 
template class MDArray<double>; 
template class MDArray<float>;