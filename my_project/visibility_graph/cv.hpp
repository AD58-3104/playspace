#ifndef CV_H_
#define CV_H_

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "graph.hpp"
class drawing
{
public:
    drawing() : img(cv::Mat::zeros(1000, 1000, CV_8UC3)),color(255,50,50)
    {
        // cv::line(img, cv::Point(100, 300), cv::Point(400, 305), cv::Scalar(200, 0, 0), 5, 8);
        // fillConvexPolyとpolylines
    }

    void display(const std::vector<shape>& obj)
    {
        writeShape(obj);
        cv::imshow("visibility graph", img);
        cv::waitKey(1000000);
    }

private:
    cv::Mat img;
    cv::Scalar color;
    void writeShape(const std::vector<shape>& shapes)
    {
        std::vector<std::vector<cv::Point>> objects_points;
        for(const auto &itm:shapes){
            std::vector<cv::Point> tmp;
            for(const auto&point:itm.points){
                tmp.push_back(cv::Point(static_cast<int>(point.first),static_cast<int>(point.second)));
                std::cout << "point\n";
            }
            objects_points.emplace_back(std::move(tmp));
        }
        
        for(const std::vector<cv::Point> &obj:objects_points){
            cv::fillConvexPoly(img,obj,color);
            // cv::polylines(img,obj,true,color);
            std::cout << "write";
        }

    }
};

// int main(int argc, char const *argv[])
// {
//     drawing draw;
//     draw.display();
//     return 0;
// }


#endif // !CV_H_