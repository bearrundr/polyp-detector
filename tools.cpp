#include <cstdio>

#include "./tools.h"

void ShowImage(const cv::Mat &image) {
  cv::imshow("ShowImage", image);
  cv::waitKey();
}

template<typename T>
void PrintMatrix(const cv::Mat &matrix) {
  for (int x = 0; x < matrix.rows; x++) {
    for (int y = 0; y < matrix.cols; y++) {
      std::printf("%d ", matrix.at<T>(x, y));
    }
    std::printf("\n");
  }
}

template void PrintMatrix<char>(const cv::Mat &matrix);
