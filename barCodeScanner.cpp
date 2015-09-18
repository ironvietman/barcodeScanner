// barCodeScanner.cpp : Defines the entry point for the console application.
//

//#include "stdio.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;


/*
* Function: readImage
* Usage: bool s = readImage(imageName, image);
* ----------------------------------------
* Returns true if image was read successfully.
*/

bool readImage(string imageName, Mat& image)
{
	image = imread(imageName, IMREAD_GRAYSCALE); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return false;
	}

	return true;
}
/*
* Function: readImage
* Usage:  displayImg(image, name);
* ----------------------------------------
* Displays the image with <code>name</code> as title
*/
void displayImg(Mat image, string name)
{
	image.convertTo(image, CV_8U);
	namedWindow(name, WINDOW_AUTOSIZE); // Create a window for display.
	imshow(name, image); // Show our image inside it.
	cout << image(Range(0, 10), Range(0, 10)) << endl;
	return;
}

string decodedToDigit(string code)
{
	if (code == "3211")
		return "0";
	else if (code == "2221")
		return "1";
	else if (code == "2122")
		return "2";
	else if (code == "1411")
		return "3";
	else if (code == "1132")
		return "4";
	else if (code == "1231")
		return "5";
	else if (code == "1114")
		return "6";
	else if (code == "1312")
		return "7";
	else if (code == "1213")
		return "8";
	else if (code == "3112")
		return "9";
	else
		return "";
}

int main(int argc, char** argv)
{
	//cout << CV_VERSION << endl;
	//cout << argv[0] << argv[1] << endl;
	if (argc != 2)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}

	//Read and display
	Mat image;
	if (readImage(argv[1], image))
	{
		displayImg(image, "Orig");
	}
	else
	{
		return 0;
	}

	//Get a slice of the image 
	Rect region_of_interest = Rect(0, 50, image.cols, 10);
	Mat imgRoi = image(region_of_interest);
	threshold(imgRoi, imgRoi, 200, 255, THRESH_BINARY);
	displayImg(imgRoi, "thres");
	//fill some holes
	int type = MORPH_RECT;
	Mat element = getStructuringElement(type,Size(1, 10));
	morphologyEx(imgRoi, imgRoi, MORPH_CLOSE, element, Point(-1, -1), 4);
	//threshold(imgRoi, imgRoi, 160, 255, THRESH_BINARY);
	displayImg(imgRoi, "Cropped");
	
	normalize(imgRoi, imgRoi, 0, 1, NORM_MINMAX);

	//Start the decoding (only need the vector or values now)
	int unitBarSize = 0;
	int currentBarSize = 0;
	int color = 1;
	int slack = 1;
	string currentDecode = "";
	const uchar* Mi = imgRoi.ptr<uchar>(1); //Use row one of ROI
	cout << imgRoi.row(1) << endl;
	
	for (int i = 0; i < imgRoi.cols; i++)
	{
		int c = (int)Mi[i];
		//cout << c << endl;
		if (c != color)
		{
			if (color == 0 && unitBarSize == 0) //First time seeing a black bar
			{
				unitBarSize = currentBarSize;
				if (unitBarSize < 2)
					unitBarSize = 2;
				cout << "Unit Bar size in pixels: " << unitBarSize << endl;
			}

			//Decode
			if (unitBarSize > 0)
			{
				//1
				if (currentBarSize <= (unitBarSize + slack))
				{
					currentDecode.append("1");
				}
				//2
				else if ((unitBarSize + slack) < currentBarSize && currentBarSize <= (2*(unitBarSize + slack)))
				{
					currentDecode.append("2");
				}
				//3
				else if ((2 * (unitBarSize + slack)) < currentBarSize && currentBarSize < (3 * (unitBarSize+slack)))
				{
					currentDecode.append("3");
				}
				//4
				else 
				{
					currentDecode.append("4");
				}
			}

			//Reset
			color = c;
			currentBarSize = 1;
		}
		else
		{
			currentBarSize++;
		}

	}
	cout << "Bars decoded: " << currentDecode << endl;

	//Parst the decoded string to get numbers
	string finalNumber = "";
	cout << currentDecode.substr(0, 3) << endl;
	if (currentDecode.substr(0, 3).compare("111") == 0)
	{
		int sum=0;
		string code = "";
		for (int i = 3; i < currentDecode.length(); i++)
		{
			sum += currentDecode[i]-48;
			code.push_back(currentDecode[i]);
			//cout << sum << endl;
			if (sum == 7)
			{
				cout << code << endl;
				finalNumber.append(decodedToDigit(code));
				code = "";
				sum = 0;
			}
			else if (code.compare("11111") == 0)
			{
				cout << code << endl;
				code = "";
				sum = 0;
			}

		}
		
	}
	else
	{
		cout << "No Start code...Try without start code" << endl;
		int sum = 0;
		string code = "";
		finalNumber = "";
		for (int i = 0; i < currentDecode.length(); i++)
		{
			sum += currentDecode[i] - 48;
			code.push_back(currentDecode[i]);
			//cout << sum << endl;
			if (sum == 7)
			{
				cout << code << endl;
				finalNumber.append(decodedToDigit(code));
				code = "";
				sum = 0;
			}
			else if (code.compare("11111") == 0)
			{
				code = "";
				sum = 0;
			}

		}
	}

	cout <<"BarCode Number: " << finalNumber << endl;
	waitKey(0); // Wait for a keystroke in the window
	return 0;

}



