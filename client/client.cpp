#include "client.h"


Client::Client(QWidget *parent) : QMainWindow(parent){
  QRect screen = QDesktopWidget().availableGeometry(this);
  screenSize = QSize(screen.width(), screen.height());
  this->setMaximumSize(screenSize);

  // Window
  ui_scroll = new QScrollArea(this);
  ui_display = new QLabel;
  ui_scroll->setWidget(ui_display);
  this->setCentralWidget(ui_scroll);

  resolution = QSize(1920, 1050);
  ui_display->setMinimumSize(resolution);
  isColored = false;

  // Menu
  menu = menuBar();
  QMenu *resolutionMenu = new QMenu("Resolutions");

  QAction* resUltra = new QAction("Ultra");
  QAction* res1080 = new QAction("1080p");
  QAction* res720 = new QAction("720p");
  QAction* resLow = new QAction("Low");
  QAction* colored = new QAction("Colored");

  resUltra->setCheckable(true);
  res1080->setCheckable(true);
  res720->setCheckable(true);
  resLow->setCheckable(true);
  res1080->setChecked(true);
  colored->setCheckable(true);
  colored->setChecked(false);

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
  connect(colored, &QAction::triggered, [this](bool isChecked){
    isColored = isChecked;
    refreshParameters();
  });

  resolutionMenu->addAction(resUltra);
  resolutionMenu->addAction(res1080);
  resolutionMenu->addAction(res720);
  resolutionMenu->addAction(resLow);
  resolutionMenu->addAction(colored);

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

  QAction* sharpnessPlus = new QAction("More sharpness");
  QAction* sharpnessMinus = new QAction("Less sharpness");
  sharpnessPlus->setShortcut(QKeySequence(tr("Ctrl+Shift+S")));
  sharpnessMinus->setShortcut(QKeySequence(tr("Ctrl+s")));
  connect(sharpnessPlus, &QAction::triggered, [this](){
    camera->setSharpness(camera->getSharpness() + 1);
  });
  connect(sharpnessMinus, &QAction::triggered, [this](){
    camera->setSharpness(camera->getSharpness() - 1);
  });
  settingsMenu->addAction(sharpnessPlus);
  settingsMenu->addAction(sharpnessMinus);
  settingsMenu->addSeparator();

  QAction* ISOPlus = new QAction("More ISO");
  QAction* ISOMinus = new QAction("Less ISO");
  ISOPlus->setShortcut(QKeySequence(tr("Ctrl+Shift+I")));
  ISOMinus->setShortcut(QKeySequence(tr("Ctrl+i")));
  connect(ISOPlus, &QAction::triggered, [this](){
    camera->setISO(camera->getISO() + 1);
  });
  connect(ISOMinus, &QAction::triggered, [this](){
    camera->setISO(camera->getISO() - 1);
  });
  settingsMenu->addAction(ISOPlus);
  settingsMenu->addAction(ISOMinus);
  settingsMenu->addSeparator();

  QAction* saturationPlus = new QAction("More saturation");
  QAction* saturationMinus = new QAction("Less saturation");
  saturationPlus->setShortcut(QKeySequence(tr("Ctrl+Shift+A")));
  saturationMinus->setShortcut(QKeySequence(tr("Ctrl+a")));
  connect(saturationPlus, &QAction::triggered, [this](){
    camera->setSaturation(camera->getSaturation() + 1);
  });
  connect(saturationMinus, &QAction::triggered, [this](){
    camera->setSaturation(camera->getSaturation() - 1);
  });
  settingsMenu->addAction(saturationPlus);
  settingsMenu->addAction(saturationMinus);
  settingsMenu->addSeparator();
  menu->addMenu(settingsMenu);

  QMenu *exposureMenu = new QMenu("Exposure");
  QStringList exposureMode = { "Off", "Auto", "Night", "Night Preview", "Backlight", "Spotlight", "Sport", "Snow", "Beach", "Very Long", "Fixed FPS", "Antishake", "Fireworks" };
  menu->addMenu(exposureMenu);
  for(int i = 0; i < exposureMode.length(); i++){
    QAction* action = new QAction(exposureMode[i]);
    action->setCheckable(true);
    connect(action, &QAction::triggered, [this, exposureMenu, action, exposureMode, i](){
      camera->setExposureMode(i);
      foreach (QAction* act, exposureMenu->actions()){
        act->setChecked(false);
      }
      action->setChecked(true);
    });
    exposureMenu->addAction(action);
  }
  exposureMenu->actions()[1]->setChecked(true);
  exposureMenu->addSeparator();
  QAction* expPlus = new QAction("More exposure compensation");
  QAction* expMinus = new QAction("Less exposureCompensation");
  expPlus->setShortcut(QKeySequence(tr("Ctrl+Shift+E")));
  expMinus->setShortcut(QKeySequence(tr("Ctrl+E")));
  connect(expPlus, &QAction::triggered, [this](){
    camera->setExposure(camera->getExposure() + 1);
  });
  connect(expMinus, &QAction::triggered, [this](){
    camera->setExposure(camera->getExposure() - 1);
  });
  exposureMenu->addAction(expPlus);
  exposureMenu->addAction(expMinus);



  QMenu *actionsMenu = new QMenu("Actions");

  QAction* takeScreenshot = new QAction("Screenshot");
  takeScreenshot->setShortcut(QKeySequence(tr("s")));
  connect(takeScreenshot, &QAction::triggered, this, &Client::screenshot);
  actionsMenu->addAction(takeScreenshot);

  record = false;
  QAction* recordAction = new QAction("Record");
  recordAction->setShortcut(QKeySequence(tr("Shift+r")));
  recordAction->setCheckable(true);
  connect(recordAction, &QAction::triggered, [this](bool state){
    this->record = state;
    if(this->record){
      QDateTime date = QDateTime::currentDateTime();
       this->savePath = QDir::homePath() + "/Videos/" + date.toString("dd_mm_yyyy") + "/" + date.toString("hh:mm:ss:zzz");
      if(!QDir(this->savePath).exists()) {QDir().mkpath(this->savePath);}
      this->writer = new QImageWriter();
    }
    else {
      delete writer;
    }
    imageNumber = 0;
  });
  actionsMenu->addAction(recordAction);
  menu->addMenu(actionsMenu);

  newCamera();

  //ui_control = new QFrame(ui_display);
  //ui_control->move(25, 50);
  //ui_button = new QPushButton(tr(""), ui_control);


}

