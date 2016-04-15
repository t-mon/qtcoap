/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2015-2016 Simon Stuerz <simon.stuerz@guh.guru>           *
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

#include "core.h"
#include "coaprequest.h"
#include "corelinkparser.h"

#include <QDebug>
#include <QUrlQuery>

Core::Core(QObject *parent) :
    QObject(parent)
{
    // create coap instance
    // Note: there sould only be one per application
    m_coap = new CoapNetworkAccessManager(this);

    connect(m_coap, &CoapNetworkAccessManager::replyFinished, this, &Core::onReplyFinished);
    connect(m_coap, &CoapNetworkAccessManager::notificationReceived, this, &Core::onNotificationReceived);
}

void Core::observe(const QUrl &url)
{
    m_observeReply = m_coap->enableResourceNotifications(CoapRequest(url));
    if (m_observeReply->isFinished()) {
        if (m_observeReply->error() != CoapReply::NoError) {
            qWarning() << "Reply error:" << m_observeReply->errorString();
            m_observeReply->deleteLater();
            exit(-1);
        }
    }
}

void Core::discover(const QUrl &url)
{
    QUrl discoverUrl(url);
    QString path("/.well-known/core");
    if (discoverUrl.path() != path && !discoverUrl.path().isEmpty()) {
        qWarning() << "Warning! Replacing the url path" << discoverUrl.path() << "with" << path;
    }
    discoverUrl.setPath(path);

    qDebug() << "Discovering resources on:" << discoverUrl.toString();
    m_discoverReply = m_coap->get(CoapRequest(discoverUrl));
    if (m_discoverReply->isFinished()) {
        if (m_discoverReply->error() != CoapReply::NoError) {
            qWarning() << "Reply error:" << m_discoverReply->errorString();
            exit(-1);
        }
    }
}

void Core::performRequest(const QUrl &url, const QString &methodString, const QByteArray &payload)
{
    CoapRequest request(url);

    if (methodString.toLower() == "get") {
        m_reply = m_coap->get(request);
    } else if (methodString.toLower() == "put") {
        m_reply = m_coap->put(request, payload);
    } else if (methodString.toLower() == "post") {
        m_reply = m_coap->post(request, payload);
    } else if (methodString.toLower() == "delete") {
        m_reply = m_coap->deleteResource(request);
    } else {
        qWarning() << "Unknown method" << methodString << endl;
        exit(-1);
    }
}

void Core::onReplyFinished(CoapReply *reply)
{
    if (reply->error() != CoapReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        reply->deleteLater();
        exit(-1);
    }

    if (reply == m_discoverReply) {
        qDebug() << "Discovery finished";
        qDebug() << "------------------------------------------" << endl;
        CoreLinkParser linkParser(reply->payload());
        foreach (const CoreLink &link, linkParser.links()) {
            qDebug() << link;
        }
    } else if (reply == m_observeReply) {
        if (m_observeReply->statusCode() != CoapPdu::Content) {
            qWarning() << reply;
            reply->deleteLater();
            exit(-1);
        }

        qDebug() << "Notifications enabled successfully on resource" << reply->request().url().path();
        qDebug() << QString::fromUtf8(reply->payload()) << endl;
        reply->deleteLater();
        return;

    } else if (reply == m_reply) {
        qDebug() << "------------------------------------------" << endl << reply;
    } else {
        qWarning() << "Unknown reply received: Please report a bug if you get this message";
    }

    // Note: don't forget to delete the reply
    reply->deleteLater();
    exit(0);
}

void Core::onNotificationReceived(const CoapObserveResource &resource, const int &notificationNumber, const QByteArray &payload)
{
    qDebug() << QString("Notification #%1").arg(notificationNumber) << "from" << resource.url().toString() << endl << QString::fromUtf8(payload) << endl;
}


