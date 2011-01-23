#include <opencv/cv.h>

#include <cstdio>
#include <cmath>

#include <list>
#include <algorithm>

#include "./detector.h"
#include "./tools.h"

namespace PolypDetector {

void Smooth(const cv::Mat &source, cv::Mat *result, double value) {
  cv::GaussianBlur(source, *result, cv::Size(value, value), 0);
}

void GetGaussianCurvature(const cv::Mat &xDerivative,
                          const cv::Mat &yDerivative,
                          const cv::Mat &xxDerivative,
                          const cv::Mat &xyDerivative,
                          const cv::Mat &yyDerivative,
                          cv::Mat *gaussianCurvature) {
  assert(xDerivative.size() == gaussianCurvature->size()
         && yDerivative.size() == gaussianCurvature->size()
         && xxDerivative.size() == gaussianCurvature->size()
         && xyDerivative.size() == gaussianCurvature->size()
         && yyDerivative.size() == gaussianCurvature->size());
  for (size_t x = 0; x < gaussianCurvature->rows; x++) {
    for (size_t y = 0; y < gaussianCurvature->cols; y++) {
      float dx = xDerivative.at<float>(x, y);
      float dy = yDerivative.at<float>(x, y);
      float dxx = xxDerivative.at<float>(x, y);
      float dxy = xyDerivative.at<float>(x, y);
      float dyy = yyDerivative.at<float>(x, y);
      gaussianCurvature->at<double>(x, y) =
        (dxx * dyy - dxy * dxy)  /
        ((1 + dx * dx + dy * dy) * (1 + dx * dx + dy * dy));
    }
  }
}

void GetMeanCurvature(const cv::Mat &xDerivative,
                      const cv::Mat &yDerivative,
                      const cv::Mat &xxDerivative,
                      const cv::Mat &xyDerivative,
                      const cv::Mat &yyDerivative,
                      cv::Mat *meanCurvature) {
  assert(xDerivative.size() == meanCurvature->size()
         && yDerivative.size() == meanCurvature->size()
         && xxDerivative.size() == meanCurvature->size()
         && xyDerivative.size() == meanCurvature->size()
         && yyDerivative.size() == meanCurvature->size());
  for (size_t x = 0; x < meanCurvature->rows; x++) {
    for (size_t y = 0; y < meanCurvature->cols; y++) {
      float dx = xDerivative.at<float>(x, y);
      float dy = yDerivative.at<float>(x, y);
      float dxx = xxDerivative.at<float>(x, y);
      float dxy = xyDerivative.at<float>(x, y);
      float dyy = yyDerivative.at<float>(x, y);
      meanCurvature->at<double>(x, y) =
        ((1 + dxx) * (1 + dxx) * dyy -
         2 * dx * dy * dxy +
         (1 + dyy) * (1 + dyy) * dxx)
        /
        (2 * std::sqrt((1 + dx * dx + dy * dy) *
                       (1 + dx * dx + dy * dy) *
                       (1 + dx * dx + dy * dy)));
    }
  }
}

void GetPolypCurvature(const cv::Mat &gaussianCurvature,
                       const cv::Mat &meanCurvature,
                       cv::Mat *polypCurvature) {
  assert(gaussianCurvature.size() == polypCurvature->size()
         && meanCurvature.size() == polypCurvature->size());
  for (size_t x = 0; x < polypCurvature->rows; x++) {
    for (size_t y = 0; y < polypCurvature->cols; y++) {
      polypCurvature->at<double>(x, y) =
        - gaussianCurvature.at<double>(x, y) *
        std::min(meanCurvature.at<double>(x, y), 0.0);
    }
  }
}

std::list<Descriptor> Locate(const cv::Mat &capsuleImage,
                             double smoothValue,
                             double derivativeSmoothValue) {
  cv::Size size = cv::Size(300, 300);
  cv::Mat resizedImage;
  cv::resize(capsuleImage, resizedImage, size);

  cv::Mat grayImage;
  cv::cvtColor(resizedImage, grayImage, CV_RGB2GRAY);

  cv::Mat smoothImage;
  Smooth(grayImage, &smoothImage, smoothValue);

  ShowImage(smoothImage, "Smooth image");

  cv::Mat xDerivative, yDerivative, xxDerivative,
    xyDerivative, yyDerivative;
  cv::Mat smoothXDerivative, smoothYDerivative,
    smoothXXDerivative, smoothXYDerivative,
    smoothYYDerivative;

  int kernelType = 1;           // See opencv docs.
  cv::Sobel(smoothImage, xDerivative, 5, 1, 0, kernelType);
  Smooth(xDerivative, &smoothXDerivative, derivativeSmoothValue);
  cv::Sobel(smoothImage, yDerivative, 5, 0, 1, kernelType);
  Smooth(yDerivative, &smoothYDerivative, derivativeSmoothValue);
  cv::Sobel(smoothXDerivative, xxDerivative, 5, 1, 0, kernelType);
  cv::Sobel(smoothXDerivative, xyDerivative, 5, 0, 1, kernelType);
  cv::Sobel(smoothYDerivative, yyDerivative, 5, 0, 1, kernelType);
  Smooth(xxDerivative, &smoothXXDerivative, derivativeSmoothValue);
  Smooth(xyDerivative, &smoothXYDerivative, derivativeSmoothValue);
  Smooth(yyDerivative, &smoothYYDerivative, derivativeSmoothValue);

  ShowHeatmap<float>(smoothXDerivative, "x derivative");
  ShowHeatmap<float>(smoothYDerivative, "y derivative");
  ShowHeatmap<float>(smoothXXDerivative, "xx derivative");
  ShowHeatmap<float>(smoothXYDerivative, "xy derivative");
  ShowHeatmap<float>(smoothYYDerivative, "yy derivative");

  cv::Mat gaussianCurvature(smoothImage.size(), CV_64FC1);
  GetGaussianCurvature(smoothXDerivative, smoothYDerivative,
                       smoothXXDerivative, smoothXYDerivative,
                       smoothYYDerivative, &gaussianCurvature);

  ShowHeatmap<double>(gaussianCurvature, "Gaussian curvature");

  cv::Mat meanCurvature(smoothImage.size(), CV_64FC1);
  GetMeanCurvature(smoothXDerivative, smoothYDerivative,
                       smoothXXDerivative, smoothXYDerivative,
                       smoothYYDerivative, &meanCurvature);

  ShowHeatmap<double>(meanCurvature, "Mean curvature");

  cv::Mat polypCurvature(smoothImage.size(), CV_64FC1);
  GetPolypCurvature(gaussianCurvature, meanCurvature,
                    &polypCurvature);

  ShowHeatmap<double>(polypCurvature, "Polyp curvature");

  std::list<Descriptor> result;
  return result;
}
}
