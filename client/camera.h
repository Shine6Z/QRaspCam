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
    explicit Camera(QSize resolution = QSize(1920, 1050), bool isColored=false);
    ~Camera();
    void grab();

  public slots:
    void start();
    void setBrightness(int brightness);
    int getBrightness();
    void setContrast(int contrast);
    int getContrast();
    void setSharpness(int sharpness);
    int getSharpness();
    void setISO(int ISO);
    int getISO();
    void setSaturation(int saturation);
    int getSaturation();
    void setExposureMode(int mode);
    void setExposure(int value);
    int getExposure();

  private:
    raspicam::RaspiCam* camera;
    raspicam::RASPICAM_FORMAT color;
    unsigned char *data;
    int exposureCompensation;

  signals:
  void newImage(unsigned char *data);
  void finished();
};

#endif
