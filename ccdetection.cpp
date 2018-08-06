#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "Tools.hpp"

using namespace cv;
using namespace std;

Mat imagProcessing(const Mat &inputImage){

    //grayscale pre-processing
    Mat img_gray;
    cvtColor(inputImage, img_gray, CV_RGB2GRAY);
    //size normalize

    float fx = 100.0/img_gray.cols;
    float fy = 100.0/img_gray.rows;
    Mat output;
    cv::resize(img_gray, output, cv::Size(), fx, fy);
    return output;
}

Mat imagProcessing(string inputImageFN){


    Mat image = imread( inputImageFN, 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return image;    
    }

    return imagProcessing(image);
}

void getHOGdescriptors(const Mat &inputImage, std::vector<float> &descriptorsValues, std::vector<Point> &locations){
    //extract feature
    HOGDescriptor d( Size(32*2,16*2), Size(8*2,8*2), Size(4*2,4*2), Size(4*2,4*2), 9);
    d.compute( inputImage, descriptorsValues, Size(0,0), Size(0,0), locations);
}


void createWekaHeader(int featNumber){
    ofstream myfile;
    myfile.open ("weka.arff",ios::trunc);
    myfile << "@RELATION cervicalCancer"<<endl;

    for(int i=0; i<featNumber; i++){
        stringstream ss;
        ss <<i+1;
        myfile<<" @ATTRIBUTE Feat_"<<ss.str()<<" NUMERIC"<<endl;
    }
    

    myfile<<"@ATTRIBUTE class {normal, abnormal}"<<endl;
    myfile<<"@DATA"<<endl;
    myfile.close();
}


void addWekaFeatures(std::vector<float> &v, string className){
    ofstream myfile;
    myfile.open ("weka.arff",ios::app);

    string featureStr;
    for(int i=0; i<v.size(); i++){

        stringstream ss;
        ss <<v[i];
        featureStr = featureStr +","+ ss.str();
        
    }
    featureStr = featureStr+","+className;
    myfile<<featureStr<<endl;

    myfile.close();
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    
    //create weka header
    createWekaHeader(34020);

    string line;
    ifstream myfile(argv[1]);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {

            std::vector<string> fntmp = Tools::stringToStringVector(line, ' ');
            string gt = fntmp[0];


            Mat inputImage = imagProcessing(fntmp[1]);
            cout<<fntmp[1]<<endl;

            //HOG feature extraction
            std::vector<float> descriptorsValues;
            std::vector<Point> locations;

            getHOGdescriptors(inputImage, descriptorsValues, locations);
            //cout<<descriptorsValues.size()<<endl;

            addWekaFeatures(descriptorsValues, gt);

        }

        myfile.close();
    }

    else cout << "Unable to open file"; 

    return 0;
}