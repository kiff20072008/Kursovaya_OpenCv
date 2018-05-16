#include "Arkanoid.h"
#include "Definitions.h"

using namespace cv;

const std::string WindowName = "Face Detection";

Arkanoid::Arkanoid(const int width, const int height, QWidget *parent) :ball_(width, height, addPixmap(QPixmap(":/Kursovaya_OpenCv/ball.png"))), is_game_over(false), window_width_(width), QGraphicsScene(parent)
{
	setupUi(width, height);
	setupFaceDetector();
}

void Arkanoid::setupUi(const int width, const int height)
{
	
	setSceneRect(0, 0, width, height);
	setBackgroundBrush(QPixmap(":/Kursovaya_OpenCv/fon_5.png"));

	ball_timer_ = new QTimer(this);
	connect(ball_timer_, SIGNAL(timeout()), SLOT(moveBall()));

	platform_img_ = addPixmap(QPixmap(":/Kursovaya_OpenCv/platform.png"));
	platform_img_->setPos(static_cast<qreal>((width - platform_img_->pixmap().width()) / 2), static_cast<qreal>(height - platform_img_->pixmap().height() - 5));

	lifes_.arr_of_lifes_.reserve(NUMOFLIFES);
	lifes_.num_of_lifes = NUMOFLIFES;
	for (int i = 0; i < NUMOFLIFES; ++i)
	{
		lifes_.arr_of_lifes_.push_back(addPixmap(QPixmap(":/Kursovaya_OpenCv/life.png")));
		lifes_.arr_of_lifes_[i]->setVisible(true);
		lifes_.arr_of_lifes_[i]->setPos(static_cast<qreal>((lifes_.arr_of_lifes_[i]->pixmap().width() + 5)*i), qreal(5));
	}

	blocks_.arr_of_blocks_.reserve(NUMOFBLOCKSX*NUMOFBLOCKSY);
	blocks_.num_of_blocks_ = NUMOFBLOCKSX * NUMOFBLOCKSY;
	for (int i = 0; i < NUMOFBLOCKSX*NUMOFBLOCKSY; ++i)
	{
		blocks_.arr_of_blocks_.push_back(addPixmap(QPixmap(":/Kursovaya_OpenCv/block.png")));
		blocks_.arr_of_blocks_[i]->setVisible(true);
		blocks_.arr_of_blocks_[i]->setPos((blocks_.arr_of_blocks_[i]->pixmap().width() + 5)*(i%NUMOFBLOCKSX) + 10, (blocks_.arr_of_blocks_[i]->pixmap().height() + 5)*(i / NUMOFBLOCKSX) + 30);
	}

	slider_ptr_=this->addWidget(&slider_);
	slider_ptr_->setPos(width - 10, 20);
	slider_.setMinimum(0);
	slider_.setMaximum(10);
	slider_.setSliderPosition(2);
	connect(&slider_, SIGNAL(valueChanged(int)), this, SLOT(setBallSpeed(int)));
	label = this->addText(getStrScore());
	label->setPos(width - 100, 5);
}

QString Arkanoid::getStrScore()
{
	char buffer[3];
	return ("Your Score is " + QString(_itoa(NUMOFBLOCKSX*NUMOFBLOCKSY - blocks_.num_of_blocks_, buffer, 10)));
}


void Arkanoid::setupFaceDetector()
{
	namedWindow(WindowName);
	VideoStream = new VideoCapture(0);
	if (!(*VideoStream).isOpened())
	{
		return;
	}
	cascade = makePtr<cv::CascadeClassifier>("lbpcascade_frontalface.xml");
	MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
	if (cascade->empty())
	{
		return;
	}
	cascade = makePtr<cv::CascadeClassifier>("lbpcascade_frontalface.xml");
	TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
	if (cascade->empty())
	{
		return;
	}
	Detector = new DetectionBasedTracker(MainDetector, TrackingDetector, params);
	temp_rect.x = temp_rect.y = 0;
}

