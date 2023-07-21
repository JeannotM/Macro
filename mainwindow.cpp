#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QAbstractItemView>
#include <QListWidget>
#include <QListWidgetItem>
#include <future>
#include <string>
#include <QString>
#include <iostream>
#include <thread>
#include <chrono>

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinBase.h>
#include <WinUser.h>

using std::chrono::milliseconds, std::chrono::system_clock, std::chrono::duration_cast, std::async;

const uint8_t itemLimit = 20;
const uint8_t tickTime = 50; // 20x per second
std::future<void> tickInstance;
bool isActivated = false;
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

void MainWindow::tick()
{
    uint64_t now;
    uint64_t resumeAfter = addTime(tickTime);
    while(1)
    {
        if(!isActivated) break;
        now = currentTime();

        if(now < resumeAfter) continue;
        resumeAfter = addTime(tickTime);

        HWND focussedWindow = getFocusGlobal();
        SendMessage(focussedWindow, WM_KEYDOWN, 0x45, 0);
        SendMessage(focussedWindow, WM_CHAR, 0x45, 0);
        SendMessage(focussedWindow, WM_KEYUP, 0x45, 0);
        std::this_thread::sleep_for(milliseconds(tickTime));
    }
}


// https://stackoverflow.com/questions/21577348/get-handle-of-focused-control-from-another-applications-window
HWND MainWindow::getFocusGlobal()
{
    HWND Wnd;
    HWND Result = NULL;
    DWORD TId, PId;

    Result = GetFocus();
    if (!Result)
    {
        Wnd = GetForegroundWindow();
        if(Wnd)
        {
            TId = GetWindowThreadProcessId(Wnd, &PId);
            if (AttachThreadInput(GetCurrentThreadId(), TId, TRUE))
            {
                Result = GetFocus();
                AttachThreadInput(GetCurrentThreadId(), TId, FALSE);
            }
        }
    }
    return Result;
}

uint64_t MainWindow::currentTime()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

uint64_t MainWindow::addTime(int ms)
{
    uint64_t now = currentTime();
    return now + ms;
}

bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    isActivated = !isActivated;
    if(isActivated)
    {
        auto ptr = std::make_unique<MainWindow>(this);
        tickInstance = std::async(std::launch::async, [&ptr]() { return ptr->tick(); });
    }

    //    MSG* msg = reinterpret_cast<MSG*>(message);
    //    char c = MapVirtualKey(msg->wParam, MAPVK_VK_TO_CHAR);
    //    ui->macroList->addItem(QString::fromStdString(std::to_string(c)));
    return true;
}

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
}


void MainWindow::on_addNewPause_clicked()
{
    if(ui->macroList->count() < itemLimit)
    {
        QString pause = QString::fromStdString(std::to_string(counter) + "-Pause-0.05");
        ui->macroList->addItem(pause);
    }
}

