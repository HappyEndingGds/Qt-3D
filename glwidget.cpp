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


#include "glwidget.h"

static GLfloat spin=-35.0;
static GLint status=1;

const GLdouble ArryInner[50][2]=
{
    {0.0, 3.0}, {0.375, 2.976}, {0.746, 2.906}, {1.104, 2.789}, {1.445, 2.629},
    {1.763, 2.427}, {2.054, 2.187}, {2.312,1.912}, {2.553, 1.607}, {2.714, 1.277}, {2.853, 0.927},
    {2.947,0.562}, {2.994, 0.188},

    {0.0, -3.0},{0.375, -2.976}, {0.746, -2.906}, {1.104, -2.789}, {1.445, -2.629},
    {1.763, -2.427}, {2.054, -2.187}, {2.312, -1.912}, {2.553, -1.607}, {2.714, -1.277}, {2.853, -0.927},
    {2.947, -0.562}, {2.994, -0.188},

    {-0.375, -2.976}, {-0.746, -2.906}, {-1.104, -2.789}, {-1.445, -2.629},
    {-1.763, -2.427}, {-2.054, -2.187}, {-2.312, -1.912}, {-2.553, -1.607}, {-2.714, -1.277}, {-2.853, -0.927},
    {-2.947, -0.562}, {-2.994, -0.188},

    {-0.375, 2.976}, {-0.746, 2.906}, {-1.104, 2.789}, {-1.445, 2.629},
    {-1.763, 2.427}, {-2.054, 2.187}, {-2.312,1.912}, {-2.553, 1.607}, {-2.714, 1.277}, {-2.853, 0.927},
    {-2.947,0.562}, {-2.994, 0.188},
};

const GLdouble ArryOuter[50][2]=
{
    {0.0, 3.6}, {0.451, 3.572}, {0.895, 3.487}, {1.325, 3.347}, {1.734, 3.155},
    {2.116, 2.912}, {2.464, 2.624}, {2.774, 2.293}, {3.040, 1.929}, {3.257, 1.522}, {3.424, 1.112},
    {3.536, 0.675}, {3.593, 0.226},//0~12

     {3.593, -0.226}, {3.536, -0.675}, {3.424, -1.112}, {3.257, -1.522}, {3.040, -1.929},  {2.774, -2.293},
     {2.464, -2.624}, {2.116, -2.912},   {1.734, -3.155}, {1.325, -3.347},{0.895, -3.487}, {0.451, -3.572},  {0.0, -3.6},//13~25

    {-0.451, -3.572}, {-0.895, -3.487}, {-1.325, -3.347}, {-1.734, -3.155},
    {-2.116, -2.912}, {-2.464, -2.624}, {-2.774, -2.293}, {-3.040, -1.929}, {-3.257, -1.522}, {-3.424, -1.112},
    {-3.536, -0.675}, {-3.593, -0.226},//26~37

    {-3.593, 0.226}, {-3.536, 0.675}, {-3.424, 1.112}, {-3.257, 1.522}, {-3.040, 1.929}, {-2.774, 2.293}, {-2.464, 2.624},
    {-2.116, 2.912}, {-1.734, 3.155}, {-1.325, 3.347},{-0.895, 3.487},{-0.451, 3.572},//38~49
};

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
       initVariable();
       //lab.setText("串口不可用");
       findCom();
}

void GLWidget::initVariable()
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    upDown=0.0;
     j=0;
    countQbyte=0;
    flagPortData=true;
    flagPos = false;//不在pos范围内
    flagUp = false;
    flagDown = false;
    gear1Rot =0;//0-(4.8/4.0 * 16.0)*2;
    writePortData=0xFF;
    strComName="";
    translate = -40;
    getRandXY();
}

