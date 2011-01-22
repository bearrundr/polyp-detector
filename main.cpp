#include <opencv/highgui.h>

#include <cstdio>

#include <list>

#include "./detector.h"

int main(int argc, char *argv[]) {
  cv::Mat wcImage = cv::imread("wc_images/test/torus1.jpg");

  // std::list<PolypDetector::Descriptor> polyps =
  PolypDetector::Locate(wcImage);
  return 0;
}