void Arkanoid::startFaceDetector()
{
	do
	{
		(*VideoStream) >> ReferenceFrame;
		cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
		(*Detector).process(GrayFrame);
		(*Detector).getObjects(Faces);
		if (Faces.size())
		{
			if (abs(Faces[0].x + Faces[0].width / 2 - temp_rect.x) > 10 || abs(Faces[0].y + Faces[0].height / 2 - temp_rect.y) > 10)
			{
				movePlatform(Faces[0].x - 20, ReferenceFrame.size().width - 100);

				temp_rect.x = Faces[0].x + Faces[0].width / 2;
				temp_rect.y = Faces[0].y + Faces[0].height / 2;
			}
		}
		for (size_t i = 0; i < Faces.size(); i++)
		{
			rectangle(ReferenceFrame, Faces[i], Scalar(0, 255, 0));
		}
		imshow(WindowName, ReferenceFrame);
	} while (waitKey(100) < 0 && !is_game_over);
	ball_timer_->stop();
	destroyAllWindows();
	(*Detector).stop();
}

void Arkanoid::start()
{
	ball_timer_->start(ball_.getBallSpeed());
	startFaceDetector();
}

void Arkanoid::changeBallDir(const QGraphicsPixmapItem* obj)
{
	if (ball_.getX() + ball_.getPixmapWidth() / 2 > obj->x() && ball_.getX() + ball_.getPixmapWidth() / 2 < obj->x() + obj->pixmap().width())
	{
		ball_.changeDirY();
	}
	else if (ball_.getX() + ball_.getPixmapWidth() == obj->x() || ball_.getX() == obj->x() + obj->pixmap().width())
	{
		ball_.changeDirX();
	}
	else
	{
		ball_.changeDirY();
		ball_.changeDirX();
	}
}

void Arkanoid::setBallSpeed(int speed)
{
	ball_.setBallSpeed(speed);
	ball_timer_->setInterval(ball_.getBallSpeed());
}


void Arkanoid::moveBall()
{
	if (ball_.isBallCollideWithItem(platform_img_))
	{
		changeBallDir(platform_img_);
	}
	for (int i = 0; i < NUMOFBLOCKSX*NUMOFBLOCKSY; ++i)
	{
		if (ball_.isBallCollideWithItem(blocks_.arr_of_blocks_[i]))
		{
			changeBallDir(blocks_.arr_of_blocks_[i]);
			blocks_.arr_of_blocks_[i]->setVisible(false);
			blocks_.arr_of_blocks_[i]->setPos(-100, -100);
			--blocks_.num_of_blocks_;
			label->setPlainText(getStrScore());
			if (!blocks_.num_of_blocks_)
			{
				gameOver();
			}
		}
	}
	if (ball_.moveBall())
	{
		changeNumLifes();
		ball_.resetBallPos();
	}
}
void Arkanoid::movePlatform(const int pos, const int widthWindow)
{
	platform_img_->setPos(WIDTH - 150 - pos * window_width_ / widthWindow, platform_img_->y());
	while (ball_.isBallCollideWithItem(platform_img_))
	{
		ball_.moveUp(1);
	}
}

void Arkanoid::changeNumLifes()
{
	if (!lifes_.num_of_lifes)
	{
		gameOver();
	}
	else
	{
		lifes_.arr_of_lifes_.at(--lifes_.num_of_lifes)->setVisible(false);
	}
}

void Arkanoid::gameOver()
{
	ball_timer_->stop();
	bool n = static_cast<bool>(QMessageBox::warning(0,
		"Warning",
		"Game over!,"
		"\n Do you want to restart?",
		"Yes",
		"No",
		QString(),
		false,
		true
	));
	if (!n) {
		restart();
	}
	else
	{
		is_game_over = true;
	}
}

void Arkanoid::restart()
{
	ball_.resetBallPos();
	lifes_.num_of_lifes = NUMOFLIFES;
	std::for_each(lifes_.arr_of_lifes_.begin(), lifes_.arr_of_lifes_.end(), [](QGraphicsPixmapItem* item) {item->setVisible(true); });
	ball_timer_->start(ball_.getBallSpeed());
	blocks_.num_of_blocks_ = NUMOFBLOCKSX * NUMOFBLOCKSY;
	for (int i = 0; i < NUMOFBLOCKSX*NUMOFBLOCKSY; ++i)
	{
		blocks_.arr_of_blocks_[i]->setVisible(true);
		blocks_.arr_of_blocks_[i]->setPos((blocks_.arr_of_blocks_[i]->pixmap().width() + 5)*(i%NUMOFBLOCKSX) + 10, (blocks_.arr_of_blocks_[i]->pixmap().height() + 5)*(i / NUMOFBLOCKSX) + 30);
	}
}