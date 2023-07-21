#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mynativeeventfilter.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QAbstractNativeEventFilter>
#include <BaseTsd.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result);

    MyNativeEventFilter *filter;


private slots:
    void on_addNewMacro_clicked();

    void on_macroList_itemDoubleClicked(QListWidgetItem *item);

    void on_addNewPause_clicked();

protected:
    void tick();

private:
    Ui::MainWindow *ui;

    uint64_t currentTime();

    uint64_t addTime(int ms);

    HWND getFocusGlobal();
};

#endif // MAINWINDOW_H
