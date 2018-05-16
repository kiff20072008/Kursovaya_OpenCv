#ifndef _BALL_H_
#define _BALL_H_
#include "Definitions.h"

class Ball
{
public:
	Ball(const qreal width, const qreal height, QGraphicsPixmapItem * item) :ball_img_(item), screen_width_(width), screen_height_(height)
	{
		resetBallPos();
	}
	inline bool isBallCollideWithItem(const QGraphicsPixmapItem * item);
	inline void resetBallPos();
	inline int getPixmapWidth()const;
	inline int getPixmapHeight()const;
	inline qreal getX()const;
	inline qreal getY()const;
	inline int moveBall();
	inline void changeDirX();
	inline void changeDirY();
	inline void moveUp(int y);
	inline void setBallSpeed(int speed);
	inline int getBallSpeed();
private:
	inline int check_coord();

	QGraphicsPixmapItem * ball_img_;
	int screen_width_, screen_height_;
	int dir_up_, dir_left_;
	size_t ball_speed_;
};

inline int Ball::getBallSpeed()
{
	return ball_speed_;
}
inline void Ball::setBallSpeed(int speed)
{
	ball_speed_ = 10-speed + 3;
}

inline void Ball::changeDirY()
{
	dir_up_ *= -1;
}

inline void Ball::changeDirX()
{
	dir_left_ *= -1;
}

inline int Ball::check_coord()
{
	if (ball_img_->x() <= 0 || ball_img_->x() + ball_img_->pixmap().width() >= screen_width_)
	{
		changeDirX();
	}
	if (ball_img_->y() <= 0)
	{
		changeDirY();
	}
	if (ball_img_->y() + ball_img_->pixmap().height() >= screen_height_)
	{
		return 1;
	}
	return 0;
}

inline bool Ball::isBallCollideWithItem(const QGraphicsPixmapItem * item)
{
	return ball_img_->collidesWithItem(item);
}

inline int Ball::getPixmapWidth() const
{
	return ball_img_->pixmap().width();
}

inline int Ball::getPixmapHeight() const
{
	return ball_img_->pixmap().height();
}

inline qreal Ball::getX() const
{
	return ball_img_->x();
}

inline qreal Ball::getY() const
{
	return ball_img_->y();
}

inline void Ball::moveUp(int y)
{
	ball_img_->setPos(ball_img_->x(), ball_img_->y() + y);
}

inline int Ball::moveBall()
{
	if (check_coord())
	{
		return 1;
	}
	ball_img_->setPos(ball_img_->x() + dir_left_, ball_img_->y() + dir_up_);
	return 0;
}

inline void Ball::resetBallPos()
{
	ball_speed_ = 5;
	dir_up_ = -1;
	dir_left_ = -1;
	ball_img_->setPos((screen_width_ - ball_img_->pixmap().width()) / 2, screen_height_ - 95);
}


#endif // !_BALL_H_
