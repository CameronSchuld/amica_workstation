#include "label.h"

//Calls all functions to import filenames 
bool label::importAllSenses(std::string memoryPath)
{
	std::string blank;

	std::string xmlPath[3];
	xmlPath[0] = memoryPath + "\\object_storage\\filename.xml";
	xmlPath[1] = memoryPath + "\\algorithm_storage\\filename.xml";
	xmlPath[2] = memoryPath + "\\algorithm_storage\\video\\filename.xml";

	std::stringstream convert;

	int integerValue = 0;
	std::string stringValue = "";
	std::string fileMod = "";

	if (!findXmlData(memoryPath + xmlObjectPath, "AUDIO", integerValue))
		return false;
	else
		audioName = integerValue;

	if (!findXmlData(memoryPath + xmlObjectPath, "VIDEO", integerValue))
		return false;
	else
		videoName = integerValue;

	if (!findXmlData(memoryPath + xmlObjectPath, "IMAGE", integerValue))
		return false;
	else
		imageName = integerValue;


	if (!findXmlData(memoryPath + xmlObjectPath, "AUDIO_PROCESS", integerValue))
		return false;
	else
		audioProcessName = integerValue;

	if (!findXmlData(memoryPath + xmlObjectPath, "VIDEO_PROCESS", integerValue))
		return false;
	else
		videoProcessName = integerValue;

	if (!findXmlData(memoryPath + xmlObjectPath, "IMAGE_PROCESS", integerValue))
		return false;
	else
		imageProcessName = integerValue;


	if (!findXmlData(memoryPath + xmlAlgorithmPath, "AUDIO", integerValue))
		return false;
	else
		audioAlgorithmName = integerValue;

	if (!findXmlData(memoryPath + xmlAlgorithmPath, "VIDEO", integerValue))
		return false;
	else
		videoAlgorithmName = integerValue;

	if (!findXmlData(memoryPath + xmlAlgorithmPath, "IMAGE", integerValue))
		return false;
	else
		imageAlgorithmName = integerValue;


	for (int a = 0; a < 100; a++)
	{
		fileMod = "ALG" + std::to_string(a);

		if (!findXmlData(memoryPath + xmlAlgorithmList, fileMod.c_str(), stringValue))
			return false;

		else
		{
			if (stringValue == "VOID")
			{
				algorithmListName[a] = -1;
			}
			else
			{
				algorithmListName[a] = std::stoi(stringValue);
			}
		}

	}




	//if (!findAudioName(memoryPath, blank))
	//	return false;
	//if (!findVideoName(memoryPath, blank))
	//	return false;
	//if (!findImageName(memoryPath, blank))
	//	return false;
	//
	//if (!findAudioProcess(memoryPath, blank))
	//	return false;
	//if (!findVideoProcess(memoryPath, blank))
	//	return false;
	//if (!findImageProcess(memoryPath, blank))
	//	return false;
	//
	//if (!findAudioAlgorithm(memoryPath, blank))
	//	return false;
	//if (!findVideoAlgorithm(memoryPath, blank))
	//	return false;
	//if (!findImageAlgorithm(memoryPath, blank))
	//	return false;

	for (int a = 0; a < 100; a++)
	{
		videoProcQueue[a] = 0;
	}

	return true;
}


//Test function to see if only one is needed to import all that data
bool label::findXmlData(std::string xmlPath, std::string elementName, int& value)
{
	tinyxml2::XMLDocument xmlLoad;
	tinyxml2::XMLNode* pRoot;
	tinyxml2::XMLElement* pElement;
	tinyxml2::XMLError eResult;

	eResult = xmlLoad.LoadFile(xmlPath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "There was an error loading '" << xmlPath << "'\n\n";
		return false;
	}

	pRoot = xmlLoad.FirstChild();
	if (pRoot == nullptr)
	{
		std::cout << "Could not find first child of '" << xmlPath << "'\n\n";
		return false;
	}

	pElement = pRoot->FirstChildElement(elementName.c_str());
	if (pElement == nullptr)
	{
		std::cout << "Could not find '" << elementName << "' in path '" << xmlPath << "'.\n\n";
		return false;
	}

	eResult = pElement->QueryIntText(&value);
	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not extract '" << elementName << "' in path '" << xmlPath << "'.\n\n";
		return false;
	}

	std::cout << "Successfully extracted data from '" << xmlPath << "'.\n";
	return true;
}

bool label::findXmlData(std::string xmlPath, std::string elementName, std::string& value)
{
	tinyxml2::XMLDocument xmlLoad;
	tinyxml2::XMLNode* pRoot;
	tinyxml2::XMLElement* pElement;
	tinyxml2::XMLError eResult;

	eResult = xmlLoad.LoadFile(xmlPath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "There was an error loading '" << xmlPath << "'\n\n";
		return false;
	}

	pRoot = xmlLoad.FirstChild();
	if (pRoot == nullptr)
	{
		std::cout << "Could not find first child of '" << xmlPath << "'\n\n";
		return false;
	}

	pElement = pRoot->FirstChildElement(elementName.c_str());
	if (pElement == nullptr)
	{
		std::cout << "Could not find '" << elementName << "' in path '" << xmlPath << "'.\n\n";
		return false;
	}

	const char* importText = nullptr;

	importText = pElement->GetText();
	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not extract '" << elementName << "' in path '" << xmlPath << "'.\n\n";
		return false;
	}

	value = importText;

	return true;
}


bool label::saveNames(std::string memoryPath)
{
	std::string completePath = memoryPath + xmlObjectPath;

	tinyxml2::XMLDocument xmlFile;

	tinyxml2::XMLNode* pRoot = xmlFile.NewElement("Root");
	xmlFile.InsertFirstChild(pRoot);

	tinyxml2::XMLElement* pElement = xmlFile.NewElement("AUDIO");
	pElement->SetText(audioName);
	pRoot->InsertEndChild(pElement);

	pElement = xmlFile.NewElement("VIDEO");
	pElement->SetText(videoName);
	pRoot->InsertEndChild(pElement);

	pElement = xmlFile.NewElement("IMAGE");
	pElement->SetText(imageName);
	pRoot->InsertEndChild(pElement);

	pElement = xmlFile.NewElement("AUDIO_PROCESS");
	pElement->SetText(audioProcessName);
	pRoot->InsertEndChild(pElement);

	pElement = xmlFile.NewElement("VIDEO_PROCESS");
	pElement->SetText(videoProcessName);
	pRoot->InsertEndChild(pElement);

	pElement = xmlFile.NewElement("IMAGE_PROCESS");
	pElement->SetText(imageProcessName);
	pRoot->InsertEndChild(pElement);

	tinyxml2::XMLError eResult = xmlFile.SaveFile(completePath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write directory to path.xml.\n\n";
		return false;
	}

	std::cout << "Successfully written to xml.\n\n";

	return true;
}