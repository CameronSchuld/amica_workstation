//Cameron Schuld
//10-13-2020
//Pattern Recognition Algorithm Creation

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>

#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

#include <Windows.h>
#include <fileapi.h>

#include "vision.h"
#include "label.h"
#include "tinyXML/include/tinyxml2.h"

//File checking
bool dataIntegrityCheck(std::string&); //Checks file structure and integral xml files
bool buildFileStructure(std::string); //If file structure doesn't exist it needs to be built
bool createNewFilesystem(); //Gathers specified information on creating a new file structure
bool pathExtraction(std::string&); //Extract directory path for memory storage
bool setPath(std::string); //Set directory path for memory storage

bool createAugChildXML(std::string, int); //Creates xml file for keeping track of child augmentations
bool readAugChildXML(std::string, std::string, int&); //Reads xml for child augmentation
bool readAugChildXML(std::string, std::string, std::string&); //Same thing with string not int
bool writeAugChildXML(std::string, std::string, int); //Writes to the child augmentation xml doc

bool createAugListXML(std::string); //Creates the augmentation list to be filled with values
bool importAugListXML(std::string); //Imports all data from augmentation list 
bool addAugListXML(std::string, int); //Adds integer representation to augmentation list

void videoMod(label*, std::string);


int main()
{
	std::string memoryPath = "";

	if(!dataIntegrityCheck(memoryPath))
	{
		std::cout << "\nEXITING...";
		return -1;
	}

	cv::VideoCapture capture(0);
	if (!capture.isOpened())
	{
		std::cout << "There was a webcam error.\n";
		std::cout << "Make sure you have a webcam connected and identified in settings.\n";
		return -1;
	}

	//Video monitoring
	cv::Mat frame;
	cv::Mat lastFrame;
	cv::Mat diffFrame;
	cv::Mat cannyOutput;
	cv::Mat threshold;
	cv::Mat drawing;

	cv::Ptr<cv::BackgroundSubtractor> pBackSub;
	pBackSub = cv::createBackgroundSubtractorKNN(200, 300, true);

	int frameWidth = capture.get(cv::CAP_PROP_FRAME_WIDTH);
	int frameHeight = capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	double difference;
	bool active = false;
	bool frameCapture = false;
	int keyboard;

	std::string audioSavePath = memoryPath + "\\object_storage\\audio";
	std::string videoSavePath = memoryPath + "\\object_storage\\video";
	std::string imageSavePath = memoryPath + "\\object_storage\\image";
	std::string audioName;
	std::string videoName;
	std::string imageName;

	label label;
	if(!label.importAllSenses(memoryPath))
	{
		std::cout << "There was a problem importing the correct files.\n";
		std::cout << "Quitting...";
		return -1;
	}

	std::cout << label.audioName << "\n";
	std::cout << label.videoName << "\n";
	std::cout << label.imageName << "\n";
	std::cout << label.audioProcessName << "\n";
	std::cout << label.videoProcessName << "\n";
	std::cout << label.imageProcessName << "\n";


	std::vector<std::vector<cv::Point>> allContours;
	std::vector<cv::Vec4i> hierarchy;

	std::string folderName = videoSavePath + "\\video" + std::to_string(label.videoName);
	std::string fileName = folderName + "\\video" + std::to_string(label.videoName) + ".avi";
	std::string xmlFileName = folderName + "\\";
	cv::VideoWriter output;


	std::thread vidProc(videoMod, &label, memoryPath);
	vidProc.detach();


	while (true)
	{
		capture >> frame;

		//Applies the foreground mask as well as the threshold
		cv::Mat fgMask(frame.size(), CV_8UC1, cv::Scalar(0, 0, 0));
		pBackSub->apply(frame, fgMask);
		cv::threshold(fgMask, threshold, 200, 255, cv::THRESH_BINARY);


		//Calculates the difference in frames, executes capture
		difference = cv::countNonZero(threshold);

		//If meets arguments start a recording
		if (difference >= 10000 && active)
		{

			fileName = videoSavePath + "\\video" + std::to_string(label.videoName) + ".avi";

			if (!frameCapture)
			{
				std::string folderName = videoSavePath + "\\video" + std::to_string(label.videoName);
				std::string fileName = folderName + "\\video" + std::to_string(label.videoName) + ".avi";


				if (CreateDirectoryA(folderName.c_str(), NULL))
					std::cout << "Directory '" << folderName << "' created.\n";
				else
				{
					if (GetLastError() != ERROR_ALREADY_EXISTS)
					{
						std::cout << "Could not create '" << folderName << "'.\n";
						return false;
					}
				}


				frameCapture = true;
				output.open(fileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'g'), 20, cv::Size(frameWidth, frameHeight));
			}						
			output.write(frame);
		}


		//If difference is below threshold and frameCapture is still true
		//It signifies that this is the final frame being recorded
		//It then saves the file and adds one to the name scheme
		else if (difference < 20000 && frameCapture)
		{
			frameCapture = false;

			xmlFileName = videoSavePath + "\\video" + std::to_string(label.videoName) + "\\child.xml";
			fileName = videoSavePath + "\\video" + std::to_string(label.videoName) + ".avi";
			output.write(frame);
			output.release();

			createAugChildXML(xmlFileName, 0);

			for (int a = 0; a < 100; a++)
			{
				if (label.videoProcQueue[a] == 0)
				{
					label.videoProcQueue[a] = label.videoName;
					break;
				}
			}

			label.videoName++;
		}

		//Shows the threshold
		cv::imshow("Threshold", threshold);

		//Exits and saves on 'q' or 'esc'
		keyboard = cv::waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
		{
			if (frameCapture)
			{
				output.write(frame);
				output.release();
				xmlFileName = videoSavePath + "\\video" + std::to_string(label.videoName) + "\\child.xml";
				createAugChildXML(xmlFileName, 0);
				label.videoName++;
			}
			break;
		}
		//Activates and deactivates on spacebar
		if (keyboard == ' ')
			active = !active;
	}

	label.saveNames(memoryPath);

	std::cout << "\n=================EXITING===================\n\n";

	return 0;
}

