#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewidget.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QClipboard>
#include <QShortcut>
#include <QRect>
#include "math.h"

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
    qApp->installEventFilter(this);
    cundo = 0;
    font = qApp->font();
    newfile();
    pen.setColor(Qt::black);
    pen.setWidth(1);
    brush = QBrush(Qt::transparent,Qt::SolidPattern);
    brush.setColor(Qt::black);
    boolBorder = true;
    boolFill = false;
    connect(new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Up), this), SIGNAL(activated()), this, SLOT(moveUp()));
    connect(new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Down), this), SIGNAL(activated()), this, SLOT(moveDown()));
    connect(new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Left), this), SIGNAL(activated()), this, SLOT(moveLeft()));
    connect(new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Right), this), SIGNAL(activated()), this, SLOT(moveRight()));
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up), this), SIGNAL(activated()), this, SLOT(moveTopUp()));
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down), this), SIGNAL(activated()), this, SLOT(moveTopDown()));
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left), this), SIGNAL(activated()), this, SLOT(moveLeftLeft()));
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right), this), SIGNAL(activated()), this, SLOT(moveLeftRight()));
    connect(new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Left), this), SIGNAL(activated()), this, SLOT(moveRightLeft()));
    connect(new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Right), this), SIGNAL(activated()), this, SLOT(moveRightRight()));
    connect(new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Up), this), SIGNAL(activated()), this, SLOT(moveBottomUp()));
    connect(new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Down), this), SIGNAL(activated()), this, SLOT(moveBottomDown()));
}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::paintEvent(QPaintEvent *)
{
    resize(imgtemp.size());
    setMinimumSize(imgtemp.size());
    QPainter painter(this);

    //生成棋盘背景
    int dx = 50;
    int dy = 50;
    QBrush brush1(QColor(200,200,200));
    QBrush brush2(QColor(255,255,255));
    for(int y=0; y<imgtemp.height(); y+=dy){
        for(int x=0; x<imgtemp.width(); x+=dx){
            painter.fillRect(x, y, dx/2, dy/2, brush1);
            painter.fillRect(x + dx/2, y, dx/2, dy/2, brush2);
            painter.fillRect(x, y + dy/2, dx/2, dy/2, brush2);
            painter.fillRect(x + dx/2, y + dy/2, dx/2, dy/2, brush1);
        }
    }

    painter.drawImage(0, 0, imgtemp);
}

