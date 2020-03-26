#pragma execution_character_set("utf-8")
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QToolButton>
#include <QColorDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QCheckBox>
#include <QTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidgetAction>
#include <QClipboard>
#include <QImage>
#include <QScrollArea>
#include <QScrollBar>
#include <QTableWidget>
#include <QLabel>
#include <QMimeData>
#include <QTextBrowser>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QActionGroup *AG = new QActionGroup(this);
    AG->addAction(ui->actionPencil);
    AG->addAction(ui->actionLine);
    AG->addAction(ui->actionArrow);
    AG->addAction(ui->actionRect);
    AG->addAction(ui->actionEllipse);
    AG->addAction(ui->actionText);
    AG->addAction(ui->actionColorPicker);
    AG->addAction(ui->actionFill);
    AG->addAction(ui->actionErase);
    AG->addAction(ui->actionMove);
    AG->addAction(ui->actionRectselect);

    setStyleSheet("QScrollArea{ background-color:#cccccc; }");
    //状态栏
    LSB1 = new QLabel("欢迎使用海天鹰画图！");
    LSB1->setMinimumSize(500,20);
    LSB1->setStyleSheet("padding:0px 3px;");
    //LSB1->setFrameShape(QFrame::WinPanel);
    //LSB1->setFrameShadow(QFrame::Sunken);
    LSB2 = new QLabel("");
    LSB2->setMinimumSize(150,20);
    LSB2->setStyleSheet("padding:0px 3px;");
    LSB2->setAlignment(Qt::AlignCenter);
    //LSB2->setFrameShape(QFrame::WinPanel);
    //LSB2->setFrameShadow(QFrame::Sunken);
    ui->statusBar->addWidget(LSB1);
    ui->statusBar->addWidget(LSB2);

    text = "文字内容";
    path = "";
    move((QApplication::desktop()->width() - width())/2, (QApplication::desktop()->height() - height())/2);
    imageWidget = new ImageWidget;
    connect(imageWidget, SIGNAL(statusbar2Message(QString)), LSB2, SLOT(setText(QString)));
    connect(imageWidget, SIGNAL(pick(QColor)), this, SLOT(setPicker(QColor)));
    scrollArea = new QScrollArea;
    scrollArea->setWidget(imageWidget);
    scrollArea->widget()->setMinimumSize(600,500);
    setCentralWidget(scrollArea);

    btnColorBorder = new QToolButton(this);
    btnColorBorder->setText("□");
    btnColorBorder->setToolTip("边框");
    ui->mainToolBar->addWidget(btnColorBorder);
    checkBox_color_border = new QCheckBox(this);
    checkBox_color_border->setCheckState(Qt::Checked);
    ui->mainToolBar->addWidget(checkBox_color_border);
    btnColorFill = new QToolButton(this);
    btnColorFill->setText("■");
    btnColorFill->setToolTip("填充");
    ui->mainToolBar->addWidget(btnColorFill);
    checkBox_color_fill = new QCheckBox(this);
    ui->mainToolBar->addWidget(checkBox_color_fill);
    spinbox = new QSpinBox(this);
    spinbox->setSingleStep(1);
    spinbox->setRange(1,100);
    spinbox->setValue(1);
    ui->mainToolBar->addWidget(spinbox);
    font = qApp->font();
    ui->actionFont->setText(font.family() + "," + QString::number(font.pointSize()));
    lineEdit = new QLineEdit(text,this);
    lineEdit->setFixedWidth(60);
    ui->mainToolBar->addWidget(lineEdit);
    imageWidget->text = text;

    connect(ui->action_new, SIGNAL(triggered(bool)), imageWidget, SLOT(newfile()));
    connect(ui->action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->action_selectAll, SIGNAL(triggered(bool)), imageWidget, SLOT(selectAll()));
    connect(ui->action_delete, SIGNAL(triggered(bool)), imageWidget, SLOT(delSelect()));
    connect(ui->actionPencil, SIGNAL(triggered(bool)), imageWidget, SLOT(drawPoint()));
    connect(ui->actionLine, SIGNAL(triggered(bool)), imageWidget, SLOT(drawLine()));
    connect(ui->actionArrow, SIGNAL(triggered(bool)), imageWidget, SLOT(drawArrow()));
    connect(ui->actionRect, SIGNAL(triggered(bool)), imageWidget, SLOT(drawRect()));
    connect(ui->actionEllipse, SIGNAL(triggered(bool)), imageWidget, SLOT(drawEllipse()));
    connect(ui->actionText, SIGNAL(triggered(bool)), imageWidget, SLOT(drawText()));
    connect(ui->actionFill, SIGNAL(triggered(bool)), imageWidget, SLOT(drawFill()));
    connect(ui->actionLine, SIGNAL(triggered(bool)), imageWidget, SLOT(drawLine()));
    connect(ui->actionErase, SIGNAL(triggered(bool)), imageWidget, SLOT(drawErase()));
    connect(ui->actionMove, SIGNAL(triggered(bool)), imageWidget, SLOT(drawMove()));
    connect(ui->actionRectselect, SIGNAL(triggered(bool)), imageWidget, SLOT(drawRectselect()));
    connect(ui->actionCutSelect, SIGNAL(triggered(bool)), this, SLOT(cutSelect()));
    connect(ui->actionColorPicker, SIGNAL(triggered(bool)), imageWidget, SLOT(colorPicker()));
    connect(ui->actionZoomin, SIGNAL(triggered(bool)), imageWidget, SLOT(zoomin()));
    connect(ui->actionZoomout, SIGNAL(triggered(bool)), imageWidget, SLOT(zoomout()));
    connect(ui->actionZoom1, SIGNAL(triggered(bool)), imageWidget, SLOT(zoom1()));
    connect(ui->action_matting, SIGNAL(triggered(bool)), imageWidget, SLOT(matting()));

    connect(btnColorFill, SIGNAL(clicked()), this, SLOT(setColorFill()));
    connect(btnColorBorder, SIGNAL(clicked()), this, SLOT(setColorBorder()));
    connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(spinValueChange(int)));
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChange(QString)));
    connect(checkBox_color_border, SIGNAL(stateChanged(int)), this, SLOT(checkBox_color_borderChanged(int)));
    connect(checkBox_color_fill, SIGNAL(stateChanged(int)), this, SLOT(checkBox_color_fillChanged(int)));

    connect(new QShortcut(QKeySequence(Qt::Key_Plus), this), SIGNAL(activated()), this, SLOT(addPenWidth()));
    connect(new QShortcut(QKeySequence(Qt::Key_Equal), this), SIGNAL(activated()), this, SLOT(addPenWidth()));
    connect(new QShortcut(QKeySequence(Qt::Key_Minus), this), SIGNAL(activated()), this, SLOT(reducePenWidth()));

    QStringList SLargs = QApplication::arguments();
    qDebug() << SLargs;
    if (SLargs.length() > 1) {
        QString filepath = SLargs.at(1);
        if(filepath.startsWith("file://")){
            QUrl url(filepath);
            open(url.toLocalFile());
        }else{
            open(filepath);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //QRect childRect = scrollArea->childrenRect();
    //imageWidget->resize(childRect.size());
}

void MainWindow::on_action_changelog_triggered()
{
    QString s = "1.16\n(2020-03)\n背景文字改成了描边文字。\n\n1.15\n(2019-11)\n增加：文字工具可以绘制矩形背景。\n\n1.14\n(2019-08)\n增加：颜色调节。\n修复：选择颜色取消时颜色变黑色。\n\n1.13\n(2019-06)\n修复：选区裁剪大小微差。删除选区操作后没有设置回到框选工具。\n优化：信息内容、选区裁剪。群组工具栏Action，实现互斥Checked。\n增加：画线长度信息，画框长、宽信息，微调显示信息，裁剪后滚动条回到0。\n\n1.12\n(2019-05)\n修复：PNG转灰度图透明度丢失。\n\n1.11\n(2019-04)\n修改：删除选取填充，白色改成透明色。\n增加：棋盘背景。\n增加：鼠标移动定位文字。\n增加：Ctrl+鼠标滚轮 改变线粗或字体大小。\n\n1.10\n(2018-12)\n增加：选区马赛克。\n\n1.9\n(2018-11)\n增加：画笔粗细快捷键，画图更加方便。\n\n1.8\n(2018-05)\n修复：删除选区有虚线框，从右键打开方式无法打开文件。\n\n1.7\n(2017-11)\n颜色透明工具，取色后在边框色、填充色显示，超出边界清空鼠标位置信息。\n增加灰色背景，凸显绘图区域。\n优化代码。\n(2017-10)\n简化工具信号，简化setCursor()。\n(2017-09)\n增加箭头工具。\n增加抗锯齿。\n\n1.6\n(2017-07)\n更新日志消息窗口写不下了，改成带滚动条的文本框。\n自定义信号结合事件过滤器，把鼠标移动位置发送到主窗体信息栏。\n增加拖放打开文件。\n(2017-06)\n使用自定义信号解决子类发信息给主窗体状态栏问题，致谢rekols。\n(2017-05)\n右键打开文件记忆文件路径。\n\n1.5 (2017-04)\n透明色反色不改变。\n增加取色工具，橡皮擦颜色不再固定为白色，而是填充色。\n\n1.4 (2017-03)\n支持命令行打开文件和打开方式打开文件。\n修复鼠标指针引用本地文件，没引用资源文件，引起启动path参数改变不能加载图标的问题。\n菜单的SIGNAL-SLOT改为on_action_triggered()\n修复PNG图片裁剪丢失透明度问题。\n新建图像为透明图像。\n\n1.3 (2017-03)\n实现选区模糊。\n加入模糊滤镜。\n\n1.2 (2017-02)\n文件名显示在窗口标题栏。\n区别保存和另存为。\n增加导入图片。\n\n1.1 (2017-01)\n新增灰度、反色。\n\n1.0 (2017-01)\n解决删除选区后画不出选框的问题。\n恢复撤销。\n增加全选。\n实现选区或剪贴板移动！\n保存时自动获取打开文件的路径。\n增加按像素、比例缩放。\n实现在属性窗口设置画布大小。\n2016-12\n增加快捷键控制选框及其边框移动。\n绘图代码从MainWindow向imageWidget迁移。\n实现水平镜像、垂直镜像。\n实现放大、缩小、原始大小。\n为了增加滚动条，增加自定义imageWidget。\n状态栏显示绘图详情。\n复制选区到系统剪贴板，从系统剪贴板获取图像粘贴。\n优化颜色选择交互。\n增加撤销、重做功能，有BUG。\n设为壁纸。\n画选区，剪裁选区。\n新建图片，打开图片，保存图片。\n实现画点、线、框、圆、字。";
    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("更新历史");
    dialog->setFixedSize(400,300);
    QVBoxLayout *vbox = new QVBoxLayout;
    QTextBrowser *textBrowser = new QTextBrowser;
    textBrowser->setText(s);
    textBrowser->zoomIn();
    vbox->addWidget(textBrowser);
    QHBoxLayout *hbox = new QHBoxLayout;
    QPushButton *btnConfirm = new QPushButton("确定");
    hbox->addStretch();
    hbox->addWidget(btnConfirm);
    hbox->addStretch();
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);
    dialog->show();
    connect(btnConfirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    if (dialog->exec() == QDialog::Accepted) {
        dialog->close();
    }
}

void MainWindow::on_action_aboutQt_triggered()
{
    QMessageBox::aboutQt(NULL, "关于 Qt");
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "关于", "海天鹰画图 1.16\n一款基于 Qt 的画图程序。\n作者：海天鹰\nE-mail: sonichy@163.com\n主页：https://github.com/sonichy\n参考文献：\n绘图：\nhttp://tzc.is-programmer.com/posts/534.html\nhttp://lizhigg.love.blog.163.com/blog/static/62611781201222105550184/\n滚动条：http://down.51cto.com/data/233634\n灰度图：http://www.cnblogs.com/xianglan/archive/2010/12/24/1915905.html\n拖放打开文件：http://blog.csdn.net/rl529014/article/details/53057577");
    aboutMB.setIconPixmap(QPixmap(":/HTYPaint.png"));
    aboutMB.exec();
}

