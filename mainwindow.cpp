#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QAbstractItemView>
#include <QListWidget>
#include <QListWidgetItem>
#include <string>
#include <QString>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winbase.h>

const uint8_t itemLimit = 20;
uint16_t counter = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , filter(new MyNativeEventFilter)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->macroList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->macroList->setMovement(QListView::Movement::Free);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete filter;
    UnregisterHotKey((HWND) this->winId(), GlobalAddAtomA("SHIFT+F5"));
}


bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    ui->macroList->addItem("No item");
    return true;
}


//bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
//{
//    ui->macroList->addItem("No item");

//    MSG* pMsg = reinterpret_cast<MSG*>(message);

//    if (pMsg->message == WM_HOTKEY)
//    {
//        // process pMsg->wParam / lParam etc.
//        char c = MapVirtualKey (pMsg->wParam, MAPVK_VK_TO_CHAR);

//        // the intended action
//        ui->macroList->addItem(QString::fromStdString(std::to_string(c)));
//    } else {
//        ui->macroList->addItem("No item");
//    }

//    delete pMsg;

//    return true;
//}


void MainWindow::on_addNewMacro_clicked()
{
    if(ui->macroList->count() < itemLimit)
    {
        QString press = QString::fromStdString(std::to_string(counter) + "-Press-?");
        QString release = QString::fromStdString(std::to_string(counter) + "-Release-?");

        ui->macroList->addItem(press);
        ui->macroList->addItem(release);
        counter++;
    }
}


void MainWindow::on_macroList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText("clicked");
//    HWND henk = GetFocus();
}


void MainWindow::on_addNewPause_clicked()
{
    if(ui->macroList->count() < itemLimit)
    {
        QString pause = QString::fromStdString(std::to_string(counter) + "-Pause-0.05");
        ui->macroList->addItem(pause);
    }
}

