/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <opencv2/opencv.hpp>
#include <future>

using namespace cv;
using namespace std;

class CameraWrapper
{
public:
    CameraWrapper();
    ~CameraWrapper();

    void AcquireImage();

    const Mat& GetImage() const { return m_imageHSV; }

	/// Save an image file on a background thread
	template <class Task>
	void SaveFileInBackground(Task& writeTask, const std::string& fileName, const Mat& matrix);

private:
    VideoCapture m_Camera = VideoCapture(0);
	Mat m_imageHSV;												//!< Converted input image BGR->HSV

	using WriteFuture = future<void>;
	WriteFuture	m_imageWriteTask;
};
#endif  // CAMERAWRAPPER_H