void MainWindow::setColorBorder()
{
    QPalette plt = btnColorBorder->palette();
    QBrush brushb = plt.color(QPalette::ButtonText);
    QColor color = QColorDialog::getColor(brushb.color(), this);
    if(color.isValid()){
        imageWidget->pen.setColor(color);
        plt.setColor(QPalette::ButtonText, color);
        btnColorBorder->setPalette(plt);
    }
}

void MainWindow::setColorFill()
{
    QPalette plt = btnColorFill->palette();
    QBrush brush = plt.color(QPalette::ButtonText);
    QColor colorf = QColorDialog::getColor(brush.color(), this);
    if(colorf.isValid()){
        imageWidget->brush.setColor(colorf);
        plt.setColor(QPalette::ButtonText, colorf);
        btnColorFill->setPalette(plt);
    }
}

void MainWindow::spinValueChange(int i)
{
    imageWidget->pen.setWidth(i);
}

void MainWindow::textChange(QString s)
{
    imageWidget->text=s;
}

void MainWindow::on_action_open_triggered()
{
    if (path == "")
        path = ".";
    path = QFileDialog::getOpenFileName(this, "打开图片", path, "图片文件(*.jpg *.jpeg *.png *.bmp)");
    if(path.length() != 0){
        open(path);
    }
}