void ImageWidget::draw(QImage &img)
{
    QPainter painter(&img);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    switch(draw_type){
    case POINT_DRAW:
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.drawLine(startPnt,endPnt);
        break;
    case LINE_DRAW:{
        int length = sqrt(pow(endPnt.x()-startPnt.x(),2) + pow(endPnt.y()-startPnt.y(),2));
        painter.drawLine(startPnt,endPnt);
        emit statusbar2Message("("+QString::number(startPnt.x()) + "," + QString::number(startPnt.y()) + ") - (" + QString::number(endPnt.x()) + "," + QString::number(endPnt.y()) + ") = " + QString::number(length));
        break;}
    case ARROW_DRAW:{
        if(boolFill){
            painter.setBrush(brush);
        }else{
            painter.setBrush(QBrush(Qt::transparent,Qt::SolidPattern));
        }
        if(!boolBorder){
            pen.setColor(Qt::transparent);
        }
        QPen pena = pen;
        pena.setWidth(1);
        painter.setPen(pena);
        float pi = 3.14;
        float a = pi/9;
        float l = sqrt(pow(endPnt.y() - startPnt.y(),2) + pow(endPnt.x() - startPnt.x(),2));
        float b = asin((endPnt.y() - startPnt.y())/l);
        int LW = pen.width();
        float x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,l56;
        l56 = LW/2;
        float d = (l56 + LW/2)/sin(a);
        if (startPnt.x() > endPnt.x()) {
            x1 = startPnt.x() - LW/2*sin(b);
            x2 = startPnt.x() + LW/2*sin(b);
            x4 = endPnt.x() + d*cos(b-a);
            x3 = x4 - l56*sin(b);
            x5 = endPnt.x() + d*cos(a + b);
            x6 = x5 + l56*sin(b);
        } else {
            x1 = startPnt.x() + LW/2*sin(b);
            x2 = startPnt.x() - LW/2*sin(b);
            x4 = endPnt.x() - d*cos(b-a);
            x3 = x4 + l56*sin(b);
            x5 = endPnt.x() - d*cos(a+b);
            x6 = x5 - l56*sin(b);
        }
        y1 = startPnt.y() - LW/2*cos(b);
        y2 = startPnt.y() + LW/2*cos(b);
        y5 = endPnt.y() - d*sin(a+b);
        y6 = y5 + l56*cos(b);
        y4 = endPnt.y() - d*sin(b-a);
        y3 = y4 - l56*cos(b);
        QPointF points[7] = {
            QPointF(x1,y1),
            QPointF(x2,y2),
            QPointF(x3,y3),
            QPointF(x4,y4),
            QPointF(endPnt),
            QPointF(x5,y5),
            QPointF(x6,y6)
        };
        painter.drawPolygon(points,7);
        int length = sqrt(pow(endPnt.x()-startPnt.x(),2) + pow(endPnt.y()-startPnt.y(),2));
        emit statusbar2Message("("+QString::number(startPnt.x()) + "," + QString::number(startPnt.y()) + ") - (" + QString::number(endPnt.x()) + "," + QString::number(endPnt.y()) + ")" + QString::number(length));
        break;}
    case RECT_DRAW:{
        pen.setJoinStyle(Qt::MiterJoin);
        if(boolFill){
            painter.setBrush(brush);
        }else{
            painter.setBrush(QBrush(Qt::transparent,Qt::SolidPattern));
        }
        QRect rect(startPnt, endPnt);
        if(!boolBorder){
            pen.setColor(Qt::transparent);
        }
        painter.drawRect(rect);
        emit statusbar2Message("("+QString::number(startPnt.x()) + "," + QString::number(startPnt.y()) + ") - (" + QString::number(endPnt.x()) + "," + QString::number(endPnt.y()) + ") = (" + QString::number(rect.width()) + "," + QString::number(rect.height()) + ")");
        break;}
    case SELECT_DRAW:{
        painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));
        QRect rect(startPnt, endPnt);
        painter.drawRect(rect);
        emit statusbar2Message("("+QString::number(startPnt.x()) + "," + QString::number(startPnt.y()) + ") - (" + QString::number(endPnt.x()) + "," + QString::number(endPnt.y()) + ") = (" + QString::number(rect.width()) + "," + QString::number(rect.height()) + ")");
        break;}
    case ELLIPSE_DRAW:{
        QRect rect(startPnt,endPnt);
        if(boolFill){
            painter.setBrush(brush);
        }else{
            painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));
        }
        painter.drawEllipse(rect);
        emit statusbar2Message("(" + QString::number(startPnt.x()) + "," + QString::number(startPnt.y()) + ") = (" + QString::number(rect.width()) + "," + QString::number(rect.height()) + ")");
        break;}
    case TEXT_DRAW:
        painter.setFont(font);
        if(boolFill){
            painter.setBrush(brush);
            QFontMetrics FM(font);
            QRect rect(endPnt, FM.boundingRect(text).size() + QSize(FM.boundingRect("*").width(), 0));
            painter.drawRect(rect);
            painter.drawText(endPnt.x() + FM.boundingRect("*").width()/2, endPnt.y() + rect.height()/1.3, text);
        }else{
            painter.drawText(endPnt.x(), endPnt.y(), text);
        }
        break;
    case FILL_DRAW:
        break;
    case ERASE_DRAW:
        //painter.setPen(QPen(Qt::white,1));
        //painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
        painter.setBrush(QBrush(brush));
        painter.drawEllipse(endPnt.x(), endPnt.y(), 20, 20);
        break;
    case DEL_DRAW:{
        QRect rect(startPnt, endPnt);
        for(int x = rect.x(); x<rect.x() + rect.width(); x++){
            for(int y = rect.y(); y < rect.y() + rect.height(); y++){
                image.setPixelColor(x, y, brush.color());
            }
        }
        draw_type = SELECT_DRAW;
        break;}
    case MOVE_DRAW:{
        QRect target(endPnt,imgmove.size());
        QPoint p(0,0);
        QRect source(p,imgmove.size());
        painter.drawImage(target,imgmove,source);
        break;}
    case COLORPICKER_DRAW:{
        QRgb RGB = imgtemp.pixel(startPnt.x(),startPnt.y());
        pen.setColor(RGB);
        brush.setColor(RGB);
        painter.setBrush(brush);
        emit pick(pen.color());
        break;}
    default:
        break;
    }
    update();
}

