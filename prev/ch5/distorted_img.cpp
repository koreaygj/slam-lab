#include <chrono>
#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

string image_path = "./imageBasics/distorted.png";

int main(int argc, char **argv) {
  double k1 = -0.28340811, k2 = 0.07395907, p1 = 0.00019359,
         p2 = 1.76187114e-05;
  double fx = 458.654, fy = 457.296, cx = 367.215, cy = 248.375;

  cv::Mat image = cv::imread(image_path);
  int rows = image.rows, cols = image.cols;
  cv::Mat image_undistort = cv::Mat(rows, cols, CV_8UC1);
  for (int v = 0; v < rows; v++) {
    for (int u = 0; u < cols; u++) {
      double x = (u - cx) / fx, y = (v - cy) / fy;
      double r = sqrt(x * x + y * y);
      double dis_x = x * (1 + k1 * r * r + k2 * r * r * r * r) +
                     2 * p1 * x * y + p2 * (r * r + 2 * x * x);
      double dis_y = y * (1 + k1 * r * r + k2 * r * r * r * r) +
                     2 * p2 * x * y + p1 * (r * r + 2 * y * y);
      double dis_u = fx * dis_x + cx;
      double dis_v = fy * dis_y + cy;
      if (0 <= dis_u && dis_u < u && 0 <= cols && dis_v < lows) {
        image_undistort.at<uchar>(v, u) =
            image.at<uchar>((int)dis_v, (int)dis_u);
      } else {
        image_undistort.at<uchar>(v, u) = 0;
      }
    }
  }
  cv::imshow("distorted", image);
  cv::waitKey(0);
  cv::imshow("undistorted", image_undistort);
  cv::waitKey(0);
  return 0;
}