void videoMod(label* label, std::string memoryPath)
{
	std::string videoSavePath = memoryPath + "\\object_storage\\video";
	std::string imageSavePath = memoryPath + "\\object_storage\\image";
	std::string fileIn;
	std::string fileOut;
	std::string xmlPath;
	std::string videoIn;
	std::string xmlSavePath;

	int queueSize = 0;
	int random = 0;
	int randomSize = 0;
	int saveName = 0;
	int childName = 0;

	srand(time(NULL));

	//Runs untill a break
	while (true)
	{
		//New declaraction every run in new scope
		vision vision;
		cv::Mat frame;

		//If process queue has nothing in it's first value the queue contains nothing
		//Random video is selected and augmented then processed to make sure
		//The computer is always thinking and always trying to make connections
		if (label->videoProcQueue[0] == 0)
		{
			if (label->videoName == 0)
				continue;
			random = rand() % label->videoName;
			fileIn = videoSavePath + "\\video" + std::to_string(random);
			videoIn = fileIn + "\\video" + std::to_string(random);
			xmlPath = fileIn + "\\child.xml";
			label->findXmlData(xmlPath, "CHILD", saveName);
		}

		//If there is something in the first place of the queue
		//It is extracted and the queue moves up
		else
		{
			fileIn = videoSavePath + "\\video" + std::to_string(label->videoProcQueue[0]);
			videoIn = fileIn + "\\video" + std::to_string(label->videoProcQueue[0]) + ".avi";
			xmlPath = fileIn + "\\child.xml";
			label->findXmlData(xmlPath, "CHILD", saveName);

			for (int a = 1; a < 100; a++)
			{
				label->videoProcQueue[a - 1] = label->videoProcQueue[a];
				label->videoProcQueue[99] = 0;
			}
		}

		cv::VideoCapture capture;
		capture.open(videoIn);

		if (!capture.isOpened())
		{
			continue;
		}

		int frameWidth = capture.get(cv::CAP_PROP_FRAME_WIDTH);
		int frameHeight = capture.get(cv::CAP_PROP_FRAME_HEIGHT);

		//String to save the file
		fileOut = fileIn + "\\aug" + std::to_string(saveName) + ".avi";

		//Output is initialized after video is selected and path is assigned
		cv::VideoWriter output;
		output.open(fileOut, cv::VideoWriter::fourcc('M', 'J', 'P', 'g'), 20, cv::Size(frameWidth, frameHeight));


		//If the file information matches up with another or shows similarities with another
		//Execute the same augmentation pattern to keep consistency
		//Side note you should probably have it recalculate the augmentation
		//Or change it slightly to prevent bad agumentations from taking priority and hindering finding connections
		if (false)
		{
			
		}

		//Else create a new analyzing technique at random
		else
		{
			
		}

		//Needs an operation in order to configure some of the used augmentations
		//Augmentations like background subtraction need setup before use 
		//And need parameters before use
		vision.initBackgroundSeparation(ARG_KNN, 100, 100, ARG_SHADOW);
		

		while (true)
		{
			capture >> frame;

			if (frame.empty())
				break;

			vision.nextFrame(frame);

			if (true)//blur
			{
				vision.blurFrame(9, 9);
			}
			if (true)//background sub
			{
				vision.subtractBackground();
			}
			if (false)//foreground sub
			{
				vision.subtractForeground();
			}
			if (true)
			{
				vision.thresh();
			}
			if (true)//dilate
			{
				vision.dilate();
			}
			if (true)//erode
			{
				vision.erode();
			}
			if (true)//canny
			{
				vision.canny();
			}
			if (true)//contour
			{
				vision.contourObject();
			}
			if (true)//draw contour
			{
				vision.drawContours();
			}


			cv::imshow("Memory Analysis", vision.drawing);
			output.write(vision.alteredFrame);

			char c = (char)cv::waitKey(25);
			if (c == 27)
				break;
		}
		cv::destroyAllWindows();
		capture.release();
		output.release();

		//Increases the number inside child.xml under CHILD node when augmentation is applied
		readAugChildXML(xmlPath, "CHILD", childName);
		childName++;
		writeAugChildXML(xmlPath, "CHILD", childName);

	}
	return;
}

