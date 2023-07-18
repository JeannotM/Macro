#ifndef MYNATIVEEVENTFILTER_H
#define MYNATIVEEVENTFILTER_H

#include <QObject>
#include <QApplication>
#include <QAbstractNativeEventFilter>

#include <windows.h>
#include <winbase.h>

class MyNativeEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) Q_DECL_OVERRIDE;

signals:
    bool buttonClick(const QByteArray &eventType, void *message, qintptr *result);


};

#endif // MYNATIVEEVENTFILTER_H