void MainWindow::open(QString filepath)
{
    imageWidget->load(filepath);
    filename = QFileInfo(filepath).fileName();
    setWindowTitle(filename + " - 海天鹰画图");
    LSB1->setText("打开 " + filepath);
    path = filepath;
    qDebug() << path;
    qDebug() << imageWidget->size();
}

void MainWindow::on_action_import_triggered(){
    QString filenamei = "";
    if(path == ""){
        filenamei = QFileDialog::getOpenFileName(this,"导入图片", ".", "图片文件(*.jpg *.png *.bmp)");
    }else{
        filenamei = QFileDialog::getOpenFileName(this,"导入图片", path, "图片文件(*.jpg *.png *.bmp)");
    }
    LSB1->setText("导入 " + filenamei);
    if(path.length() != 0){
        imageWidget->image = imageWidget->imgtemp;
        QImage imgImport(filenamei);
        QApplication::clipboard()->setImage(imgImport, QClipboard::Clipboard);
        imageWidget->paste();
        imageWidget->imgmove = imgImport;
        //imageWidget->imgtemp=imgImport;
        //imageWidget->imgbuf[0]=imgImport;
    }
}

void MainWindow::on_action_save_triggered(){
    qDebug() << "save path=" << path;
    if(path.length() != 0){
        LSB1->setText("保存 " + path);
        imageWidget->image = imageWidget->imgtemp;
        imageWidget->save(path);
    }else{
        on_action_saveas_triggered();
    }
}