void GLWidget::findCom()
{
    /*使用了QT4串口*/
    /*com.setQueryMode(QextSerialBase::EventDriven);//设置模式必须放到open串口之前，否则无效
    com.setBaudRate(BAUD115200);
    com.setDataBits(DATA_8);
    com.setParity(PAR_NONE);
    com.setStopBits(STOP_1);*/

        bool findOK = false;
        QSerialPortInfo cominfo;
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {
            //qDebug()<<"Name:"<<info.portName();
           // qDebug()<<"Desvription:"<<info.description();
            //qDebug()<<"Manufacture:"<<info.manufacturer();

            if(info.description()=="USB-SERIAL CH340")
            {
               lab.setText("找到虚拟接口卡在"+info.portName());
               strComName = info.portName();
               findOK=true;
               cominfo = info;
               break;
            }
        }
        if(!findOK)
        {
            lab.setText("未找到虚拟接口卡");
            return;
        }
    com.setPortName(strComName);
    //com.setPortName("COM2");
    if(!com.open(QIODevice::ReadWrite))
    {
        lab.setText( lab.text() + ",但打开失败" );
        return;
    }
    lab.setText( lab.text() + ",打开成功" );
    com.setBaudRate(QSerialPort::Baud115200);
    com.setDataBits(QSerialPort::Data8);
    com.setParity(QSerialPort::NoParity);
    com.setStopBits(QSerialPort::OneStop);

    connect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()));
}

GLWidget::~GLWidget()
{
    makeCurrent();
    glDeleteLists(gear1, 1);
}

void GLWidget::initializeGL()//执行初始化工作
{
    static const GLfloat lightPos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
    static const GLfloat reflectance1[4] = { 0.8f, 0.1f, 0.0f, 0.5f };//颜色设置
    static const GLfloat reflectance2[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
    static const GLfloat reflectance3[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
    static const GLfloat reflectance4[4] = { 0.7f, 0.7f, 0.7f, 0.5f };
    static const GLfloat reflectance5[4] = { 0.3f, 0.3f, 0.3f, 0.3f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    gear1 = makeGear(reflectance1, 0.4, 4.0, 1.0, 0.0, 20);//中间孔，齿轮直径，齿轮厚度，尺长，齿轮密集度
    gear3 = makeGear(reflectance3, 0.4, 4.0, 6.0, 0.0, 20);//底座
    gear4 = makeGear(reflectance4, 0.0, 0.8, 10.6, 0.0, 10);//中轴和取样器架子
    gear2 = makeGear(reflectance2, 0.0, 0.15, 4.1, 0.0, 10);//装样品管子
    gear10 = makeGear(reflectance5, 0.0, 0.15, 4.1, 0.0, 10);//装样品管子
    gear5 = makeGear(reflectance5, 0.0, 2.5, 4.05,0.0, 20);//电机
    gear6 = makeGear(reflectance2, 0.8, 1.5,4.0,0.0,20);//支架
    gear7=makeGear(reflectance5,0.0,0.3,3.2,0.0,20);//拐弯
    gear8=makeGear(reflectance2,0.0,0.075,4.0,0.0,20);//取样器
    gear9=makeGear(reflectance3,0.0,3.3,5.0,0.0,20);//后底座
    gear11 = makeGear(reflectance5, 0.1, 0.1, 0.1, 0.0, 10);

    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
}

void GLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0.0, 0.0, translate);
    glPushMatrix();
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    drawGear2(gear3, 0.0, 0.0, -5.0);
    drawGear2(gear5,0.0,0.0,-6.0);
    drawGear2(gear9,0.0,5.0,-5.5);
    drawGear2(gear6,0.0,6.0,-1.0);
    drawGear2(gear4,0.0,6.0,5.5);
    drawGear4(gear8,0.0,3.6,7.0);
    drawGear5(gear8,0.0,3.0,7.0);
    drawGear6(gear11,0.0,0.0,-2.0);

    glRotated(+90.0, -1.0, 0.0, 0.0);
    drawGear3(gear7,0.0,-8.5,4.0);
    glRotated(-90.0, -1.0, 0.0, 0.0);

    //drawGear(gear1, 0.0, 0.0, 2.5, gear1Rot / 16.0);
    //drawGear(gear1, 0.0, 0.0, -1.5, gear1Rot / 16.0);
    //drawGear(gear4,0.0,0.0,-2.75,gear1Rot/16.0);

    drawGear(gear4,0.0,0.0,-2.75,gear1Rot);
    drawGear(gear1, 0.0, 0.0, 2.5, gear1Rot);
    drawGear(gear1, 0.0, 0.0, -1.5, gear1Rot);

    drawGear(gear10,randX,randY,1.0,0);
    for(int i=0; i<=49; i++)
    {
        drawGear(gear2,ArryInner[i][0],ArryInner[i][1],1.0,0);//内圈
        drawGear(gear2,ArryOuter[i][0],ArryOuter[i][1],1.0,0);//外圈
    }
    glPopMatrix();
}

void GLWidget::getRandXY()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int index =(qrand()%40)%(40-5+1)+5;
    qDebug()<<"index:"<<index;
    randX = ArryOuter[index][0];
    randY = ArryOuter[index][1];
    currentRot = 2.4*index*3;
    //qDebug()<<"index:"<<index;
    //qDebug()<<"目前处于"<<currentRot<<"°的地方";
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport(0, (height - side) / 2,(GLint)width,(GLint)height);//设置视口的大小
    gluPerspective(45.0,(GLfloat)width/(GLfloat)width,0.1,100.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glTranslated(0.0, 0.0, -50.0);//平移转换坐标系原点，这句是坐标系原点在z轴上平移，进而控制大小

}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_F1)
    {
        emit sigFullScreen();
    }
    if(event->key()==Qt::Key_Escape)
    {
       emit sigQuitFullScreen();
    }

    if(event->key()==Qt::Key_Left)
    {
        zRot+=8;
        updateGL();
    }

    if(event->key()==Qt::Key_Right)
    {
        zRot-=8;
        updateGL();
    }

    if(event->key()==Qt::Key_Up)
    {
        xRot-=8;
        updateGL();
    }

    if(event->key()==Qt::Key_Down)
    {
        xRot+=8;
        updateGL();
    }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
        translate--;
        if(translate<=-55)
            translate=-55;
    }
    else
    {
        translate++;
        if(translate>=-25)
            translate=-25;
    }
    updateGL();
}

