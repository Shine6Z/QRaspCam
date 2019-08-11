#include "camera.h"

void Camera::start(){
  if (camera->open()){
    data = new unsigned char[  camera->getImageTypeSize ( color )];
    grab();
  }
}

void Camera::grab(){
    while(true){
      camera->grab();
      camera->retrieve(data, color);
      emit(newImage(data));
    }
}

Camera::Camera(QSize resolution, bool isColored) : QObject(){
  if (isColored){
    color = raspicam::RASPICAM_FORMAT_RGB;
  }
  else{
    color = raspicam::RASPICAM_FORMAT_GRAY;
  }
  camera = new raspicam::RaspiCam;
  camera->setFormat(color);
  camera->setWidth(resolution.width());
  camera->setHeight(resolution.height());
  camera->setSensorMode(raspicam::RASPICAM_EXPOSURE_AUTO);
  exposureCompensation = 0;
  camera->setExposureCompensation(exposureCompensation);
  camera->setImageEffect(raspicam::RASPICAM_IMAGE_EFFECT_NONE);
  if(resolution.width() >= 1500){
    camera->setFrameRate(10);
  }
  else if(resolution.width() < 1500 && resolution.width() > 1000){
    camera->setFrameRate(15);
  }
  else if(resolution.width() <= 1000 ){
    camera->setFrameRate(60);
  }
}

Camera::~Camera(){
  camera->release();
  emit(finished());
}
    
void Camera::setBrightness(int brightness){
  camera->setBrightness(brightness);
}

int Camera::getBrightness(){
  int brightness = camera->getBrightness();
  return brightness;
}
    
void Camera::setContrast(int contrast){
  camera->setContrast(contrast);
}

int Camera::getContrast(){
  int contrast = camera->getContrast();
  return contrast;
}
    
void Camera::setSharpness(int sharpness){
  camera->setSharpness(sharpness);
}

int Camera::getSharpness(){
  int sharpness = camera->getSharpness();
  return sharpness;
}

void Camera::setISO(int ISO){
  camera->setISO(ISO);
}

int Camera::getISO(){
  int ISO = camera->getISO();
  return ISO;
}

void Camera::setSaturation(int saturation){
  camera->setSaturation(saturation);
}

int Camera::getSaturation(){
  int saturation = camera->getSaturation();
  return saturation;
}

void Camera::setExposureMode(int mode){
  camera->setExposure(raspicam::RASPICAM_EXPOSURE(mode));
}

void Camera::setExposure(int value){
  camera->setExposureCompensation(value);
}

int Camera::getExposure(){
  return exposureCompensation;
}
