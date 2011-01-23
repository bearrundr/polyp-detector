#include <opencv/highgui.h>

#include <cstdio>

#include <list>

#include "./detector.h"

int main(int argc, char *argv[]) {
  cv::Mat wcImage = cv::imread("wc_images/test/torus_sphere1.bmp");

  std::list<PolypDetector::Descriptor> polyps =
    PolypDetector::Locate(wcImage);

  for (std::list<PolypDetector::Descriptor>::const_iterator
         polyp = polyps.begin();
       polyp != polyps.end(); ++polyp) {
    polyp->Print();
  }
  return 0;
}
