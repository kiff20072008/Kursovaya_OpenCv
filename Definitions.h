#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <opencv2/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>

#include <QGraphicsScene>
#include <QMainWindow>
#include <QtWidgets>

#define HEIGHT 700
#define WIDTH 500
#define NUMOFBLOCKSX 6 //num of blocks for gorizontal
#define NUMOFBLOCKSY 4 // num of blocks to vertikal
#define NUMOFLIFES 3 

#endif // !DEFINITIONS_H_