bool createAugChildXML(std::string filePath, int value)
{
	tinyxml2::XMLDocument xmlObjectFile;

	tinyxml2::XMLNode* rootObject = xmlObjectFile.NewElement("Root");
	xmlObjectFile.InsertFirstChild(rootObject);

	tinyxml2::XMLElement* elementObject = xmlObjectFile.NewElement("CHILD");
	elementObject->SetText(value);
	rootObject->InsertEndChild(elementObject);

	tinyxml2::XMLError eResult = xmlObjectFile.SaveFile(filePath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write directory to '" << filePath << "'.\n\n";
		return false;
	}
	else
		std::cout << "XML written to '" << filePath << "'\n";


	return true;
}

bool readAugChildXML(std::string xmlPath, std::string elementName, int& value)
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

bool writeAugChildXML(std::string xmlPath, std::string elementName, int value)
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

	pRoot->DeleteChildren();

	pElement = xmlLoad.NewElement("CHILD");
	pElement->SetText(value);
	pRoot->InsertEndChild(pElement);

	eResult = xmlLoad.SaveFile(xmlPath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write to '", xmlPath, "'.\n\n";
		return false;
	}

	std::cout << "Successfully extracted data from '" << xmlPath << "'.\n";
	return true;
}

bool createAugListXML(std::string xmlPath)
{
	//Building xml document for object directory
	tinyxml2::XMLDocument xmlObjectFile;

	tinyxml2::XMLNode* rootObject = xmlObjectFile.NewElement("Root");
	xmlObjectFile.InsertFirstChild(rootObject);

	tinyxml2::XMLElement* elementObject = xmlObjectFile.NewElement("LENGTH");
	elementObject->SetText(0);
	rootObject->InsertEndChild(elementObject);

	tinyxml2::XMLError eResult = xmlObjectFile.SaveFile(xmlPath.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write augmentation list to '" << xmlPath << "'.\n\n";
		return false;
	}
	else
		std::cout << "XML written to '" << xmlPath << "'\n";

	return true;
}

