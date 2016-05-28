/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <math.h>
#include <QtGui>
#include <QtOpenGL>
#include<QDebug>
#include<QTime>
#include<QTimer>
#include<windows.h>
#include<QThread>
#include<GL/glu.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<QtCore>
#include <QKeyEvent>
#include <QMessageBox>
#include <QWheelEvent>
//#include "win_qextserialport.h"
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    void initVariable();
    void findCom();
    void getRandXY();
    void writeToPort(uint);
    QLabel lab;
    ~GLWidget();

signals:
    void sigInputDisplayLed(uint);
    void sigOutputDisplayLed(uint);
    void sigFullScreen();
    void sigQuitFullScreen();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:
    void advanceGears();
    void slotKeyPressEvent(QString key);



private:
    GLuint makeGear(const GLfloat *reflectance, GLdouble innerRadius,
                    GLdouble outerRadius, GLdouble thickness,
                    GLdouble toothSize, GLint toothCount);
    void drawGear(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
                  GLdouble angle);
    void drawGear2(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz);
    void drawGear3(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz);
    void drawGear4(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz);
    void drawGear5(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz);
    void drawGear6(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz);
    void normalizeAngle(int *angle);
    void spinDisplay();

    GLuint gear1;
    GLuint gear2;
    GLuint gear3;
    GLuint gear4;
    GLuint gear5;
    GLuint gear6;
    GLuint gear7;
    GLuint gear8;
    GLuint gear9;
    GLuint gear10;//黑色的试管
    GLuint gear11;


    int xRot;
    int yRot;
    int zRot;
    GLdouble gear1Rot;
    double currentRot;//记录黑色试管所在位置（角度）
    QPoint lastPos;


    QSerialPort com;
   //Win_QextSerialPort com;
    QString strComName;
    QByteArray qbya;
    qint8 outChar;
    uint readPortData[20];//保存串口的发来的数据
    uint readLastData;
    uint writePortData;//向串口写出的数据，用于UI指示灯亮灭
    int j,countQbyte;
    bool flagPortData;
    bool flagPos;
    bool flagUp;
    bool flagDown;

    QString strToHex;
    double upDown;
    QChar ch;

    GLfloat translate;
    GLdouble randX;//随机产生的黑色试管X坐标
    GLdouble randY;//随机产生的黑色试管Y坐标

};

#endif
