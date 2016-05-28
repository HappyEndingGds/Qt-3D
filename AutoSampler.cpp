#include "AutoSampler.h"
#include "ui_AutoSampler.h"

AutoSampler::AutoSampler(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoSampler)
{
    ui->setupUi(this);
    this->setGeometry(200,50,800,660);
    glayout =new QGridLayout;
    widget =new QWidget;

    glayout->setContentsMargins(5,5,5,5);
    glayout->setRowStretch(0,1);
    glayout->setRowStretch(1,1);
    glayout->setRowStretch(2,1);
    glayout->setColumnStretch(0,1);
    glayout->setColumnStretch(1,1);
    glayout->setColumnStretch(2,1);

    setCentralWidget(widget);

    glayout->addWidget(ui->scrollArea,0,0,2,2);
    glayout->addWidget(ui->lab_pic,2,0,1,2);
    glayout->addWidget(ui->textEdit,0,2,2,1);
    glayout->addWidget(ui->groupBox,2,2,1,1);
    widget->setLayout(glayout);

    ui->scrollArea->setWidget(&glWidget);
    ui->lab_pic->setPixmap(QPixmap(":/4.png"));
    ui->groupBox->setDisabled(true);

    ui->textEdit->setReadOnly(true);
    ui->textEdit->append("实验指导:\r\n");
    QString strExperimentalContent = "1. 主转盘电机的转动控制\r\n";
    strExperimentalContent = "（1）硬件连线主转盘电机采用步进电机，电机驱动器实现了电机脉冲控制及方向控制。"
                                     "转动脉冲信号线接至虚拟接口板的P10；方向控制信号接至P11。"
                                  "开发板使用P20引脚产生电机转动脉冲；P21引脚控制电机的方向。将开发板与虚拟接口板对接，如下图所示：";

    //插入图片
    ui->textEdit->append(strExperimentalContent+"\r\n");
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml("<img src=':\\Experiment.png'>");
    ui->textEdit->textCursor().insertFragment(fragment);

    strExperimentalContent = "（2）将虚拟接口板与PC机通过USB线进行连接，运行仿真软件AutoSampler。";
    ui->textEdit->append(strExperimentalContent+"\r\n");

    strExperimentalContent = "（3）软件设计"
            "步进电机每接收到一个脉冲，就转动1个固定角度。根据步进电机转动精度不同，可分为100-800步/圈不等。自动进样器采用400步/圈，即每转动一步，其转过的角度为0.9度。"
            "要让步进电机连续转动，就需要一定频率发送脉冲。脉冲频率越高，电机转动越快，反之亦然。如果频率过高，电机的响应速度不够，则电机不会转动；如果频率过低，电机转动会出现走走停停、有顿挫感。"
            "利用单片机的定时中断实现脉冲发送，可以比较精准地控制脉冲的频率。如下面的定时中断程序，实现1ms的定时中断：";
    ui->textEdit->append(strExperimentalContent+"\r\n");

    strExperimentalContent = "#include <STC89C5xRC.h>\r\n"
            "bit  PFlag=0; //脉冲高低电平标志\r\n"
            "char tcount=0; //计时计数器，控制脉冲频率\r\n"
            "char M1SPEED=20;  //电机转动速度设定值\r\n"
            "void timer0() interrupt 1\r\n"
            "{\r\n"
                  "     TH0=0xfc;\r\n"
                  "     TL0=0x18;//0xfc18=64536+1000=65536,1ms under 12MHz\r\n"
                  "     TR0=1;  //Start timer1 to generate BAUD rate pulse\r\n"

                  "if(++tcount>M1SPEED)//高电平维持20ms，低电平维持20ms，每个脉冲共40ms\r\n"
                  "{\r\n"
                      "     tcount = 0;\r\n"
                      "     PFlag = ~PFlag;\r\n"
                      "     P20 = PFlag;    //通过P20引脚发送脉冲信号\r\n"
                   "}\r\n"
            "}\r\n"

            "通过修改M1SPEED的数值，就可以控制电机的速度：\r\n"
            "void SetM1(char speed,bit dir)\r\n"
            "{\r\n"
                "   M1SPEED = speed;\r\n"
                "   P21 = dir;\r\n"
            "}\r\n"

            "主程序完成定时器的初始化及电机的设置\r\n"
            "main()\r\n"
            "{\r\n"
                "   TMOD=0x1;/*--T1:Mode 2;T0:Mode 1*/\r\n"
                "   TH0=0xfc;\r\n"
                "   TL0=0x18;//0xfc18=64536+1000=65536,1ms under 12MHz\r\n"
                "   IE = 0x82; \r\n"
                "   TR0=1;  //Start timer1 to generate BAUD rate pulse\r\n"
                "   SetM1(20,0);\r\n"
                "while(1); \r\n"
            "}";
      ui->textEdit->append(strExperimentalContent);

      strExperimentalContent = "下载运行该程序，观察虚拟软件中的自动进样器，其主转盘将以固定速度顺时针转动。";
      ui->textEdit->append(strExperimentalContent+"\r\n");

      ui->textEdit->append("2. 主转盘的定位");
      strExperimentalContent = "主转盘1号瓶的下面埋设一块圆形磁铁。在主转盘下面的固定底座上，靠近针架的位置安装了霍尔传感器。"
                               "当磁铁靠近霍尔传感器时，传感器将通过电磁感应，输出高电平信号；当磁铁原理传感器时，传感器输出信号为低电平。"
                               "传感器的输出信号接至虚拟接口板";
      ui->textEdit->append(strExperimentalContent+"\r\n");

      ui->textEdit->moveCursor(QTextCursor::Start);
      ui->textEdit->installEventFilter(this);//安装事件监听器，这样就不用重新textEdit的键盘事件了

    connect(&glWidget,SIGNAL(sigInputDisplayLed(uint)),this,SLOT(slotInputDisplayLed(uint)),Qt::QueuedConnection);
    connect(&glWidget,SIGNAL(sigOutputDisplayLed(uint)),this,SLOT(slotOutputDisplayLed(uint)),Qt::QueuedConnection);
    connect(&glWidget,SIGNAL(sigFullScreen()),this,SLOT(slotFullScreen()),Qt::QueuedConnection);
    connect(&glWidget,SIGNAL(sigQuitFullScreen()),this,SLOT(slotQuitFullScreen()),Qt::QueuedConnection);
    connect(this,SIGNAL(sigKey(QString)),&glWidget,SLOT(slotKeyPressEvent(QString)),Qt::QueuedConnection);

    ui->statusBar->addPermanentWidget(&glWidget.lab);
}

