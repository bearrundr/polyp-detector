#include <opencv/highgui.h>

#include <cstdio>

#include <list>

#include "./detector.h"

int main(int argc, char *argv[]) {
  cv::Mat wcImage = cv::imread("wc_images/test/torus_sphere2.bmp");

  PolypDetector::Locate(wcImage);
  return 0;
}