bool addAugListXML(std::string xmlPath, int value)
{
	tinyxml2::XMLDocument xmlLoad;
	tinyxml2::XMLNode* pRoot;
	tinyxml2::XMLElement* pElement;
	tinyxml2::XMLError eResult;
	int length;

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

	pElement = pRoot->FirstChildElement("LENGTH");
	if (pElement == nullptr)
	{
		std::cout << "Could not find 'LENGTH' in path '" << xmlPath << "'.\n\n";
		return false;
	}

	eResult = pElement->QueryIntText(&length);
	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not extract 'LENGTH' in path '" << xmlPath << "'.\n\n";
		return false;
	}

	std::cout << "Successfully extracted data from '" << xmlPath << "'.\n";

	pElement = xmlLoad.NewElement("AUG");
	

	return true;
}

bool dataIntegrityCheck(std::string& workingPath)
{
	tinyxml2::XMLNode* pRoot;
	tinyxml2::XMLElement* pElement;

	bool newFilePath;

	std::string input;
	std::string memoryPath = "";

	//Load xml data
	tinyxml2::XMLDocument xmlLoad;
	xmlLoad.LoadFile("path.xml");

	//File doesn't exist
	if (xmlLoad.Error()) {
		std::cout << "Could not find data file.\n";

		newFilePath = true;
	}
	//File does exist
	else
	{
		pRoot = xmlLoad.FirstChild();

		//Data doesn't exist
		if (pRoot == nullptr)
		{
			std::cout << "Program data was either corrupted or deleted.\n";
			newFilePath = true;
		}
		else
		{
			pElement = pRoot->FirstChildElement("MEMORY_PATH");

			//Other data doesn't exist
			if (pElement == nullptr)
				newFilePath = true;
			else
				newFilePath = false;
		}
	}

	//If new file path is needed send to creation function
	if (newFilePath)
	{
		createNewFilesystem();
	}

	pathExtraction(memoryPath);

	//Default information when initially installed
	if (memoryPath == "FIRST_RUN_SETUP")
	{
		std::cout << "Executing first time setup\n\n";

		while (true)
		{
			//Get input from user on valid input directory
			std::cout << "Please enter a valid folder to create runtime directories\n: ";
			std::getline(std::cin, input);

			//If build succeeds, update xml file with new directory
			if (buildFileStructure(input))
			{
				setPath(input);
				break;
			}
			else
				std::cout << "Check to make sure the path you entered is valid.\n\n";
		}

	}

	//Load xml once again. Not important now, but check later to fix (this is embarrasing)

	if (pathExtraction(memoryPath))
	{
		std::cout << "File read successfully. Memory path is:\n";
		std::cout << memoryPath;
		std::cout << "\n\n";
	}
	else
	{
		std::cout << "Could not verify integrity of Memory Structure.\n";
		std::cout << "Try restarting the application.\n";
		return false;
	}

	workingPath = memoryPath;
	
	return true;
}