void MainWindow::on_action_saveas_triggered()
{
    if(path == "") path = "./未命名.jpg";
    path = QFileDialog::getSaveFileName(this, "保存图片", path, "图片文件(*.jpg *.png *.bmp)");
    if(path.length() != 0){
        LSB1->setText("保存 " + path);
        imageWidget->image = imageWidget->imgtemp;
        imageWidget->save(path);
        filename = QFileInfo(path).fileName();
        setWindowTitle(filename+" - 海天鹰画图");
    }
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    font = QFontDialog::getFont(&ok, font, this, "选择字体");
    if (ok) {
       imageWidget->font = font;
       ui->actionFont->setText(font.family() + "," + QString::number(font.pointSize()));
    }
}

void MainWindow::on_action_undo_triggered()
{
    imageWidget->undo();
}

void MainWindow::on_action_redo_triggered()
{
    imageWidget->redo();
}

void MainWindow::on_action_setWallpaper_triggered()
{
    QString str = "gsettings set org.gnome.desktop.background picture-uri file://" + path;
    QByteArray ba = str.toLocal8Bit();
    qDebug() << ba.data();
    system(ba.data());
}

void MainWindow::on_action_property_triggered()
{
    //qDebug() << image.width() << "X" << image.height();
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("属性");
    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *label = new QLabel("宽度：");
    QSpinBox *spinw = new QSpinBox;
    spinw->setRange(0, 20000);
    spinw->setValue(imageWidget->imgtemp.width());
    hbox->addWidget(label, 0, Qt::AlignCenter);
    hbox->addWidget(spinw);
    vbox->addLayout(hbox);
    label = new QLabel("高度：");
    QSpinBox *spinh = new QSpinBox;
    spinh->setRange(0, 20000);
    spinh->setValue(imageWidget->imgtemp.height());
    hbox = new QHBoxLayout;
    hbox->addWidget(label, 0, Qt::AlignCenter);
    hbox->addWidget(spinh);
    vbox->addLayout(hbox);
    QPushButton *btnConfirm = new QPushButton("确定");
    QPushButton *btnCancel = new QPushButton("取消");
    hbox = new QHBoxLayout;
    hbox->addWidget(btnConfirm);
    hbox->addWidget(btnCancel);
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);
    connect(btnConfirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), dialog, SLOT(reject()));
    if(dialog->exec() == QDialog::Accepted){
        imageWidget->newSize(spinw->value(), spinh->value());
    }
    dialog->close();
}

