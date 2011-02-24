#include <opencv/highgui.h>

#include <cstdio>

#include <list>
#include <vector>
#include <string>

#include "./detector.h"
#include "./test_set.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    for (double smoothValue = 1; smoothValue <= 7; smoothValue += 2) {
      for (double derivativeSmoothValue = 25; derivativeSmoothValue <= 33;
           derivativeSmoothValue += 2) {
        for (double curvatureThreshold = 0; curvatureThreshold < 1;
             curvatureThreshold += 0.01) {
          std::list<TestSet::Descriptor> descriptors;
          ReadDescriptors("wc_images/description.txt", &descriptors);

          std::vector< std::vector<size_t> > errors;
          std::vector<size_t> row1(2, 0), row2(2, 0);
          errors.push_back(row1);
          errors.push_back(row2);

          for (std::list<TestSet::Descriptor>::const_iterator
                 descriptor = descriptors.begin();
               descriptor != descriptors.end(); ++descriptor) {
            cv::Mat wcImage = cv::imread(descriptor->imagePath.c_str());
            std::list<PolypDetector::Descriptor> polyps =
              PolypDetector::Locate(wcImage,
                                    smoothValue,
                                    derivativeSmoothValue, curvatureThreshold);

            PolypDetector::GetError(*descriptor, polyps, &errors);
          }

          std::printf("Er: %lf sv: %lf dsv: %lf ct: %lf)\n",
                      TestSet::GetError(errors), smoothValue,
                      derivativeSmoothValue, curvatureThreshold);
        }
      }
    }
  } else {
    std::list<TestSet::Descriptor> descriptors;
    ReadDescriptors("wc_images/description.txt", &descriptors);

    std::vector< std::vector<size_t> > errors;
    std::vector<size_t> row1(2, 0), row2(2, 0);
    errors.push_back(row1);
    errors.push_back(row2);

    std::list< TestSet::Descriptor > alphaErrors;
    std::list< TestSet::Descriptor > betaErrors;

    for (std::list<TestSet::Descriptor>::const_iterator
           descriptor = descriptors.begin();
         descriptor != descriptors.end(); ++descriptor) {
      // std::printf("%s\n", descriptor->imagePath.c_str());
      cv::Mat wcImage = cv::imread(descriptor->imagePath.c_str());
      std::list<PolypDetector::Descriptor> polyps =
        PolypDetector::Locate(wcImage);

      PolypDetector::GetError(*descriptor, polyps, &errors,
                              &alphaErrors, &betaErrors);
    }

    // print header
    std::printf("\t%s\t%s\n", "ep imho", "no imho");
    for (size_t x = 0; x < 2; ++x) {
      if (x == 0) {
        std::printf("%s\t", "ep");
      } else if (x == 1) {
        std::printf("%s\t", "no");
      }
      for (size_t y = 0; y < 2; ++y) {
        std::printf("%lu\t", errors[x][y]);
      } std::printf("\n");
    }
    std::printf("Er: %lf\n", TestSet::GetError(errors));

    std::printf("%s\n", "Alpha errors");
    for (std::list< TestSet::Descriptor >::const_iterator
           descriptor = alphaErrors.begin();
         descriptor != alphaErrors.end(); ++descriptor) {
      std::printf("%s\n", descriptor->imagePath.c_str());
    }
    std::printf("%s\n", "Beta errors");
    for (std::list< TestSet::Descriptor >::const_iterator
           descriptor = betaErrors.begin();
         descriptor != betaErrors.end(); ++descriptor) {
      std::printf("%s\n", descriptor->imagePath.c_str());
    }
  }
  return 0;
}
