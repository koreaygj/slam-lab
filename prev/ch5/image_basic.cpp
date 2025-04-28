#include <chrono>
#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "이미지 파일 이름을 입력하세요" << endl;
    return -1;
  }
  cv::Mat image;
  image = cv::imread(argv[1]);
  if (image.data == nullptr) {
    cerr << "문서" << argv[1] << "존재하지 않음" << endl;
    return 0;
  }
  cout << "이미지 열" << image.cols << "행" << image.rows << ", 채널수 "
       << image.channels() << endl;
  cv::imshow("image", image);
  cv::waitKey(0);  // 버튼 입력 대기

  if (image.type() != CV_8UC1 && image.type() != CV_8UC3) {
    cout << "이미지 유형이 요구사항을 충족하지 않음" << endl;
    return 0;
  }
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  for (size_t y = 0; y < image.rows; y++) {
    unsigned char *row_ptr = image.ptr<unsigned char>(y);
    for (size_t x = 0; x < image.cols; x++) {
      unsigned char *data_ptr = &row_ptr[x * image.channels()];
      for (int c = 0; c != image.channels(); c++) {
        unsigned char data = data_ptr[c];
      }
    }
  }
  chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
  chrono::duration<double> time_used =
      chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cout << "이미지를 통과하는데 걸리는 시간 " << time_used.count() << "초"
       << endl;

  cv::Mat image_another = image;
  image_another(cv::Rect(0, 0, 100, 100)).setTo(0);
  cv::imshow("image", image);
  cv::waitKey(0);

  cv::Mat image_clone = image.clone();
  image_clone(cv::Rect(0, 0, 100, 100)).setTo(255);
  cv::imshow("image", image);
  cv::imshow("image_clone", image_clone);
  cv::waitKey(0);

  cv::destroyAllWindows();

  return 0;
}