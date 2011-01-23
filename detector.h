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
 pixel. For more go to http://en.wikipedia.org/wiki/Gaussian_curvature
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
void GetGaussianCurvature(const cv::Mat &xDerivative,
                          const cv::Mat &yDerivative,
                          const cv::Mat &xxDerivative,
                          const cv::Mat &xyDerivative,
                          const cv::Mat &yyDerivative,
                          cv::Mat *gaussianCurvature);

/*!
 \brief Computes Mean curvature at each
 pixel. For more go to http://en.wikipedia.org/wiki/Mean_curvature
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
 \param meanCurvature Pointer to resulting matrix.
*/
void GetMeanCurvature(const cv::Mat &xDerivative,
                      const cv::Mat &yDerivative,
                      const cv::Mat &xxDerivative,
                      const cv::Mat &xyDerivative,
                      const cv::Mat &yyDerivative,
                      cv::Mat *meanCurvature);


/*!
 \brief Computes polyp curvature as -G min(M, 0) (x, y)
 where G(x, y) is gaussian curvature and M(x, y) is 
 mean curvature.
 \param gaussianCurvature Matrix with gaussian curvatures
 at each pixel.
 \param meanCurvature Matrix with mean curvatures at each
 pixel.
 \param polypCurvature Pointer to resulting matrix.
*/
void GetPolypCurvature(const cv::Mat &gaussianCurvature,
                       const cv::Mat &meanCurvature,
                       cv::Mat *polypCurvature);

/*!
  \brief Given an wireless capsule image tries to 
  locate polyps.
  \param capsuleImage CvMat image.
  \param smoothValue Bigger the value smoother the image.
  \param derivativeSmoothValue Bigger the value more will
  be smoothed x and y derivative before computing xx, xy
  and yy ones.
  \returns List of polyp descriptors. If it is empty, 
  no polyps were found.
*/
std::list<Descriptor> Locate(const cv::Mat &capsuleImage,
                             double smoothValue = 47,
                             double derivativeSmoothValue = 37);

}

#endif /* _DETECTOR_H_ */
