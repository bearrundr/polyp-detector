#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <opencv/cv.h>

/*!
 \brief Shows image and waits till user presses
 any key.
 \param image Image to show.
 \param title Window title.
*/
void ShowImage(const cv::Mat &image,
               const std::string title = "Show Image");

/*!
 \brief Prints text on image in top left corner.
 \param text Text to print.
 \param image Pointer to image to print on.
*/
void PrintText(const std::string text, cv::Mat *image);

/*!
 \brief Returns smallest and biggest element of matrix.
 \param matrix Matrix to search in.
 \returns Pair where first element is minimum and second
 is maximum.
*/
template<typename T>
std::pair<T, T> GetMinMax(const cv::Mat &matrix);

/*!
 \brief Shows image as a heat map and waits till user presses
 any key.
 \param image Image to show.
 \param title Window title.
*/
template<typename T>
void ShowHeatmap(const cv::Mat &image,
                 const std::string title = "Show Heatmap");

/*!
 \brief Print to stdout content of matrix.
 \param matrix Matrix to print.
*/
template<typename T>
void PrintMatrix(const cv::Mat &matrix);


#endif /* _TOOLS_H_ */
