/*
  ==============================================================================

    AFParameterSet.h
    Created: 30 Jun 2019 1:26:54pm
    Author:  HAROL

  ==============================================================================
*/

// TODO 
// - HOEZO NIET JSON!!!!

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../JuceLibraryCode/JuceHeader.h"

class AFFeatureSet {
public:
	AFFeatureSet(std::string path, std::string version, std::string* params, float* val, int size) {
		setData(path, version, params, val, size);
		toString();
	}	

	AFFeatureSet(std::string data) {
		fromString(data);
	}

	std::string filePath;
	std::string versionID;

	std::vector<std::string> parameters;
	std::vector<float> values;
	int numValues;

	void fromString(std::string data) {		
		//const std::string delimiter = "|";
		//size_t pos = 0;
		//std::vector<std::string> subStrings;
		//while ((pos = data.find(delimiter)) != std::string::npos) {
		//	subStrings.push_back( data.substr(0, pos) );
		//	data.erase(0, pos + delimiter.length());
		//}
		//subStrings.push_back( data );
	}

	void setData(std::string path, std::string version, std::string* params, float* val, int size) {
		filePath = path;
		versionID = version;

		numValues = size;
		parameters.resize(size);
		values.resize(size);

		for (int i = 0; i < size; i++) {
			parameters[i] = params[i];
			values[i] = val[i];
		}
	}

	std::string toString() {
		std::string data = "?";
		data.append(filePath);
		data.append("|");
		data.append(versionID);
		data.append("|");
		data.append( arrToString(parameters) );
		data.append("|");
		data.append( arrToString(values) );
		return data;
	}
private:
	std::string arrToString(std::vector<float>& arr) {
		std::string sArr = "";
		for (int i = 0; i < arr.size(); i++) {
			sArr.append( std::to_string(arr[i]) );
			if (i != arr.size()- 1) {
				sArr.append(",");
			}
		}
		return(sArr);
	}

	std::string arrToString(std::vector<std::string>& arr) {
		std::string sArr = "";
		for (int i = 0; i < arr.size(); i++) {
			sArr.append( arr[i] );
			if (i != arr.size() - 1) {
				sArr.append(",");
			}
		}
		return(sArr);
	}
};