void GLWidget::slotKeyPressEvent(QString key)
{
    if(key=="left")
    {
        zRot+=8;
        updateGL();
    }
    if(key=="right")
    {
        zRot-=8;
        updateGL();
    }
    if(key=="up")
    {
        xRot-=8;
        updateGL();
    }
    if(key=="down")
    {
        xRot+=8;
        updateGL();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        xRot+=8*dy;
        yRot+=8*dx;
    }
    else if (event->buttons() & Qt::RightButton)
    {
        xRot+=8*dy;
        zRot+=8*dx;
    }
    updateGL();
    lastPos = event->pos();
}

//***********************************************************************************************
bool ok;
void GLWidget::advanceGears()
{
    //qDebug()<<"--------------------------->come in";
     qbya = com.readAll();
     countQbyte=qbya.size();
     memset(readPortData,0,sizeof(readPortData));
     QDataStream out(&qbya,QIODevice::ReadWrite);
      j=0;
      while(!out.atEnd())
      {
          if(j>=countQbyte)
              break;
          out>>outChar;
          readPortData[j] = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUInt(&ok,16);
          emit sigInputDisplayLed(readPortData[j]);
          //数据做加密处理
          //readPortData[j]=readPortData[j]^'M';
          if(j==0&&flagPortData==true)
          {
              flagPortData=false;
              readLastData=readPortData[j];
              j++;
              continue;
          }

          if( ((readLastData&0x01)==0x01) && ((readPortData[j]&0x01)==0x00) )//主电机有高低电平的变化
          {
              if(upDown>=2.0)
              {
                  com.readAll();
                  /*disconnect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()));

                 int Yes= QMessageBox::warning(this,"警告","请抬起取样试管",QMessageBox::Yes);
                 if(Yes==QMessageBox::Yes)
                 {
                     com.readAll();
                     connect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()));
                 }*/
                 return;
              }
              if((readPortData[j]&0x02)==0x02)//dir位是1，逆时针旋转
              {            
                  gear1Rot +=0.3;//这个0.3纯粹是为了保证能400步一圈，实际上是一圈走了1200步
                  currentRot-=0.9;//黑试管所在位置（角度）
                  writePortData = writePortData | 0x01;//pos置1,只要不是指定范围，就熄灭pos指示灯

                  if((currentRot<=20&&currentRot>=0) || (currentRot<=360&&currentRot>=340))//pos到达指定范围
                  {
                        writePortData = writePortData & 0xFE;//pos位置0
                        if(!flagPos)
                        {
                            writeToPort(writePortData);
                            //qDebug()<<"逆时针wirtr:"<<writePortData;
                            flagPos= true;
                        }
                        if(currentRot<1)
                        {
                             currentRot=360;//逆时针旋转超过1°，到达360°地方
                        }
                  }
                  else//不在pos范围内
                  {
                      flagPos = false;
                  }
                  updateGL();
                  readLastData=readPortData[j];
                  j++;
                 continue;
              }
              else if((readPortData[j]&0x02)==0x0)//dir位是0,顺时针旋转
              {
                  gear1Rot -=0.3;
                  currentRot+=0.9;
                  writePortData = writePortData | 0x01;//pos置1,只要不是指定范围，就熄灭pos指示灯
                  if((currentRot<=20&&currentRot>=0) || (currentRot<=360&&currentRot>=340))
                  {
                        writePortData = writePortData&0xFE;//pos位置0
                        writeToPort(writePortData);
                        if(!flagPos)
                        {
                            writeToPort(writePortData);
                            //qDebug()<<"顺时针wirtr:"<<writePortData;
                            flagPos= true;
                        }
                        if(currentRot>=359)
                        {
                             currentRot=0;
                        }
                  }
                  else//不在pos范围内
                  {
                      flagPos = false;
                  }
                  updateGL();
                  readLastData=readPortData[j];
                  j++;
                 continue;
              }
          }//主电机高低电平变化--->end

          if( ((readLastData&0x04)==0x04) && ((readPortData[j]&0x04)==0x00) )//辅电机有高低电平的变化
          {
              if((readPortData[j]&0x08)==0x08)//向下走
              {
                  writePortData = writePortData | 0x02;//只要一向下，就up位置1，熄灭up灯
                  if( upDown>=0.0&&upDown<=3.85)//3.85是约等于值,实际的下限值是3.8495
                  {
                      if(upDown>=3.84)//down到最下边
                      {
                          upDown=3.84;
                          writePortData = writePortData&0xFB;//到达最下边后down位置0，点亮down灯
                          if(!flagDown)
                          {
                              writeToPort(writePortData);
                              flagDown = true;
                          }
                      }
                      else
                      {
                          upDown=upDown+0.0096;
                          flagDown = false;
                      }
                  }
                  updateGL();
              }
              //else if((readPortData[j]&0x08)==0x0)//向上走
            else  if((readPortData[j]&0x08)==0x0)
              {
                  writePortData = writePortData | 0x04;//只要一向上，down位置1，熄灭down灯
                  if(upDown<=0.0)//up到最上边
                  {
                      upDown=0.0;
                      writePortData = writePortData&0xFD;//到达最上边后，up位置0，点亮up灯
                      if(!flagUp)
                      {
                          writeToPort(writePortData);
                          flagUp = true;
                      }
                  }
                  else
                  {
                       upDown=upDown-0.0096;
                       flagUp = false;
                  }
                 updateGL();
               }             
         }//辅电机高低电平变化---->end
         emit sigOutputDisplayLed(writePortData);//发信号给指示灯，这时的数据是不加密的

         //做加密处理
         //writePortData = writePortData^'M';

          //---------------------------------------------------------
          //qDebug()<<"55555555555555555"<<writePortData;
         //strToHex=QChar(writePortData);
         /*if( lastStrToHex != strToHex )
         {
             //qDebug()<<"lastStrToHex"<<strToHex;
              com.write(strToHex.toLatin1());
              lastStrToHex = strToHex;
         }*/
        //com.write(strToHex.toLatin1());
         readLastData=readPortData[j];
         writePortData=0xFF;
      }//while
}

