#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <fileapi.h>
#include <sstream>
#include "tinyXML/include/tinyxml2.h"

class label
{
public:

	bool findXmlData(std::string, std::string, int&);
	bool findXmlData(std::string, std::string, std::string&);

	bool importAllSenses(std::string);

	bool saveNames(std::string memoryPath);

	int videoName;
	int audioName;
	int imageName;

	int videoAlgorithmName;
	int audioAlgorithmName;
	int imageAlgorithmName;

	int videoProcessName;
	int audioProcessName;
	int imageProcessName;

	int algorithmListName[100];

	//Each integer represents the filename to be processed
	int videoProcQueue[100];

private:
	std::string xmlObjectPath = "\\object_storage\\filename.xml";

	std::string videoPath = "\\object_storage\\video";
	std::string audioPath = "\\object_storage\\audio";
	std::string imagePath = "\\object_storage\\image";

	std::string xmlAlgorithmPath = "\\algorithm_storage\\filename.xml";

	std::string videoAlgorithmPath = "\\algorithm_storage\\video";
	std::string audioAlgorithmPath = "\\algorithm_storage\\audio";
	std::string imageAlgorithmPath = "\\algorithm_storage\\image";

	std::string xmlAlgorithmList = "\\algorithm_storage\\video\\algorithmList.xml";

};