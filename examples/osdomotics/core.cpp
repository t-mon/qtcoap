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

    // timer for refresh and light toggle
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);
    m_timer->setInterval(1000);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    m_timer->start();
}

void Core::onTimeout()
{
    m_batteryReplies.append(m_coap->get(CoapRequest(QUrl("coap://[2001:db8:c001:f00d:221:2eff:ff00:331d]:5683/sensors/battery"))));
    m_toggleReplies.append(m_coap->post(CoapRequest(QUrl("coap://[2001:db8:c001:f00d:221:2eff:ff00:331d]:5683/actuators/toggle"))));
}

void Core::onReplyFinished(CoapReply *reply)
{
    if (m_toggleReplies.contains(reply)) {

        m_toggleReplies.removeAll(reply);

        if (reply->error() != CoapReply::NoError) {
            qDebug() << "Toggle LED reply error" << reply->errorString();
            reply->deleteLater();
            return;
        }

        if (reply->statusCode() != CoapPdu::Content) {
            qDebug() << "Toggle LED return unexpected status code" << reply;
            reply->deleteLater();
            return;
        }

    } else if (m_batteryReplies.contains(reply)) {

        m_batteryReplies.removeAll(reply);

        if (reply->error() != CoapReply::NoError) {
            qDebug() << "Get battery reply error" << reply->errorString();
            reply->deleteLater();
            return;
        }

        if (reply->statusCode() != CoapPdu::Content) {
            qDebug() << "Get battery return unexpected status code" << reply;
            reply->deleteLater();
            return;
        }
        qDebug() << "Battery" << reply->payload();
    }

    // Note: please don't forget to delete the reply
    reply->deleteLater();
}


