#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <sstream>
#include <utility>
#include <algorithm>


#include "./tools.h"

void ShowImage(const cv::Mat &image, const std::string title) {
  cv::imshow(title, image);
  cv::waitKey();
}

void PrintText(const std::string text, cv::Mat *image) {
  int fontFace = cv::FONT_HERSHEY_PLAIN;
  double fontScale = 0.7;
  int fontThickness = 1;
  cv::Scalar fontColor = cv::Scalar::all(255);

  int textBaseline;
  cv::Size textSize = cv::getTextSize(text, fontFace,
                                      fontScale, fontThickness, &textBaseline);

  cv::Point textPosition(textSize.width / 2,
                         textSize.height + 5);

  cv::putText(*image, text, textPosition,
              fontFace, fontScale, fontColor, fontThickness);
}

template<typename T>
std::pair<T, T> GetMinMax(const cv::Mat &matrix) {
  T minimum, maximum;
  minimum = maximum = matrix.at<T>(0, 0);
  for (size_t x = 0; x < matrix.rows; x++) {
    for (size_t y = 0; y < matrix.cols; y++) {
      T current = matrix.at<T>(x, y);
      maximum = std::max(current, maximum);
      minimum = std::min(current, minimum);
    }
  }
  return std::make_pair<T, T>(minimum, maximum);
}

template std::pair<float, float> GetMinMax<float>(const cv::Mat &matrix);
template std::pair<double, double> GetMinMax<double>(const cv::Mat &matrix);


template<typename T>
void ShowHeatmap(const cv::Mat &image, const std::string title) {
  std::pair<T, T> minMax = GetMinMax<T>(image);
  T minimum = minMax.first, maximum = minMax.second;

  if (minimum == maximum) {
    ShowImage(image, title);
  }

  cv::Mat heatmap(image.size(), CV_8UC3);
  cv::Mat icefire = cv::imread("resources/icefire.jpg");

  for (size_t x = 0; x < image.rows; x++) {
    for (size_t y = 0; y < image.cols; y++) {
      double current = static_cast<double>(image.at<T>(x, y));
      double shift;
      if (current < 0) {
        shift = current / minimum;
        shift *= -1;
      } else {
        shift = current / maximum;
      }
      heatmap.at<cv::Vec3b>(x, y) =
        icefire.at<cv::Vec3b>(1, static_cast<size_t>(icefire.cols /
                                                  2 * (1 + shift)));
    }
  }
  std::stringstream heatTitle;
  heatTitle << "Min: " << minimum << " " << "Max: " << maximum;
  PrintText(heatTitle.str(), &heatmap);

  ShowImage(heatmap, title);
}

template void ShowHeatmap<float>(const cv::Mat &image,
                                 const std::string title);
template void ShowHeatmap<double>(const cv::Mat &image,
                                 const std::string title);


template<typename T>
void PrintMatrix(const cv::Mat &matrix) {
  for (size_t x = 0; x < matrix.rows; x++) {
    for (size_t y = 0; y < matrix.cols; y++) {
      std::cout << matrix.at<T>(x, y) << ' ';
    }
    std::cout << std::endl;
  }
}

template void PrintMatrix<float>(const cv::Mat &matrix);
template void PrintMatrix<double>(const cv::Mat &matrix);

