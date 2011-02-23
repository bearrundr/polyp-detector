#include <opencv/highgui.h>

#include <cstdio>

#include <list>

#include "./detector.h"
#include "./test_set.h"

int main(int argc, char *argv[]) {
  // cv::Mat wcImage = cv::imread("wc_images/test/torus_sphere2.bmp");
  std::list<TestSet::Descriptor> descriptors;
  ReadDescriptors("wc_images/description.txt", &descriptors);

  // std::list<PolypDetector::Descriptor> polyps =
  //   PolypDetector::Locate(wcImage);

  // for (std::list<PolypDetector::Descriptor>::const_iterator
  //        polyp = polyps.begin();
  //      polyp != polyps.end(); ++polyp) {
  //   polyp->Print();
  // }
  return 0;
}
