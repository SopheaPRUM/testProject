#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat imagProcessing(const Mat &inputImage){

    //grayscale pre-processing
    Mat img_gray;
    cvtColor(inputImage, img_gray, CV_RGB2GRAY);
    //size normalize

    float fx = 100.0/img_gray.cols;
    float fy = 50.0/img_gray.rows;
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
    HOGDescriptor d( Size(32,16), Size(8,8), Size(4,4), Size(4,4), 9);
    d.compute( inputImage, descriptorsValues, Size(0,0), Size(0,0), locations);
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat inputImage = imagProcessing(argv[1]);

    vector< float> descriptorsValues;
    vector< Point> locations;

    getHOGdescriptors(inputImage, descriptorsValues, locations);

    //for(int i=0; i<descriptorsValues.size(); i++){
    //    cout<<descriptorsValues.at(i)<<", ";
    //}
    cout<<descriptorsValues.size()<<endl;
    cout<<locations.size()<<endl;
    test
    return 0;
}