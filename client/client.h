#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QFrame>
#include <QThread>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QSize>
#include <QAction>
#include <QKeySequence>
#include <QScrollArea>
#include <QPushButton>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QDesktopWidget>

#include "camera.h"

class Client : public QMainWindow{
  Q_OBJECT
  public:
    explicit Client(QWidget *parent = 0);

  public slots:
void refreshDisplay(unsigned char *data);
void newCamera();
void refreshParameters();

  private:

    QSize screenSize;
    QFrame* ui_control;
    QScrollArea* ui_scroll;
    QLabel* ui_display;
    QMenuBar* menu;
    QPushButton* ui_button;
    Camera* camera;
    QSize resolution;
    QThread* cameraThread;
};

#endif