void Client::newCamera(){ 

  cameraThread = new QThread;
  camera = new Camera(resolution, isColored);
  camera->moveToThread(cameraThread);
  connect(cameraThread, SIGNAL(started()), camera, SLOT(start()));
  connect(camera, &Camera::newImage, this, &Client::refreshDisplay);
  connect(camera, SIGNAL(finished()), cameraThread, SLOT(quit()));
  connect(camera, SIGNAL(finished()), camera, SLOT(deleteLater()));
  connect(cameraThread, SIGNAL(finished()), cameraThread, SLOT(deleteLater()));
  cameraThread->start();
}

void Client::refreshParameters(){
  disconnect(camera, &Camera::newImage, this, &Client::refreshDisplay);
  delete camera;
  //QThread::sleep(10);
  newCamera();
  ui_display->setMinimumSize(resolution);
  ui_display->setMaximumSize(resolution);
  if (screenSize.width() < resolution.width() || screenSize.height() < resolution.height() ){
    ui_scroll->setMaximumSize(screenSize);
    this->setMinimumSize(screenSize);
  }
  else {
    ui_scroll->setMaximumSize(resolution);
    this->setMinimumSize(resolution);
  }
  this->adjustSize();
  this->setWindowState(Qt::WindowMinimized|Qt::WindowActive);
}

void Client::refreshDisplay(unsigned char *data){

QPixmap image;
if(isColored){
  image = QPixmap::fromImage(QImage(data, resolution.width(), resolution.height(), QImage::Format_RGB888));
}
else{
  image = QPixmap::fromImage(QImage(data, resolution.width(), resolution.height(), QImage::Format_Grayscale8));
}
ui_display->setPixmap(image);

if(record){
  writer->setFileName(savePath + "/frame" + QString("%1").arg(imageNumber, 6, 10, QChar('0')) + ".png");
  writer->write(image.toImage());
  imageNumber++;
  qInfo() << imageNumber << savePath;
}
}

void Client::screenshot(){
  QString date = QDateTime::currentDateTime().toString("dd:MM:yyyy:hh:mm:ss:zzz");
  QString path = QDir::homePath() + "/Pictures/" + date + ".png";
  QImageWriter writer(path);
  writer.write(ui_display->pixmap()->toImage());

}