bool AutoSampler::eventFilter(QObject *obj, QEvent * event)
{
    if(obj==ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *eventKey = static_cast<QKeyEvent *>(event);
            if(eventKey->key()==Qt::Key_F1)
            {
                slotFullScreen();
            }
            if(eventKey->key()==Qt::Key_Escape)
            {
               slotQuitFullScreen();
            }
            if(eventKey->key() == Qt::Key_Left)
            {
                emit sigKey("left");
            }
            if(eventKey->key() == Qt::Key_Right)
            {
                emit sigKey("right");
            }
            if(eventKey->key() == Qt::Key_Up)
            {
                emit sigKey("up");
            }
            if(eventKey->key() == Qt::Key_Down)
            {
                emit sigKey("down");
            }

            return true;
        }
        else
            return false;
    }
    else
        return AutoSampler::eventFilter(obj,event);
}

void AutoSampler::slotFullScreen()
{
      ui->groupBox->setHidden(true);
      ui->lab_pic->setHidden(true);
      ui->textEdit->setHidden(true);
      glayout->addWidget(ui->scrollArea,0,0,3,3);
      this->setWindowFlags(Qt::Window);
      this->showFullScreen();
}

void AutoSampler::slotQuitFullScreen()
{
    glayout->addWidget(ui->scrollArea,0,0,2,2);
    ui->groupBox->setHidden(false);
    ui->lab_pic->setHidden(false);
    ui->textEdit->setHidden(false);
    this->setWindowFlags(Qt::Window);
    this->showNormal();
}

void AutoSampler::slotInputDisplayLed(uint portData)
{
    if( (portData&0x01)==0x00 )//主电机脉冲
    {
        ui->rad_1_0->setChecked(true);
    }
    else if( (portData&0x01)==0x01 )
    {
        ui->rad_1_0->setChecked(false);
    }

    if( (portData&0x02)==0x00 )//主电机方向
    {
        ui->rad_1_1->setChecked(true);
    }
    else if( (portData&0x02)==0x02 )
    {
        ui->rad_1_1->setChecked(false);
    }

    if( (portData&0x04)==0x00 )//辅电机脉冲
    {
        ui->rad_1_2->setChecked(true);
    }
    else if( (portData&0x04)==0x04 )
    {
        ui->rad_1_2->setChecked(false);
    }

    if( (portData&0x08)==0x00 )//辅电机方向
    {
        ui->rad_1_3->setChecked(true);
    }
    else if( (portData&0x08)==0x08 )
    {
        ui->rad_1_3->setChecked(false);
    }
}

void AutoSampler::slotOutputDisplayLed(uint portData)
{
    if( (portData&0x01)==0x00 )//初始位置
    {
        //qDebug()<<"11111111111111111点亮了";
        ui->rad_0_0->setChecked(true);
    }
    else if( (portData&0x01)==0x01 )
    {
        //qDebug()<<"222222222222222222熄灭了";
        ui->rad_0_0->setChecked(false);
    }

    if( (portData&0x02)==0x00 )//up
    {
        ui->rad_0_1->setChecked(true);
    }
    else if( (portData&0x02)==0x02 )
    {
        ui->rad_0_1->setChecked(false);
    }

    if( (portData&0x04)==0x00 )//down
    {
        ui->rad_0_2->setChecked(true);
    }
    else if( (portData&0x04)==0x04 )
    {
        ui->rad_0_2->setChecked(false);
    }

}

AutoSampler::~AutoSampler()
{
    delete ui;
}
