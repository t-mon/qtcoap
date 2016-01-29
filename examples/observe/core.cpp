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

#include "core.h"
#include "coaprequest.h"
#include "corelinkparser.h"

#include <QDebug>

Core::Core(QObject *parent) :
    QObject(parent)
{
    // create coap instance (sould only be one / application)
    m_coap = new Coap(this);
    connect(m_coap, &Coap::replyFinished, this, &Core::onReplyFinished);
    connect(m_coap, &Coap::notificationReceived, this, &Core::onNotificationReceived);

    // observe resource (enable notifications)
    CoapRequest request(QUrl("coap://[2001:db8:c001:f00d:221:2eff:ff00:331d]:5683/sensors/button"));
    m_enableReply = m_coap->enableResourceNotifications(request);
}

void Core::onReplyFinished(CoapReply *reply)
{
    if (reply == m_enableReply) {
        if (reply->error() != CoapReply::NoError) {
            qDebug() << "Enable notifications reply error" << reply->errorString();
            reply->deleteLater();
            return;
        }

        if (reply->statusCode() != CoapPdu::Content) {
            qDebug() << "Enable notifications return unexpected status code" << reply;
            reply->deleteLater();
            return;
        }

        qDebug() << "Enabled successfully notifications for " << reply->request().url().path();
    }

    // Note: please don't forget to delete the reply
    reply->deleteLater();
}

void Core::onNotificationReceived(const CoapObserveResource &resource, const int &notificationNumber, const QByteArray &payload)
{
    qDebug() << "Button pressed notification" << notificationNumber << resource.url().path() << "\n" << payload;
}


