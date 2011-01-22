#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <opencv/cv.h>

#include <list>

namespace PolypDetector {

struct Descriptor {
  double confidence;
};

/*!
 \brief Smooths images using some filter.
 \param source Image to smooth.
 \param result Smoothed image.
 \param value Bigger the value smoother the image.
*/
void Smooth(const cv::Mat &source, cv::Mat *result, double value);

/*!
 \brief Computes Gaussian curvature at each
 pixel.
 \param image For that image curvatures are computed.
 \param xDerivative Matrix of x derivatives in each pixel
 of input image.
 \param yDerivative Matrix of y derivatives in each pixel
 of input image.
 \param xxDerivative Matrix of xx derivatives in each pixel
 of input image.
 \param xyDerivative Matrix of xy derivatives in each pixel
 of input image.
 \param yyDerivative Matrix of yy derivatives in each pixel
 of input image.
 \param gaussianCurvature Pointer to resulting matrix.
*/
void GetGaussianCurvature(const cv::Mat &image,
                          const cv::Mat &xDerivative,
                          const cv::Mat &yDerivative,
                          const cv::Mat &xxDerivative,
                          const cv::Mat &xyDerivative,
                          const cv::Mat &yyDerivative,
                          cv::Mat *gaussianCurvature);


/*!
  \brief Given an wireless capsule image tries to 
  locate polyps.
  \param capsuleImage CvMat image.
  \param smoothValue Bigger the smoothValue smoother the image.
  \returns List of polyp descriptors. If it is empty, 
  no polyps were found.
*/
std::list<Descriptor> Locate(const cv::Mat &capsuleImage,
                             double smoothValue = 47);

}

#endif /* _DETECTOR_H_ */
