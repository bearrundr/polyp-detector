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
  for (int x = 0; x < image.rows; x++) {
    for (int y = 0; y < image.cols; y++) {
      char dx = xDerivative.at<char>(x, y);
      char dy = yDerivative.at<char>(x, y);
      char dxx = xxDerivative.at<char>(x, y);
      char dxy = xyDerivative.at<char>(x, y);
      char dyy = yyDerivative.at<char>(x, y);
      gaussianCurvature->at<char>(x, y) = (dxx * dyy - dxy * dxy) /
      (1 + dx * dx + dy * dy) * (1 + dx * dx + dy * dy);
    }
  }
}

std::list<Descriptor> Locate(const cv::Mat &capsuleImage,
                             double smoothValue) {
  cv::Mat grayImage;
  cv::cvtColor(capsuleImage, grayImage, CV_RGB2GRAY);

  cv::Mat smoothImage;
  Smooth(grayImage, &smoothImage, smoothValue);

  cv::Mat xDerivative, yDerivative, xxDerivative,
    xyDerivative, yyDerivative;
  cv::Sobel(smoothImage, xDerivative, 0, 1, 0);
  cv::Sobel(smoothImage, yDerivative, 0, 0, 1);
  cv::Sobel(smoothImage, xxDerivative, 0, 2, 0);
  cv::Sobel(smoothImage, xyDerivative, 0, 1, 1);
  cv::Sobel(smoothImage, yyDerivative, 0, 0, 2);

  cv::Mat gaussianCurvature(smoothImage.size(), CV_8UC1);
  GetGaussianCurvature(smoothImage,
                       xDerivative, yDerivative, xxDerivative,
                       xyDerivative, yyDerivative, &gaussianCurvature);
  ShowImage(gaussianCurvature);

  std::list<Descriptor> result;
  return result;
}
}
