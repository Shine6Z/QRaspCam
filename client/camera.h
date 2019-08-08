#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QTimer>
#include <QSize>
#include <QDebug>
#include <raspicam/raspicam.h>
#include <unistd.h>

class Camera : public QObject{
  Q_OBJECT

  public:
    explicit Camera(QSize resolution = QSize(1920, 1050));
    ~Camera();
    void grab();

  public slots:
    void start();

  private:
    raspicam::RaspiCam* camera;
    unsigned char *data;

  signals:
  void newImage(unsigned char *data);
  void finished();
};

#endif
