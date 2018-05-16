#ifndef _ARKANOID_H_
#define _ARKANOID_H_

#include "ball.h"
#include "Definitions.h"

class CascadeDetectorAdapter : public cv::DetectionBasedTracker::IDetector
{
public:
	CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector) :
		IDetector(),
		Detector(detector)
	{
		CV_Assert(detector);
	}

	void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects)
	{
		Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
	}

	virtual ~CascadeDetectorAdapter()
	{}

private:
	CascadeDetectorAdapter();
	cv::Ptr<cv::CascadeClassifier> Detector;
};


class Arkanoid : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit Arkanoid(const int width, const int height, QWidget *parent = 0);
	void start();
private:
	struct Life_t
	{
		QVector<QGraphicsPixmapItem*> arr_of_lifes_;
		int num_of_lifes;
	};
	struct Blocks_t
	{
		QVector<QGraphicsPixmapItem*> arr_of_blocks_;
		int num_of_blocks_;
	};
	void changeNumLifes();
	void changeBallDir(const QGraphicsPixmapItem* obj);
	void gameOver();
	void movePlatform(const int pos, const int widthWindow);
	void restart();
	void startFaceDetector();
	void setupFaceDetector();
	void setupUi(const int width, const int height);
	
	QString getStrScore();

	Ball ball_;
	Blocks_t blocks_;
	QTimer* ball_timer_;
	bool is_game_over;
	Life_t lifes_;
	QGraphicsPixmapItem * platform_img_;
	size_t Score_;
	QSlider slider_;
	QGraphicsProxyWidget * slider_ptr_;
	int window_width_;
	QGraphicsTextItem* label;
	int y_coord_lowest_prep;

	cv::VideoCapture * VideoStream;
	cv::Ptr<cv::CascadeClassifier> cascade;
	cv::Ptr<cv::DetectionBasedTracker::IDetector> MainDetector;
	cv::Ptr<cv::DetectionBasedTracker::IDetector> TrackingDetector;
	cv::DetectionBasedTracker::Parameters params;
	cv::DetectionBasedTracker *Detector;
	cv::Mat ReferenceFrame;
	cv::Mat GrayFrame;
	std::vector<cv::Rect> Faces;
	cv::Rect temp_rect;

private slots:
	void moveBall();
	void setBallSpeed(int speed);
};


#endif // !_ARKANOID_H_