void ImageWidget::mousePressEvent(QMouseEvent *e)
{
    startPnt = e->pos();
    endPnt = e->pos();
    switch(draw_type){
    case ERASE_DRAW:
        draw(imgtemp);
        image = imgtemp;
        moveImgbuf();
        cundo = 0;
        break;
    case COLORPICKER_DRAW:
        draw(imgtemp);
        break;
    default:
        break;
    }
    if(e->buttons() & Qt::RightButton){
        imgtemp = image;
        draw(imgtemp);
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    //if(this->isPressed){
    if(e->buttons() & Qt::LeftButton){
        endPnt = e->pos();
        switch(draw_type){
        case POINT_DRAW:
            draw(imgtemp);
            startPnt = endPnt;
            cundo=0;
            moveImgbuf();
            break;
        case ERASE_DRAW:
            draw(imgtemp);
            cundo=0;
            moveImgbuf();
            break;
        default:
            imgtemp = image;
            draw(imgtemp);
        }
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
        switch(draw_type){
        case MOVE_DRAW:
            imgtemp = image;
            draw(imgtemp);
            break;
        case SELECT_DRAW:
            break;
        case TEXT_DRAW:
            imgtemp = image;
            draw(imgtemp);
            image = imgtemp;
            moveImgbuf();
            cundo = 0;
            break;
        default:
            image = imgtemp;
            moveImgbuf();
            cundo = 0;
        }

}

void ImageWidget::zoomin()
{
    QImage imgzoom = imgtemp.scaled(imgtemp.width() * 1.2, imgtemp.height() * 1.2);
    imgtemp = imgzoom;
    update();
}

void ImageWidget::zoomout()
{
    QImage imgzoom = imgtemp.scaled(imgtemp.width() * 0.8, imgtemp.height() * 0.8);
    imgtemp = imgzoom;
    update();
}

void ImageWidget::zoom1()
{
    imgtemp = imgload;
    update();
}

void ImageWidget::scale(int width,int height)
{
    QImage imgscale = imgtemp.scaled(width,height);
    imgtemp = imgscale;
    image = imgtemp;
    update();
}

void ImageWidget::rotate(qreal degrees)
{
    QMatrix matrix;
    matrix.rotate(degrees);
    QImage imgrotate = imgtemp.transformed(matrix,Qt::SmoothTransformation);
    imgtemp = imgrotate;
    image = imgtemp;
    //resize(imgrotate.size());
    update();
}

void ImageWidget::mirror(bool bh,bool bv)
{
    QImage imgmirror = imgtemp.mirrored(bh,bv);
    imgtemp = imgmirror;
    image = imgtemp;
    //resize(imgmirror.size());
    update();
}

void ImageWidget::drawPoint()
{
    image = imgtemp;
    draw_type = POINT_DRAW;
    setCursor(QCursor(QPixmap(":/pencil.png")));
}

void ImageWidget::drawLine()
{
    image = imgtemp;
    draw_type = LINE_DRAW;
    setCursor(QCursor(QPixmap(":/line.png")));
}

void ImageWidget::drawArrow()
{
    image = imgtemp;
    draw_type = ARROW_DRAW;
    setCursor(QCursor(QPixmap(":/arrow.png")));
}

void ImageWidget::drawRect()
{
    image = imgtemp;
    draw_type = RECT_DRAW;
    setCursor(QCursor(QPixmap(":/rect.png")));
}

void ImageWidget::drawEllipse()
{
    image = imgtemp;
    draw_type = ELLIPSE_DRAW;
    setCursor(QCursor(QPixmap(":/ellipse.png")));
}

void ImageWidget::drawText()
{
    image = imgtemp;
    draw_type = TEXT_DRAW;
    setCursor(QCursor(QPixmap(":/text.png")));
}

void ImageWidget::drawFill()
{
    image = imgtemp;
    draw_type = FILL_DRAW;
    setCursor(QCursor(QPixmap(":/fill.png")));
}

void ImageWidget::drawErase()
{
    image = imgtemp;
    draw_type = ERASE_DRAW;
    setCursor(QCursor(QPixmap(":/eraser.png")));
    //spinbox->setValue(20);
}

void ImageWidget::drawMove()
{
    if(draw_type == SELECT_DRAW){
        imgmove = imgtemp.copy(startPnt.x()+1,startPnt.y()+1,endPnt.x()-startPnt.x()-1,endPnt.y()-startPnt.y()-1);
    }else{
        imgmove = imgpaste;
    }
    draw_type = MOVE_DRAW;
    setCursor(QCursor(QPixmap(":/move.png")));
}

void ImageWidget::drawRectselect()
{
    image = imgtemp;
    draw_type = SELECT_DRAW;
    setCursor(QCursor(QPixmap(":/rectselect.png")));
}

void ImageWidget::colorPicker()
{
    draw_type = COLORPICKER_DRAW;
    setCursor(QCursor(QPixmap(":/color_picker.png")));
}

void ImageWidget::selectAll()
{
    //image=imgtemp;
    draw_type = SELECT_DRAW;
    startPnt = QPoint(0,0);
    endPnt = QPoint(imgtemp.width()-2,imgtemp.height()-2);
    draw(imgtemp);
}

void ImageWidget::delSelect()
{
    draw_type = DEL_DRAW;
    draw(image);
    imgtemp = image;
    update();
}

void ImageWidget::newfile()
{
    QImage imgnew = QImage(800, 600, QImage::Format_ARGB32);
    imgnew.fill(Qt::transparent);
    imgtemp = imgnew;
    image = imgnew;
    update();
}

void ImageWidget::load(QString fileName)
{
    imgload.load(fileName);
    imgtemp = imgload;
    image = imgload;
    resize(imgload.size());
    update();
    draw_type = NONE_DRAW;
    setCursor(Qt::ArrowCursor);
    cundo = 0;
    for(int i=0; i<10; i++){
        imgbuf[i] = image;
    }
}

void ImageWidget::save(QString path)
{
    image.save(path,0,100);
}

void ImageWidget::cutSelect()
{
    //imgtemp = imgtemp.copy(startPnt.x()+1,startPnt.y()+1,endPnt.x()-startPnt.x()-1,endPnt.y()-startPnt.y()-1);
    imgtemp = image.copy(startPnt.x(), startPnt.y(), endPnt.x() - startPnt.x() + 1, endPnt.y() - startPnt.y() + 1);
    image = imgtemp;
    update();
}

void ImageWidget::newsize(int width,int height)
{
    imgtemp = QImage(width,height,QImage::Format_ARGB32);
    imgtemp.fill(Qt::transparent);
    QPainter painter(&imgtemp);
    painter.drawImage(0,0,image);
    update();
    image = imgtemp;
}

void ImageWidget::copy()
{
    if(draw_type == SELECT_DRAW){
        QImage imgcopy = imgtemp.copy(startPnt.x()+1,startPnt.y()+1,endPnt.x()-startPnt.x()-1,endPnt.y()-startPnt.y()-1);
        QApplication::clipboard()->setImage(imgcopy, QClipboard::Clipboard);
        //ui->statusBar->showMessage("选区已复制到剪贴板");
    }
}

void ImageWidget::paste()
{
    //image=imgtemp;
    imgpaste = QApplication::clipboard()->image();
    QPainter painter(&imgtemp);
    painter.drawImage(0,0,imgpaste);
    update();
    //ui->statusBar->showMessage("剪贴板已粘贴");
}

void ImageWidget::moveUp()
{
    if(startPnt.y()>0 && endPnt.y()>0){
        imgtemp = image;
        startPnt.setY(startPnt.y()-1);
        endPnt.setY(endPnt.y()-1);
        draw(imgtemp);
    }
}

void ImageWidget::moveDown()
{
    if(endPnt.y() < imgtemp.height()){
        imgtemp = image;
        startPnt.setY(startPnt.y()+1);
        endPnt.setY(endPnt.y()+1);
        draw(imgtemp);
    }
}

void ImageWidget::moveLeft()
{
    if(startPnt.x()>0 && endPnt.x()>0){
        imgtemp = image;
        startPnt.setX(startPnt.x()-1);
        endPnt.setX(endPnt.x()-1);
        draw(imgtemp);
    }
}

void ImageWidget::moveRight()
{
    if(endPnt.x() < imgtemp.width()-2 && startPnt.x() < imgtemp.width()-2){
        imgtemp = image;
        startPnt.setX(startPnt.x()+1);
        endPnt.setX(endPnt.x()+1);
        draw(imgtemp);
    }
}

void ImageWidget::moveTopUp()
{
    if(startPnt.y()>0){
        imgtemp = image;
        startPnt.setY(startPnt.y()-1);
        draw(imgtemp);
    }
}

void ImageWidget::moveTopDown()
{
    if(startPnt.y() < imgtemp.height()-1){
        imgtemp = image;
        startPnt.setY(startPnt.y()+1);
        draw(imgtemp);
    }
}

void ImageWidget::moveLeftLeft()
{
    if(startPnt.x() > 0){
        imgtemp = image;
        startPnt.setX(startPnt.x()-1);
        draw(imgtemp);
    }
}

void ImageWidget::moveLeftRight()
{
    if(startPnt.x() < imgtemp.width()-1){
        imgtemp = image;
        startPnt.setX(startPnt.x()+1);
        draw(imgtemp);
    }
}

void ImageWidget::moveRightLeft()
{
    if(endPnt.x() > 0){
        imgtemp = image;
        endPnt.setX(endPnt.x()-1);
        draw(imgtemp);
    }
}

void ImageWidget::moveRightRight()
{
    if(endPnt.x()<imgtemp.width()-1){
        imgtemp = image;
        endPnt.setX(endPnt.x()+1);
        draw(imgtemp);
    }
}

void ImageWidget::moveBottomUp()
{
    if(endPnt.y() > 0){
        imgtemp = image;
        endPnt.setY(endPnt.y()-1);
        draw(imgtemp);
    }
}

void ImageWidget::moveBottomDown()
{
    if(endPnt.y() < imgtemp.height()-2){
        imgtemp = image;
        endPnt.setY(endPnt.y()+1);
        draw(imgtemp);
    }
}

void ImageWidget::moveImgbuf()
{
    for(int i=9; i>0; i--){
        imgbuf[i] = imgbuf[i-1];
        //qDebug() << "imgbuf" << i << "=" << i-1;
    }
    imgbuf[0] = imgtemp;
}

void ImageWidget::undo()
{
    qDebug() << "undo" << cundo;
    if(cundo < 10){
        imgtemp = imgbuf[cundo];
        image = imgtemp;
        //draw(imgtemp);
        update();
        cundo++;
    }
}

void ImageWidget::redo()
{
    qDebug() << "redo" << cundo;
    if(cundo==10)cundo--;
    if(cundo>=0){
        imgtemp = imgbuf[cundo];
        image = imgtemp;
        //draw(imgtemp);
        update();
        cundo--;
    }
}

void ImageWidget::gray()
{
    int w,h;
    w = imgtemp.width();
    h = imgtemp.height();
    QImage imgGray(w, h, QImage::Format_ARGB32);
    for(int x=0; x<w; x++){
        for(int y=0; y<h; y++){
            QRgb rgba = image.pixel(x,y);
            int gray = qGray(rgba);
            QRgb grayRGBA = qRgba(gray, gray, gray, qAlpha(rgba));
            imgGray.setPixel(x, y, grayRGBA);
        }
    }
    imgtemp = imgGray;
    image = imgGray;
    update();
    moveImgbuf();
}

void ImageWidget::invert()
{
    int w,h;
    w = imgtemp.width();
    h = imgtemp.height();
    QImage imgInvert(w,h,QImage::Format_ARGB32);
    for(int x=0; x<w; x++){
        for(int y=0;y<h; y++){
            QRgb RGB = image.pixel(x,y);
            QRgb RGBi = qRgba(255-qRed(RGB), 255-qGreen(RGB), 255-qBlue(RGB), qAlpha(RGB));
            imgInvert.setPixel(x,y,RGBi);
        }
    }
    imgtemp = imgInvert;
    image = imgInvert;
    update();
    moveImgbuf();
}

void ImageWidget::transparent()
{
    int w,h;
    w = imgtemp.width();
    h = imgtemp.height();
    QImage imgTansparent(w,h,QImage::Format_ARGB32);
    for(int x=0; x<w; x++){
        for(int y=0; y<h; y++){
            QRgb RGB = image.pixel(x,y);
            //if (RGB == pen.color().rgb()) {//颜色相等
            if(qAbs(qRed(RGB) - pen.color().red()) <= pen.width() && qAbs(qGreen(RGB) - pen.color().green()) <= pen.width() && qAbs(qBlue(RGB) - pen.color().blue()) < pen.width()){//颜色近似
                QRgb RGBT = qRgba(qRed(RGB), qGreen(RGB), qBlue(RGB), 0);
                imgTansparent.setPixel(x,y,RGBT);
            }else{
                imgTansparent.setPixel(x,y,RGB);
            }
        }
    }
    imgtemp = imgTansparent;
    image = imgTansparent;
    update();
    moveImgbuf();
}

void ImageWidget::blur(int p)
{
    int xs,xe,ys,ye;
    if(startPnt.x()<endPnt.x() && startPnt.y()<endPnt.y()){xs=startPnt.x(); ys=startPnt.y(); xe=endPnt.x(); ye=endPnt.y();}
    if(startPnt.x()>endPnt.x() && startPnt.y()<endPnt.y()){xs=endPnt.x(); ys=startPnt.y(); xe=startPnt.x(); ye=endPnt.y();}
    if(startPnt.x()>endPnt.x() && startPnt.y()>endPnt.y()){xs=endPnt.x(); ys=endPnt.y(); xe=startPnt.x(); ye=startPnt.y();}
    if(startPnt.x()<endPnt.x() && startPnt.y()>endPnt.y()){xs=startPnt.x(); ys=endPnt.y(); xe=endPnt.x(); ye=startPnt.y();}
    QImage imgBlur(qAbs(endPnt.x()-startPnt.x())+2, qAbs(endPnt.y()-startPnt.y())+2, QImage::Format_RGB32);
    for(int x=xs; x<xe+2; x++){
        for(int y=ys;y<ye+2; y++){
            int red=0, green=0, blue=0, pc=0;
            for(int a=-p; a<=p; a++){
                for(int b=-p; b<=p; b++){
                    int xa = x+a;
                    int yb = y+b;
                    if(xa>0 && yb>0 && xa<image.width() && yb<image.height()){
                        red += qRed(image.pixel(xa,yb));
                        green += qGreen(image.pixel(xa,yb));
                        blue += qBlue(image.pixel(xa,yb));
                        pc += 1;
                    }
                }
            }
            QRgb RGBblur = qRgb(red/pc,green/pc,blue/pc);
            imgBlur.setPixel(x-xs,y-ys,RGBblur);
        }
    }
    QPainter painter(&imgtemp);
    painter.drawImage(xs,ys,imgBlur);
    update();
}

bool ImageWidget::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    if(event->type() == QEvent::MouseMove){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        emit statusbar2Message(QString::number(mouseEvent->pos().x())+","+QString::number(mouseEvent->pos().y()));
    }else if(event->type() == QEvent::Leave){
        emit statusbar2Message("");
    }
    return false;
}

void ImageWidget::mosaic(int p)
{
    imgtemp = image;
    update();
    int xs, ys, xe, ye;
    if(startPnt.x()<endPnt.x() && startPnt.y()<endPnt.y()){xs=startPnt.x(); ys=startPnt.y(); xe=endPnt.x(); ye=endPnt.y();}
    if(startPnt.x()>endPnt.x() && startPnt.y()<endPnt.y()){xs=endPnt.x(); ys=startPnt.y(); xe=startPnt.x(); ye=endPnt.y();}
    if(startPnt.x()>endPnt.x() && startPnt.y()>endPnt.y()){xs=endPnt.x(); ys=endPnt.y(); xe=startPnt.x(); ye=startPnt.y();}
    if(startPnt.x()<endPnt.x() && startPnt.y()>endPnt.y()){xs=startPnt.x(); ys=endPnt.y(); xe=endPnt.x(); ye=startPnt.y();}
    QImage imgMosaic(qAbs(endPnt.x()-startPnt.x()), qAbs(endPnt.y()-startPnt.y()), QImage::Format_RGB32);
    QPainter painterm(&imgMosaic);
    for(int x=xs; x<xe; x+=p){
       for(int y=ys; y<ye; y+=p){
            QRgb rgb = image.pixel(x,y);
            QColor colorm(qRed(rgb),qGreen(rgb),qBlue(rgb));
            painterm.setPen(QPen(colorm));
            painterm.setBrush(QBrush(colorm));
            painterm.drawRect(QRect(x-xs,y-ys,p,p));
        }
    }
    QPainter painter(&imgtemp);
    painter.drawImage(xs,ys,imgMosaic);
    update();
}

void ImageWidget::matting()
{
    //抠图，不可能完成的任务
    int w,h;
    w = imgtemp.width();
    h = imgtemp.height();
    QImage imgTansparent(w,h,QImage::Format_ARGB32);
    int gray = qGray(pen.color().rgb());
    for(int x=0; x<w; x++){
        for(int y=0; y<h; y++){
            QRgb RGB = image.pixel(x,y);
            if (qAbs(qGray(RGB) - gray) < 5){//颜色近似
                QRgb RGBT = qRgba(qRed(RGB), qGreen(RGB), qBlue(RGB), 0);
                imgTansparent.setPixel(x,y,RGBT);
            }else{
                imgTansparent.setPixel(x,y,RGB);
            }
        }
    }
    imgtemp = imgTansparent;
    image = imgTansparent;
    update();
    moveImgbuf();
}

void ImageWidget::adjustRGB(int vr, int vg, int vb, bool b)
{
    int w,h;
    w = image.width();
    h = image.height();
    QImage imga(w, h, QImage::Format_ARGB32);
    for(int x=0; x<w; x++){
        for(int y=0; y<h; y++){
            QRgb RGB = image.pixel(x,y);
            int R = qRed(RGB);
            int G = qGreen(RGB);
            int B = qBlue(RGB);
            R += vr;
            G += vg;
            B += vb;
            if (R < 0) R = 0;
            if (G < 0) G = 0;
            if (B < 0) B = 0;
            if (R > 255) R = 255;
            if (G > 255) G = 255;
            if (B > 255) B = 255;
            QRgb RGBa = qRgba(R, G, B, qAlpha(RGB));
            imga.setPixel(x, y, RGBa);
        }
    }
    if(b){
        image = imga;
        moveImgbuf();
    }else{
        imgtemp = imga;
    }
    update();
}