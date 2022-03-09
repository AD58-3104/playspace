#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
class drawing
{
public:
    drawing() : img(cv::Mat::zeros(1000, 1000, CV_8UC3))
    {
        cv::line(img, cv::Point(100, 300), cv::Point(400, 305), cv::Scalar(200, 0, 0), 5, 8);
        //fillPolyとpolylines
    }
    void writeShape()
    {
    }
    void display()
    {
        cv::imshow("visibility graph", img);
        cv::waitKey(0);
    }

private:
    cv::Mat img;
};

int main(int argc, char const *argv[])
{
    drawing draw;
    draw.display();
    return 0;
}