void MainWindow::on_action_excude_triggered()
{
    int wo = imageWidget->imgtemp.width();
    int ho = imageWidget->imgtemp.height();
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("缩放");

    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *label = new QLabel("宽度：");
    spinw = new QSpinBox;
    spinw->setRange(1,10000);
    spinw->setValue(wo);
    hbox->addWidget(label,0,Qt::AlignCenter);
    hbox->addWidget(spinw);
    label = new QLabel("px");
    hbox->addWidget(label);
    spinwr = new QSpinBox;
    spinwr->setRange(10,1000);
    spinwr->setValue(100);
    hbox->addWidget(spinwr);
    label = new QLabel("%");
    hbox->addWidget(label);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    label = new QLabel("高度：");
    hbox->addWidget(label,0,Qt::AlignCenter);
    spinh = new QSpinBox;
    spinh->setRange(1,10000);
    spinh->setValue(ho);
    hbox->addWidget(spinh);
    label = new QLabel("px");
    hbox->addWidget(label);
    spinhr = new QSpinBox;
    spinhr->setRange(10,1000);
    spinhr->setValue(100);
    hbox->addWidget(spinhr);
    label = new QLabel("%");
    hbox->addWidget(label);
    vbox->addLayout(hbox);

    QPushButton *btnConfirm = new QPushButton("确定");
    QPushButton *btnCancel = new QPushButton("取消");
    hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(btnConfirm);
    hbox->addWidget(btnCancel);
    hbox->addStretch();
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);

    connect(spinw, SIGNAL(valueChanged(int)), this, SLOT(onSpinwChanged(int)));
    connect(spinh, SIGNAL(valueChanged(int)), this, SLOT(onSpinhChanged(int)));
    connect(spinwr, SIGNAL(valueChanged(int)), this, SLOT(onSpinwrChanged(int)));
    connect(spinhr, SIGNAL(valueChanged(int)), this, SLOT(onSpinhrChanged(int)));
    connect(btnConfirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), dialog, SLOT(reject()));
    if(dialog->exec() == QDialog::Accepted){
        imageWidget->scale(spinw->value(),spinh->value());
    }
    dialog->close();
}

