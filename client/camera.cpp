#include "camera.h"

void Camera::start(){
  if (camera->open()){
    data=new unsigned char[  camera->getImageTypeSize ( raspicam::RASPICAM_FORMAT_GRAY )];
    grab();
  }
}

void Camera::grab(){
    camera->grab();
    camera->retrieve(data, raspicam::RASPICAM_FORMAT_GRAY);
    emit(newImage(data));
    QTimer::singleShot(0, this, &Camera::grab);

}

Camera::Camera(QSize resolution) : QObject(){
  camera = new raspicam::RaspiCam;
  camera->setFormat(raspicam::RASPICAM_FORMAT_GRAY);
  camera->setWidth(resolution.width());
  camera->setHeight(resolution.height());
  camera->setSensorMode(raspicam::RASPICAM_EXPOSURE_AUTO);
  camera->setImageEffect(raspicam::RASPICAM_IMAGE_EFFECT_NONE);
}

Camera::~Camera(){
  camera->release();
  emit(finished());
}
