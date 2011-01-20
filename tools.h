#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <opencv/highgui.h>

/*!
 \brief Shows image and waits till user presses
 any key.
 \param image Image to show.
*/
void ShowImage(const cv::Mat &image);

/*!
 \brief Print to stdout content of matrix.
 \param matrix Matrix to print.
*/
template<typename T>
void PrintMatrix(const cv::Mat &matrix);


#endif /* _TOOLS_H_ */