void MainWindow::onSpinwChanged(int i)
{
    disconnect(spinwr, SIGNAL(valueChanged(int)), this, SLOT(onSpinwrChanged(int)));
    spinwr->setValue(i*100/imageWidget->image.width());
    connect(spinwr, SIGNAL(valueChanged(int)), this, SLOT(onSpinwrChanged(int)));
}

void MainWindow::onSpinhChanged(int i)
{
    disconnect(spinhr, SIGNAL(valueChanged(int)), this, SLOT(onSpinhrChanged(int)));
    spinhr->setValue(i*100/imageWidget->image.height());
    connect(spinhr, SIGNAL(valueChanged(int)), this, SLOT(onSpinhrChanged(int)));
}

void MainWindow::onSpinwrChanged(int i)
{
    disconnect(spinw, SIGNAL(valueChanged(int)), this, SLOT(onSpinwChanged(int)));
    spinw->setValue(imageWidget->image.width()*i/100);
    connect(spinw, SIGNAL(valueChanged(int)), this, SLOT(onSpinwChanged(int)));
}

void MainWindow::onSpinhrChanged(int i)
{
    disconnect(spinh, SIGNAL(valueChanged(int)), this, SLOT(onSpinhChanged(int)));
    spinh->setValue(imageWidget->image.height()*i/100);
    connect(spinh, SIGNAL(valueChanged(int)), this, SLOT(onSpinhChanged(int)));
}

void MainWindow::on_action_copy_triggered()
{
    if(imageWidget->draw_type == imageWidget->SELECT_DRAW){
        imageWidget->copy();
        LSB1->setText("选区已复制到剪贴板");
    }
}

void MainWindow::on_action_paste_triggered()
{
    imageWidget->paste();
    LSB1->setText("剪贴板已粘贴");
}


void MainWindow::on_actionRotateLeft_triggered()
{
    imageWidget->rotate(-90);
}

void MainWindow::on_actionRotateRight_triggered()
{
    imageWidget->rotate(90);
}

void MainWindow::on_actionMirrorHorizontal_triggered()
{
    imageWidget->mirror(true,false);
}

void MainWindow::on_actionMirrorVertical_triggered()
{
    imageWidget->mirror(false,true);
}

void MainWindow::on_action_help_triggered()
{
    QString s = "<h3 align='center'>快捷键</h3>"
                "<table align='center'>"
                "<tr><td>Alt + ↑</td><td>选区上移</td></tr>"
                "<tr><td>Alt + ↓</td><td>选区下移</td></tr>"
                "<tr><td>Alt + ←</td><td>选区左移</td></tr>"
                "<tr><td>Alt + →</td><td>选区右移</td></tr>"
                "<tr><td>Ctrl + ↑</td><td>选区上边上移</td></tr>"
                "<tr><td>Ctrl + ↓</td><td>选区上边下移</td></tr>"
                "<tr><td>Ctrl + ←</td><td>选区左边左移</td></tr>"
                "<tr><td>Ctrl + →</td><td>选区左边右移</td></tr>"
                "<tr><td>Shift + ↑</td><td>选区下边上移</td></tr>"
                "<tr><td>Shift + ↓</td><td>选区下边下移</td></tr>"
                "<tr><td>Shift + ←</td><td>选区右边左移</td></tr>"
                "<tr><td>Shift + →</td><td>选区右边右移</td></tr>"
                "<tr><td>Ctrl + 鼠标滚轮</td><td>改变线粗或字体大小</td></tr>"
                "<table>";
    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("帮助");
    dialog->setFixedSize(400,300);
    QVBoxLayout *vbox = new QVBoxLayout;
    QTextBrowser *textBrowser = new QTextBrowser;
    textBrowser->setHtml(s);
    textBrowser->zoomIn();
    vbox->addWidget(textBrowser);
    QHBoxLayout *hbox = new QHBoxLayout;
    QPushButton *pushButton_confirm = new QPushButton("确定");
    hbox->addStretch();
    hbox->addWidget(pushButton_confirm);
    hbox->addStretch();
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);
    dialog->show();
    connect(pushButton_confirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    if (dialog->exec() == QDialog::Accepted) {
        dialog->close();
    }
}

