#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <vector>
#include <QScrollArea>
#include <imagewidget.h>
using namespace std;
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

protected:
    void resizeEvent(QResizeEvent *);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

private:
    QScrollArea *scrollArea;
    ImageWidget *imageWidget;
    QLabel *LSB1,*LSB2;

private slots:
    void on_action_new_triggered();
    void on_action_open_triggered();
    void on_action_changelog_triggered();
    void on_action_aboutQt_triggered();
    void on_action_about_triggered();
    void on_action_selectAll_triggered();
    void on_action_delete_triggered();
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
    void on_action_save_triggered();
    void on_action_saveas_triggered();
    void on_action_import_triggered();
    void on_action_blur_triggered();

    void on_actionPencil_triggered();
    void on_actionLine_triggered();
    void on_actionArrow_triggered();
    void on_actionRect_triggered();
    void on_actionEllipse_triggered();
    void on_actionText_triggered();
    void on_actionFill_triggered();
    void on_actionErase_triggered();
    void on_actionMove_triggered();
    void on_actionRectselect_triggered();
    void on_actionCutSelect_triggered();
    void on_actionRotateLeft_triggered();
    void on_actionRotateRight_triggered();
    void on_actionMirrorHorizontal_triggered();
    void on_actionMirrorVertical_triggered();
    void on_actionZoomin_triggered();
    void on_actionZoomout_triggered();
    void on_actionZoom1_triggered();
    void on_actionFont_triggered();
    void on_actionColorPicker_triggered();

    void setColorFill();
    void setColorBorder();
    void onSpinValueChanged(int i);
    void onTextChanged(QString text);
    void onSpinwChanged(int i);
    void onSpinhChanged(int i);
    void onSpinwrChanged(int i);
    void onSpinhrChanged(int i);
    void checkBorderChanged(int state);
    void checkFillChanged(int state);
    void open(QString path);
};

#endif // MAINWINDOW_H
