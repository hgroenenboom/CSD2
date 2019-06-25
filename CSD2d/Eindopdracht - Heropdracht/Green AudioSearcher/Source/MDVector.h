/*
  ==============================================================================

    MDVector.h
    Created: 22 Mar 2019 4:16:19pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

// a multidimensional vector with lots of functional methods
template <class T>
class MDVector {
public:
	MDVector(int xSize = 2, int ySize = 2)
		: v(xSize, std::vector<T>(ySize)), x(xSize), y(ySize)
	{
	}

	const MDVector* get() {
		return &v;
	}

	MDVector<T> getCopy(int xDim, int yDim) {
		jassert(xDim <= x && yDim <= y);

		MDVector<T> c(xDim, yDim);

		for (int i = 0; i < xDim; i++) {
			for (int j = 0; j < yDim; j++) {
				c[i][j] = v[i][j];
			}
		}

		return c;
	}

	void clear(int _x = -1, int _y = -1) {
		if (_x == -1) {
			_x = this->x;
		}
		if (_y == -1) {
			_y = this->y;
		}

		for (int i = 0; i < _x; i++) {
			for (int j = 0; j < _y; j++) {
				v[i][j] = 0.0;
			}
		}
	}

	void resize(int x, int y) {
		if (v.size() < x) {
			v.resize(x);
		}
		for (int i = 0; i < x; i++) {
			if (v[i].size() < y) {
				v[i].resize(y);
			}
		}
		this->x = x;
		this->y = y;
	}

	void absolute() {
		absolute(x, y);
	}

	void absolute(int _x, int _y) {
		for (int i = 0; i < _x; i++) {
			for (int j = 0; j < _y; j++) {
				v[i][j] = abs(v[i][j]);
			}
		}
	}

	void multiply(float val, int _x, int _y)
	{
		for (int i = 0; i < _x; i++) {
			for (int j = 0; j < _y; j++) {
				v[i][j] *= val;
			}
		}
	}

	void doubleMultiply(MDVector& v2, float val)
	{
		const int x2 = v2.x;
		const int y2 = v2.y; 
		const int xIt = x2 > x ? x : x2;
		const int yIt = y2 > y ? y : y2;

		for (int i = 0; i < xIt; i++) {
			for (int j = 0; j < yIt; j++) {
				v[i][j] *= v2[i][j];
				v[i][j] *= val;
			}
		}
	}

	void doubleMultiply(std::vector<T>& v2, float val)
	{
		const int y2 = v2.size();
		const int xIt = x;
		const int yIt = y2 > y ? y : y2;

		for (int i = 0; i < xIt; i++) {
			for (int j = 0; j < yIt; j++) {
				v[i][j] *= v2[j];
				v[i][j] *= val;
			}
		}
	}

	MDVector* operator *= (float val)
	{
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				v[i][j] *= val;
			}
		}

		return this;
	}

	void operator -= (MDVector& md)
	{
		const int x2 = md.x;
		const int y2 = md.y;
		jassert(x2 <= x && y2 <= y);

		for (int i = 0; i < x2; i++) {
			for (int j = 0; j < y2; j++) {
				v[i][j] -= md[i][j];
			}
		}
	}

	void operator += (MDVector& md)
	{
		const int x2 = md.x;
		const int y2 = md.y;
		//jassert(x2 <= x && y2 <= y);

		const int xIt = x2 > x ? x : x2;
		const int yIt = y2 > y ? y : y2;

		for (int i = 0; i < xIt; i++) {
			for (int j = 0; j < yIt; j++) {
				v[i][j] += md[i][j];
			}
		}
	}

	void operator += (std::vector<T>& md)
	{
		const int y2 = md.size();
		const int xIt = x;
		const int yIt = y2 > y ? y : y2;

		for (int i = 0; i < xIt; i++) {
			for (int j = 0; j < yIt; j++) {
				v[i][j] += md[j];
			}
		}
	}

	void add(float val, int pos, int numChannels = -1)
	{
		const int xIt = numChannels <= 0 ? x : numChannels;
		for (int i = 0; i < xIt; i++) {
			v[i][pos] += val;
		}
	}

	std::vector<T>& operator [] (int index)
	{
		return v[index];
	}

	std::vector<T>& getRow(int r) {
		jassert(r >= 0 && r < x);
		return v[r];
	}

#if JUCE_ENABLED
	void addToAudioBuffer(AudioBuffer<float>& b) {
		for (int c = 0; c < b.getNumChannels(); c++) {
			auto wp = b.getWritePointer(c);
			for (int i = 0; i < b.getNumSamples(); i++) {
				wp[i] += v[c][i];
			}
		}
	}

	void fromAudioBuffer(AudioBuffer<float>& b) {
		for (int c = 0; c < b.getNumChannels(); c++) {
			auto rp = b.getReadPointer(c);
			for (int i = 0; i < b.getNumSamples(); i++) {
				v[c][i] = rp[i];
			}
		}
	}

	int getNumChannels() {
		return x;
	}

	int getNumSamples() {
		return y;
	}

	const std::vector<float>& getReadPointer(int chan) {
		return v[chan];
	}

	std::vector<float>& getWritePointer(int chan) {
		return v[chan];
	}
#endif

	int x;
	int y;
private:
	std::vector< std::vector<T> > v;
};