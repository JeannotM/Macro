#include "mainwindow.h"
#include "mynativeeventfilter.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Macro_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;

    RegisterHotKey((HWND) w.winId(),
                   GlobalAddAtomA("SHIFT+F5"),
                   MOD_SHIFT,
                   VK_F5);

    QObject::connect(w.filter, &MyNativeEventFilter::buttonClick, &w, &MainWindow::nativeEventFilter);

    a.installNativeEventFilter(w.filter);

    w.show();
    return a.exec();
}