void MainWindow::checkBox_color_borderChanged(int state)
{
    if(state == Qt::Checked){
        imageWidget->boolBorder=true;
    }else if(state == Qt::PartiallyChecked){
    }else{
        imageWidget->boolBorder=false;
    }
}

void MainWindow::checkBox_color_fillChanged(int state)
{
    if(state == Qt::Checked){
       imageWidget->boolFill = true;
    }else if(state == Qt::PartiallyChecked){
    }else{
       imageWidget->boolFill=false;
    }
}

void MainWindow::on_action_gray_triggered()
{
    imageWidget->gray();
}

void MainWindow::on_action_invert_triggered()
{
    imageWidget->invert();
}

void MainWindow::on_action_transparent_triggered()
{
    imageWidget->transparent();
}

void MainWindow::on_action_blur_triggered()
{
    imageWidget->blur(spinbox->value());
}

void MainWindow::on_action_mosaic_triggered()
{
    imageWidget->mosaic(spinbox->value());
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    //if(e->mimeData()->hasFormat("text/uri-list")) //只能打开文本文件
        e->acceptProposedAction(); //可以在这个窗口部件上拖放对象
}

void MainWindow::dropEvent(QDropEvent *e) //释放对方时，执行的操作
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;

    QString fileName = urls.first().toLocalFile();

//    foreach (QUrl u, urls) {
//        qDebug() << u.toString();
//    }
//    qDebug() << urls.size();

    if(fileName.isEmpty())
        return;

    open(fileName);
}

void MainWindow::setPicker(QColor color)
{
    QPalette plt = btnColorBorder->palette();
    plt.setColor(QPalette::ButtonText, color);
    btnColorBorder->setPalette(plt);

    plt = btnColorFill->palette();
    plt.setColor(QPalette::ButtonText, color);
    btnColorFill->setPalette(plt);
}

void MainWindow::addPenWidth()
{
    spinbox->setValue(spinbox->value() + 1);
}

void MainWindow::reducePenWidth()
{
    spinbox->setValue(spinbox->value() - 1);
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    if(QApplication::keyboardModifiers() == Qt::ControlModifier){
        if (e->delta() > 0) {
            if(imageWidget->draw_type == imageWidget->TEXT_DRAW){
                font.setPointSize(font.pointSize()+1);
                imageWidget->font = font;
                ui->actionFont->setText(font.family() + "," + QString::number(font.pointSize()));
            }else{
                addPenWidth();
            }
        } else {
            if(imageWidget->draw_type == imageWidget->TEXT_DRAW){
                font.setPointSize(font.pointSize()-1);
                imageWidget->font = font;
                ui->actionFont->setText(font.family() + "," + QString::number(font.pointSize()));
            }else{
                reducePenWidth();
            }
        }
    } else {
        QWidget::wheelEvent(e);
    }
}