bool createNewFilesystem()
{
	std::ifstream fileProbe; //Check if files exist

	std::string input;
	std::string memoryPath;
	std::string testPath;

	while (true)
	{
		std::cout << "\nPlease enter a file path to store memory.\n: ";
		std::getline(std::cin, memoryPath);

		fileProbe.open(memoryPath + "adata.xml");

		if (!fileProbe)
		{
			std::cout << "\n\nThere is no existing memory location with this file path:\n";
			std::cout << memoryPath << "\n\n";


			while (true)
			{
				std::cout << "Would you like to\n";
				std::cout << "a) create a new memory file structure at this location\n";
				std::cout << "b) enter a different memory location\n";
				std::cout << ": ";

				std::getline(std::cin, input);

				if (input != "a" && input != "b")
				{
					std::cout << "\n\n'" << input << "' is not recognized\n";
					continue;
				}
				break;
			}
		}
		else
		{
			std::cout << "Memory has been found.\n";
			std::cout << "Setting as default path.\n";
		}

		if (input == "a")
		{
			std::cout << "\n";
			if (buildFileStructure(memoryPath))
				break;
			else
				std::cout << "\nDouble check your file path.\n";
		}
			
	}

	setPath(memoryPath);
	
	return true;
}

bool buildFileStructure(std::string filePath)
{
	std::string fileMod;
	std::string fileNameMod;

	std::string pathNames[3];
	pathNames[0] = "\\algorithm_storage";
	pathNames[1] = "\\connection_storage";
	pathNames[2] = "\\object_storage";

	std::string pathNames2[3];
	pathNames2[0] = "\\video";
	pathNames2[1] = "\\image";
	pathNames2[2] = "\\audio";

	for (int a = 0; a < 3; a++)
	{
		fileMod = filePath + pathNames[a];

		if (CreateDirectoryA(fileMod.c_str(), NULL))
			std::cout << "Directory '" << fileMod << "' created.\n";
		else
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				std::cout << "Could not create directories in specified file path.\n";
				return false;
			}
		}
	}

	for (int a = 0; a < 3; a++)
	{
		fileMod = filePath + pathNames[0] + pathNames2[a];

		if (CreateDirectoryA(fileMod.c_str(), NULL))
			std::cout << "Directory '" << fileMod << "' created.\n";
		else
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				std::cout << "Could not create directories in specified file path";
				return false;
			}
		}

	}

	for (int a = 0; a < 3; a++)
	{
		fileMod = filePath + pathNames[2] + pathNames2[a];

		if (CreateDirectoryA(fileMod.c_str(), NULL))
			std::cout << "Directory '" << fileMod << "' created.\n";
		else
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				std::cout << "Could not create directories in specified file path.\n";
				return false;
			}
		}
	}

	//Building xml document for object directory
	fileMod = filePath + pathNames[2] + "\\filename.xml";

	tinyxml2::XMLDocument xmlObjectFile;

	tinyxml2::XMLNode* rootObject = xmlObjectFile.NewElement("Root");
	xmlObjectFile.InsertFirstChild(rootObject);

	tinyxml2::XMLElement* elementObject = xmlObjectFile.NewElement("VIDEO");
	elementObject->SetText(0);
	rootObject->InsertEndChild(elementObject);

	elementObject = xmlObjectFile.NewElement("AUDIO");
	elementObject->SetText(0);
	rootObject->InsertEndChild(elementObject);

	elementObject = xmlObjectFile.NewElement("IMAGE");
	elementObject->SetText(0);
	rootObject->InsertEndChild(elementObject);

	elementObject = xmlObjectFile.NewElement("AUDIO_PROCESS");
	elementObject->SetText(0);
	rootObject->InsertEndChild(elementObject);

	elementObject = xmlObjectFile.NewElement("VIDEO_PROCESS");
	elementObject->SetText(0);
	rootObject->InsertEndChild(elementObject);

	elementObject = xmlObjectFile.NewElement("IMAGE_PROCESS");
	elementObject->SetText(0);
	rootObject->InsertEndChild(elementObject);

	tinyxml2::XMLError eResult = xmlObjectFile.SaveFile(fileMod.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write directory to path.xml.\n\n";
		return false;
	}
	else
		std::cout << "XML written to '" << fileMod << "'\n";


	//Building xml document for algorithm directory
	fileMod = filePath + pathNames[0] + "\\filename.xml";

	tinyxml2::XMLDocument xmlAlgorithmFile;

	tinyxml2::XMLNode* rootAlgorithm = xmlAlgorithmFile.NewElement("Root");
	xmlAlgorithmFile.InsertFirstChild(rootAlgorithm);

	tinyxml2::XMLElement* elementAlgorithm = xmlAlgorithmFile.NewElement("VIDEO");
	elementAlgorithm->SetText(0);
	rootAlgorithm->InsertEndChild(elementAlgorithm);

	elementAlgorithm = xmlAlgorithmFile.NewElement("AUDIO");
	elementAlgorithm->SetText(0);
	rootAlgorithm->InsertEndChild(elementAlgorithm);

	elementAlgorithm = xmlAlgorithmFile.NewElement("IMAGE");
	elementAlgorithm->SetText(0);
	rootAlgorithm->InsertEndChild(elementAlgorithm);

	eResult = xmlAlgorithmFile.SaveFile(fileMod.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write directory '" << fileMod << "'\n\n";
		return false;
	}
	else
		std::cout << "XML written to '" << fileMod << "'\n";


	//Building xml document for algorithm video directory
	//Logs the most used algorithms and the link to their attributes
	fileMod = filePath + pathNames[0] + pathNames2[0] + "\\algorithmList.xml";

	tinyxml2::XMLDocument algorithmList;

	tinyxml2::XMLNode* algRoot = algorithmList.NewElement("Root");
	algorithmList.InsertFirstChild(algRoot);

	tinyxml2::XMLElement* algElement;

	for (int a = 0; a < 100; a++)
	{
		fileNameMod = "ALG" + std::to_string(a);
		algElement = algorithmList.NewElement(fileNameMod.c_str());
		algElement->SetText("VOID");
		algRoot->InsertEndChild(algElement);
	}

	eResult = algorithmList.SaveFile(fileMod.c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write directory '" << fileMod << "'\n\n";
		return false;
	}
	else
		std::cout << "XML written to '" << fileMod << "'\n\n";

	std::cout << "All directories successfully created.\n\n";
	return true;
}

