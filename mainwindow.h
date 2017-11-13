#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <imagewidget.h>
#include <QToolButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    QToolButton *btnColorFill,*btnColorBorder;
    QLineEdit *lineEdit;
    QSpinBox *spinbox,*spinw,*spinwr,*spinh,*spinhr;
    QString text,path;
    QColor colorf;
    QCheckBox *checkFill,*checkBorder;
    QLabel *labelFont;
    QString filename;

protected:
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);

private:
    QScrollArea *scrollArea;
    ImageWidget *imageWidget;
    QLabel *LSB1,*LSB2;

private slots:
//  void on_action_new_triggered();
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_saveas_triggered();
    void on_action_import_triggered();
    void on_action_changelog_triggered();
    void on_action_aboutQt_triggered();
    void on_action_about_triggered();

    void on_action_undo_triggered();
    void on_action_redo_triggered();
    void on_action_copy_triggered();
    void on_action_paste_triggered();
    void on_action_setWallpaper_triggered();
    void on_action_property_triggered();
    void on_action_help_triggered();
    void on_action_excude_triggered();
    void on_action_gray_triggered();    
    void on_action_invert_triggered();
    void on_action_blur_triggered();
    void on_action_transparent_triggered();

    void on_actionRotateLeft_triggered();
    void on_actionRotateRight_triggered();
    void on_actionMirrorHorizontal_triggered();
    void on_actionMirrorVertical_triggered();
    void on_actionFont_triggered();
    void setColorFill();
    void setColorBorder();
    void spinValueChange(int);
    void textChange(QString);
    void onSpinwChanged(int);
    void onSpinhChanged(int);
    void onSpinwrChanged(int);
    void onSpinhrChanged(int);
    void checkBorderChanged(int);
    void checkFillChanged(int);
    void open(QString);
};

#endif // MAINWINDOW_H
