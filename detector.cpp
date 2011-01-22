#include <opencv/cv.h>

#include <cstdio>

#include <list>

#include "./detector.h"
#include "./tools.h"

namespace PolypDetector {

void Smooth(const cv::Mat &source, cv::Mat *result, double value) {
  cv::GaussianBlur(source, *result, cv::Size(value, value), 0);
}

void GetGaussianCurvature(const cv::Mat &image,
                          const cv::Mat &xDerivative,
                          const cv::Mat &yDerivative,
                          const cv::Mat &xxDerivative,
                          const cv::Mat &xyDerivative,
                          const cv::Mat &yyDerivative,
                          cv::Mat *gaussianCurvature) {
  assert(image.size() == gaussianCurvature->size());
  for (size_t x = 0; x < image.rows; x++) {
    for (size_t y = 0; y < image.cols; y++) {
      float dx = xDerivative.at<float>(x, y);
      float dy = yDerivative.at<float>(x, y);
      float dxx = xxDerivative.at<float>(x, y);
      float dxy = xyDerivative.at<float>(x, y);
      float dyy = yyDerivative.at<float>(x, y);
      // printf("Up %d Down %d Curv %f \n",  dxx * dyy - dxy * dxy,
      //        (1 + dx * dx + dy * dy) * (1 + dx * dx + dy * dy),
      //        (static_cast<double>(dxx * dyy - dxy * dxy)  /
      //         (1 + dx * dx + dy * dy) * (1 + dx * dx + dy * dy)));
      gaussianCurvature->at<double>(x, y) =
        (dxx * dyy - dxy * dxy)  /
        ((1 + dx * dx + dy * dy) * (1 + dx * dx + dy * dy));
    }
  }
}

std::list<Descriptor> Locate(const cv::Mat &capsuleImage,
                             double smoothValue) {
  cv::Size size = cv::Size(300, 300);
  cv::Mat resizedImage;
  cv::resize(capsuleImage, resizedImage, size);

  cv::Mat grayImage;
  cv::cvtColor(resizedImage, grayImage, CV_RGB2GRAY);

  cv::Mat smoothImage;
  Smooth(grayImage, &smoothImage, smoothValue);

  cv::Mat xDerivative, yDerivative, xxDerivative,
    xyDerivative, yyDerivative;
  cv::Sobel(smoothImage, xDerivative, 5, 1, 0);
  cv::Sobel(smoothImage, yDerivative, 5, 0, 1);
  cv::Sobel(smoothImage, xxDerivative, 5, 2, 0);
  cv::Sobel(smoothImage, xyDerivative, 5, 1, 1);
  cv::Sobel(smoothImage, yyDerivative, 5, 0, 2);

  // ShowHeatmap<float>(xDerivative, "x");
  // ShowHeatmap<float>(yDerivative, "y");
  // ShowHeatmap<float>(xxDerivative, "xx");
  // ShowHeatmap<float>(xyDerivative, "xy");
  // ShowHeatmap<float>(yyDerivative, "yy");

  cv::Mat gaussianCurvature(smoothImage.size(), CV_64FC1);
  GetGaussianCurvature(smoothImage,
                       xDerivative, yDerivative, xxDerivative,
                       xyDerivative, yyDerivative, &gaussianCurvature);

  ShowHeatmap<double>(gaussianCurvature);
  // PrintMatrix<double>(gaussianCurvature);

  std::list<Descriptor> result;
  return result;
}
}