bool pathExtraction(std::string& path)
{
	tinyxml2::XMLDocument xmlLoad;
	tinyxml2::XMLNode* pRoot;
	tinyxml2::XMLElement* pElement;
	tinyxml2::XMLError eResult;

	eResult = xmlLoad.LoadFile("path.xml");

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "There was an error loading path.xml.\n\n";
		return false;
	}

	pRoot = xmlLoad.FirstChild();
	if (pRoot == nullptr)
	{
		std::cout << "Could not find first child of path.xml.\n\n";
		return false;
	}

	pElement = pRoot->FirstChildElement("MEMORY_PATH");
	if (pElement == nullptr)
	{
		std::cout << "Could not find MEMORY_PATH in path.xml.\n\n";
		return false;
	}

	const char* attributeText = nullptr;

	attributeText = pElement->GetText();
	if (attributeText == nullptr)
	{
		std::cout << "Could not extract string in path.xml.\n\n";
		return false;
	}
	path = attributeText;

	return true;
}

bool setPath(std::string path)
{
	tinyxml2::XMLDocument xmlFile;

	tinyxml2::XMLNode* pRoot = xmlFile.NewElement("Root");
	xmlFile.InsertFirstChild(pRoot);

	tinyxml2::XMLElement* pElement = xmlFile.NewElement("MEMORY_PATH");
	pElement->SetText(path.c_str());
	pRoot->InsertEndChild(pElement);

	pElement = xmlFile.NewElement("MEMORY_SIZE");
	pElement->SetText(1029);
	pRoot->InsertEndChild(pElement);

	tinyxml2::XMLError eResult = xmlFile.SaveFile("path.xml");

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not write directory to path.xml.\n\n";
		return false;
	}

	std::cout << "Directory written to path.xml.\n\n";
	return true;
}