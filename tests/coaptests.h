#ifndef COAPTESTS_H
#define COAPTESTS_H

#include <QUrl>
#include <QObject>
#include <QHostInfo>
#include <QHostAddress>

#include <QSignalSpy>
#include <QtTest>

#include "coap.h"
#include "coappdu.h"
#include "coapreply.h"

class CoapTests : public QObject
{
    Q_OBJECT

public:
    explicit CoapTests(QObject *parent = 0);

private:
    Coap *m_coap;

private slots:
    void invalidUrl_data();
    void invalidUrl();

    void invalidScheme();

    void pingTest();
    void helloTest();
    void brokenTest();
    void queryTest();
    void subPath();
    void secret();
    void separated();

    void deleteResource();
    void post();
    void put();
};

#endif // COAPTESTS_H