void GLWidget::writeToPort(uint i)
{
    strToHex=QChar(i);
    com.write(strToHex.toLatin1());
}

GLuint GLWidget::makeGear(const GLfloat *reflectance, GLdouble innerRadius,
                          GLdouble outerRadius, GLdouble thickness,
                          GLdouble toothSize, GLint toothCount)
{
    const double Pi = 3.14159265358979323846;

    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectance);

    GLdouble r0 = innerRadius;
    GLdouble r1 = outerRadius - toothSize / 2.0;
    GLdouble r2 = outerRadius + toothSize / 2.0;
    GLdouble delta = (2.0 * Pi / toothCount) / 4.0;
    GLdouble z = thickness / 2.0;
    int i, j;

    glShadeModel(GL_FLAT);

    for (i = 0; i < 2; ++i) {
        GLdouble sign = (i == 0) ? +1.0 : -1.0;

        glNormal3d(0.0, 0.0, sign);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= toothCount; ++j) {
            GLdouble angle = 2.0 * Pi * j / toothCount;
	    glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
	    glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
	    glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
	    glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
                       sign * z);
        }
        glEnd();

        glBegin(GL_QUADS);
        for (j = 0; j < toothCount; ++j) {
            GLdouble angle = 2.0 * Pi * j / toothCount;
	    glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
	    glVertex3d(r2 * cos(angle + delta), r2 * sin(angle + delta),
                       sign * z);
	    glVertex3d(r2 * cos(angle + 2 * delta), r2 * sin(angle + 2 * delta),
                       sign * z);
	    glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
                       sign * z);
        }
        glEnd();
    }

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < toothCount; ++i) {
        for (j = 0; j < 2; ++j) {
            GLdouble angle = 2.0 * Pi * (i + (j / 2.0)) / toothCount;
            GLdouble s1 = r1;
            GLdouble s2 = r2;
            if (j == 1)
                qSwap(s1, s2);

	    glNormal3d(cos(angle), sin(angle), 0.0);
	    glVertex3d(s1 * cos(angle), s1 * sin(angle), +z);
	    glVertex3d(s1 * cos(angle), s1 * sin(angle), -z);

	    glNormal3d(s2 * sin(angle + delta) - s1 * sin(angle),
                       s1 * cos(angle) - s2 * cos(angle + delta), 0.0);
	    glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), +z);
	    glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), -z);
        }
    }
    glVertex3d(r1, 0.0, +z);
    glVertex3d(r1, 0.0, -z);
    glEnd();

    glShadeModel(GL_SMOOTH);

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= toothCount; ++i)
    {
        GLdouble angle = i * 2.0 * Pi / toothCount;
        glNormal3d(-cos(angle), -sin(angle), 0.0);
        glVertex3d(r0 * cos(angle), r0 * sin(angle), +z);
        glVertex3d(r0 * cos(angle), r0 * sin(angle), -z);
    }
    glEnd();
    glEndList();
    return list;
}