void MainWindow::cutSelect()
{
    imageWidget->cutSelect();
    scrollArea->horizontalScrollBar()->setSliderPosition(0);
    scrollArea->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::on_action_adjustRGB_triggered()
{
    int ov = 0;
    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("调色");
    dialog->setFixedSize(400, 200);

    QVBoxLayout *vbox = new QVBoxLayout;
    QGridLayout *gridLayout = new QGridLayout;

    // RGB
    QLabel *label = new QLabel("RGB");
    gridLayout->addWidget(label, 0, 0);
    QLabel *label_value_RGB = new QLabel("0");
    gridLayout->addWidget(label_value_RGB, 0, 1);
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(-50, 50);
    gridLayout->addWidget(slider, 0, 2);
    QPushButton *pushButton_reset_RGB = new QPushButton;
    pushButton_reset_RGB->setIcon(QIcon::fromTheme("view-refresh"));
    gridLayout->addWidget(pushButton_reset_RGB, 0, 3);

    // Red
    label = new QLabel("R");
    gridLayout->addWidget(label, 1, 0, Qt::AlignCenter);
    QLabel *label_value_R = new QLabel("0");
    gridLayout->addWidget(label_value_R, 1, 1);
    QSlider *sliderR = new QSlider(Qt::Horizontal);
    sliderR->setRange(-50, 50);
    gridLayout->addWidget(sliderR, 1, 2);
    QPushButton *pushButton_reset_R = new QPushButton;
    pushButton_reset_R->setIcon(QIcon::fromTheme("view-refresh"));
    connect(pushButton_reset_R, &QPushButton::clicked, [&](){
        sliderR->setValue(0);
    });
    gridLayout->addWidget(pushButton_reset_R, 1, 3);

    // Green
    label = new QLabel("G");
    gridLayout->addWidget(label, 2, 0, Qt::AlignCenter);
    QLabel *label_value_G = new QLabel("0");
    gridLayout->addWidget(label_value_G, 2, 1);
    QSlider *sliderG = new QSlider(Qt::Horizontal);
    sliderG->setRange(-50, 50);
    gridLayout->addWidget(sliderG, 2, 2);
    QPushButton *pushButton_reset_G = new QPushButton;
    pushButton_reset_G->setIcon(QIcon::fromTheme("view-refresh"));
    connect(pushButton_reset_G, &QPushButton::clicked, [&](){
        sliderG->setValue(0);
    });
    gridLayout->addWidget(pushButton_reset_G, 2, 3);

    // Blue
    label = new QLabel("B");
    gridLayout->addWidget(label, 3, 0, Qt::AlignCenter);
    QLabel *label_value_B = new QLabel("0");
    gridLayout->addWidget(label_value_B, 3, 1);
    QSlider *sliderB = new QSlider(Qt::Horizontal);
    sliderB->setRange(-50, 50);
    gridLayout->addWidget(sliderB, 3, 2);
    QPushButton *pushButton_reset_B = new QPushButton;
    pushButton_reset_B->setIcon(QIcon::fromTheme("view-refresh"));
    connect(pushButton_reset_B, &QPushButton::clicked, [&](){
        sliderB->setValue(0);
    });
    gridLayout->addWidget(pushButton_reset_B, 3, 3);

    vbox->addLayout(gridLayout);

    connect(slider, &QSlider::actionTriggered, [&](){
        ov = slider->value();
    });
    connect(slider, &QSlider::valueChanged, [&](int v){
        label_value_RGB->setText(QString::number(v));
        sliderR->setValue(sliderR->value() + v - ov);
        sliderG->setValue(sliderG->value() + v - ov);
        sliderB->setValue(sliderB->value() + v - ov);
    });
    connect(pushButton_reset_RGB, &QPushButton::clicked, [&](){
        slider->setValue(0);
        sliderR->setValue(0);
        sliderG->setValue(0);
        sliderB->setValue(0);
    });
    connect(sliderR, &QSlider::valueChanged, [&](int v){
        label_value_R->setText(QString::number(v));
        imageWidget->adjustRGB(v, sliderG->value(), sliderB->value(), false);
    });
    connect(sliderG, &QSlider::valueChanged, [&](int v){
        label_value_G->setText(QString::number(v));
        imageWidget->adjustRGB(sliderR->value(), v, sliderB->value(), false);
    });
    connect(sliderB, &QSlider::valueChanged, [&](int v){
        label_value_B->setText(QString::number(v));
        imageWidget->adjustRGB(sliderR->value(), sliderG->value(), v, false);
    });

    QPushButton *pushButton_confirm = new QPushButton("确定");
    QPushButton *pushButton_cancel = new QPushButton("取消");
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(pushButton_confirm);
    hbox->addWidget(pushButton_cancel);
    hbox->addStretch();
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);
    connect(pushButton_confirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(pushButton_cancel, SIGNAL(clicked()), dialog, SLOT(reject()));
    if(dialog->exec() == QDialog::Accepted){
        imageWidget->adjustRGB(sliderR->value(), sliderG->value(), sliderB->value(), true);
    }
}