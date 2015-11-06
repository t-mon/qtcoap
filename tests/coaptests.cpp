/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2015 Simon Stuerz <simon.stuerz@guh.guru>                *
 *                                                                         *
 *  This file is part of QtCoap.                                           *
 *                                                                         *
 *  QtCoap is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 3 of the License.                *
 *                                                                         *
 *  QtCoap is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with QtCoap. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "coaptests.h"

CoapTests::CoapTests(QObject *parent) : QObject(parent)
{
    m_coap = new Coap(this);
}

void CoapTests::invalidUrl_data()
{
    QTest::addColumn<QUrl>("url");

    QTest::newRow("missing backslash") << QUrl("coap:/coap.me");
    QTest::newRow("invalid host") << QUrl("coap://foo.bar");
}

void CoapTests::invalidUrl()
{
    QFETCH(QUrl, url);

    CoapRequest request(url);
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(reply->isFinished(), "Reply not finished.");
    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->error(), CoapReply::HostNotFoundError);
}

void CoapTests::invalidScheme()
{
    CoapRequest request(QUrl("http://coap.me"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait(1000);
    QVERIFY2(reply->isFinished(), "Reply not finished.");
    QVERIFY2(spy.count() == 0, "Got a response.");
    QCOMPARE(reply->error(), CoapReply::InvalidUrlSchemeError);
}

void CoapTests::ping()
{
    CoapRequest request(QUrl("coap://coap.me/"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->ping(request);
    spy.wait();

    QVERIFY2(reply->isFinished(), "Reply not finished.");
    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->statusCode(), CoapPdu::Empty);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY(reply->payload().isEmpty());
    reply->deleteLater();
}

void CoapTests::hello()
{
    CoapRequest request(QUrl("coap://coap.me/hello"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Content);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "world", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::broken()
{
    CoapRequest request(QUrl("coap://coap.me/broken"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::InternalServerError);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QVERIFY2(reply->payload() == "Oops: broken", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::query()
{
    CoapRequest request(QUrl("coap://coap.me/query?guh=awesome"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Content);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "You asked me about: guh=awesome", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::subPath()
{
    CoapRequest request(QUrl("coap://coap.me/path/sub1"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Content);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "TD_CORE_COAP_09 sub1", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::extendedOptionLength()
{
    CoapRequest request(QUrl("coap://coap.me:5683/123412341234123412341234"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Content);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "very long resource name", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::extendedDelta_data()
{
    QTest::addColumn<QUrl>("url");

    QTest::newRow("weird33") << QUrl("coap://coap.me/weird33");
    QTest::newRow("weird44") << QUrl("coap://coap.me/weird44");
    QTest::newRow("weird55") << QUrl("coap://coap.me/weird55");
    QTest::newRow("weird333") << QUrl("coap://coap.me/weird333");
    QTest::newRow("weird3333") << QUrl("coap://coap.me/weird3333");
    QTest::newRow("weird33333") << QUrl("coap://coap.me/weird33333");
}

void CoapTests::extendedDelta()
{
    QFETCH(QUrl, url);

    CoapRequest request(url);
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Content);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload().startsWith("resource with option"), "Invalid payload");
    reply->deleteLater();
}

void CoapTests::secret()
{
    CoapRequest request(QUrl("coap://coap.me/secret"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait();

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Unauthorized);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "Not authorized", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::separated()
{
    CoapRequest request(QUrl("coap://coap.me:5683/separate"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->get(request);
    spy.wait(10000);

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Content);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "That took a long time", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::deleteResource()
{
    CoapRequest request(QUrl("coap://coap.me:5683/validate"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->deleteResource(request);
    spy.wait(10000);

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Deleted);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "DELETE OK", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::post()
{
    CoapRequest request(QUrl("coap://coap.me:5683/validate"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->post(request, "guh is awesome");
    spy.wait(10000);

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Created);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "POST OK", "Invalid payload");
    reply->deleteLater();
}

void CoapTests::put()
{
    CoapRequest request(QUrl("coap://coap.me:5683/validate"));
    qDebug() << request.url().toString();

    QSignalSpy spy(m_coap, SIGNAL(replyFinished(CoapReply*)));
    CoapReply *reply = m_coap->put(request, "guh is awesome");
    spy.wait(10000);

    QVERIFY2(spy.count() > 0, "Did not get a response.");
    QCOMPARE(reply->messageType(), CoapPdu::Acknowledgement);
    QCOMPARE(reply->statusCode(), CoapPdu::Changed);
    QCOMPARE(reply->contentType(), CoapPdu::TextPlain);
    QCOMPARE(reply->error(), CoapReply::NoError);
    QVERIFY2(reply->payload() == "PUT OK", "Invalid payload");
    reply->deleteLater();
}

QTEST_MAIN(CoapTests)