void GLWidget::drawGear(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
                        GLdouble angle)
{
    glPushMatrix();//保存当前坐标
    glTranslated(dx, dy, dz);//移动坐标到指定位置
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标
    glRotated(angle, 0.0, 0.0, 1.0);
}

void GLWidget::drawGear2(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz)
{
    glPushMatrix();//保存当前坐标
    glTranslated(dx, dy, dz);//移动坐标到指定位置
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标

}

void GLWidget::drawGear3(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{
    glPushMatrix();//保存当前坐标
    if(upDown>=0.0&&upDown<=4.0)
    {
        glTranslated(dx, dy+upDown, dz);//移动坐标到指定位置

    }
    else if(upDown>=-4.0&&upDown<0.0)
    {
         glTranslated(dx, dy-upDown, dz);

    }
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标
}

void GLWidget::drawGear4(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{
    glPushMatrix();//保存当前坐标

    if(upDown>=0.0&&upDown<=4.0)
    {
        glTranslated(dx, dy, dz-upDown);//移动坐标到指定位置
    }
    else if(upDown>=-4.0&&upDown<0.0)
    {
         glTranslated(dx, dy, dz+upDown);
    }

    glCallList(gear);

    glPopMatrix();//恢复保存的坐标

}

void GLWidget::drawGear5(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{

    glPushMatrix();//保存当前坐标

    if(upDown>=0.0&&upDown<=4.0)
    {
        glTranslated(dx, dy, dz-upDown);//移动坐标到指定位置
    }
    else if(upDown>=-4.0&&upDown<0.0)
    {
         glTranslated(dx, dy, dz+upDown);
    }
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标

}

void spinDisplay(void)
{
    if (spin >= 30)
        status = -1;
    else if(spin<=-35)
        status = 1;
   spin += status*0.05;//这一行代码用来控制移动的速度
}

void GLWidget::drawGear6(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{
    glPushMatrix();
    glTranslated(dx,dy,dz);
    glCallList(gear);
    glPopMatrix();
}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
