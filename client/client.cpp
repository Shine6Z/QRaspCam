#include "client.h"


Client::Client(QWidget *parent) : QMainWindow(parent){


  // Window
  ui_display = new QLabel(this);
  ui_display->setText("test");
  this->setCentralWidget(ui_display);

resolution = QSize(1920, 1050);

  // Menu
  menu = menuBar();
  QMenu *resolutionMenu = new QMenu("Resolutions");

  QAction* resUltra = new QAction("Ultra");
  QAction* res1080 = new QAction("1080p");
  QAction* res720 = new QAction("720p");
  QAction* resLow = new QAction("Low");

  resUltra->setCheckable(true);
  res1080->setCheckable(true);
  res720->setCheckable(true);
  resLow->setCheckable(true);
  res1080->setChecked(true);

  connect(resUltra, &QAction::triggered, [this, resUltra, res1080, res720, resLow](){
    resolution = QSize(2592, 1944);
    res1080->setChecked(false);
    res720->setChecked(false);
    resLow->setChecked(false);
    refreshParameters();
  });
  connect(res1080, &QAction::triggered, [this, resUltra, res1080, res720, resLow](){
    resolution = QSize(1920, 1080);
    resUltra->setChecked(false);
    res720->setChecked(false);
    resLow->setChecked(false);
    refreshParameters();
  });
  connect(res720, &QAction::triggered, [this, resUltra, res1080, res720, resLow](){
    resolution = QSize(1280, 720);
    res1080->setChecked(false);
    resUltra->setChecked(false);
    resLow->setChecked(false);
    refreshParameters();
  });
  connect(resLow, &QAction::triggered, [this, resUltra, res1080, res720, resLow](){
    resolution = QSize(640, 480);
    refreshParameters();
    res1080->setChecked(false);
    res720->setChecked(false);
    resUltra->setChecked(false);
  });

  resolutionMenu->addAction(resUltra);
  resolutionMenu->addAction(res1080);
  resolutionMenu->addAction(res720);
  resolutionMenu->addAction(resLow);

  menu->addMenu(resolutionMenu);


  QMenu *settingsMenu = new QMenu("Settings");

  QAction* brightnessPlus = new QAction("More bright");
  QAction* brightnessMinus = new QAction("Less bright");
  brightnessPlus->setShortcut(QKeySequence(tr("Ctrl+Shift+B")));
  brightnessMinus->setShortcut(QKeySequence(tr("Ctrl+b")));
  connect(brightnessPlus, &QAction::triggered, [this](){
    camera->setBrightness(camera->getBrightness() + 1);
  });
  connect(brightnessMinus, &QAction::triggered, [this](){
    camera->setBrightness(camera->getBrightness() - 1);
  });
  settingsMenu->addAction(brightnessPlus);
  settingsMenu->addAction(brightnessMinus);
  settingsMenu->addSeparator();

  QAction* contrastPlus = new QAction("More contrast");
  QAction* contrastMinus = new QAction("Less contrast");
  contrastPlus->setShortcut(QKeySequence(tr("Ctrl+Shift+C")));
  contrastMinus->setShortcut(QKeySequence(tr("Ctrl+c")));
  connect(contrastPlus, &QAction::triggered, [this](){
    camera->setContrast(camera->getContrast() + 1);
  });
  connect(contrastMinus, &QAction::triggered, [this](){
    camera->setContrast(camera->getContrast() - 1);
  });
  settingsMenu->addAction(contrastPlus);
  settingsMenu->addAction(contrastMinus);
  settingsMenu->addSeparator();

  menu->addMenu(settingsMenu);

  newCamera();

  ui_control = new QFrame(ui_display);
  ui_control->move(25, 50);
  ui_button = new QPushButton(tr(""), ui_control);


}

void Client::newCamera(){ 

  cameraThread = new QThread;
  camera = new Camera(resolution);
  camera->moveToThread(cameraThread);
  connect(cameraThread, SIGNAL(started()), camera, SLOT(start()));
  connect(camera, &Camera::newImage, this, &Client::refreshDisplay);
  connect(camera, SIGNAL(finished()), cameraThread, SLOT(quit()));
  connect(camera, SIGNAL(finished()), camera, SLOT(deleteLater()));
  connect(cameraThread, SIGNAL(finished()), cameraThread, SLOT(deleteLater()));
  cameraThread->start();
}

void Client::refreshParameters(){
  delete camera;
  QThread::sleep(3);
  newCamera();
  QTimer::singleShot(1000, this, &QMainWindow::adjustSize);
}

void Client::refreshDisplay(unsigned char *data){

QPixmap image = QPixmap::fromImage(QImage(data, resolution.width(), resolution.height(), QImage::Format_Grayscale8));
ui_display->setPixmap(image);

}
