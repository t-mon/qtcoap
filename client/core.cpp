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

    ping();
    separated();
    hello();
}

void Core::ping()
{
    // request data
    CoapRequest request(QUrl("coap://coap.me/"));
    qDebug() << request.url().toString();

    m_pingReply = m_coap->ping(request);

    // check if the reply is allready finished (error case or for NonConfirmable reply)
    if (m_pingReply->isFinished()) {
        qDebug() << "---------------------------------------";
        if (m_pingReply->error() != CoapReply::NoError) {
            qDebug() << "Reply finished with error" << m_pingReply->errorString();
        } else {
            qDebug() << "Reply finished";
        }

        // Note: please don't forget to delete the reply
        m_pingReply->deleteLater();
    }
}

void Core::hello()
{
    // request data
    CoapRequest request(QUrl("coap://coap.me/hello"));
    qDebug() << request.url().toString();

    m_helloReply = m_coap->get(request);

    // check if the reply is allready finished (error case or for NonConfirmable reply)
    if (m_helloReply->isFinished()) {
        qDebug() << "---------------------------------------";
        if (m_helloReply->error() != CoapReply::NoError) {
            qDebug() << "Reply finished with error" << m_helloReply->errorString();
        } else {
            qDebug() << "Reply finished";
        }

        // Note: please don't forget to delete the reply
        m_helloReply->deleteLater();
    }
}

void Core::separated()
{
    // request data
    CoapRequest request(QUrl("coap://coap.me/separate"));
    qDebug() << request.url().toString();

    m_separatedReply = m_coap->get(request);

    // check if the reply is allready finished (error case or for NonConfirmable reply)
    if (m_separatedReply->isFinished()) {
        qDebug() << "---------------------------------------";
        if (m_separatedReply->error() != CoapReply::NoError) {
            qDebug() << "Reply finished with error" << m_separatedReply->errorString();
        } else {
            qDebug() << "Reply finished";
        }

        // Note: please don't forget to delete the reply
        m_separatedReply->deleteLater();
    }
}

void Core::onReplyFinished(CoapReply *reply)
{
    qDebug() << "---------------------------------------";
    if (reply->error() != CoapReply::NoError) {
        qDebug() << "Reply finished with error" << reply->errorString();
        reply->deleteLater();
        return;
    }

    if (reply == m_pingReply) {
        qDebug() << "Reply finished:";
        qDebug() << m_pingReply;
        qDebug() << "Pong!";
        qDebug() << "=======================================";
    } else if (reply == m_helloReply) {
        qDebug() << "Reply finished:";
        qDebug() << m_helloReply;
        qDebug() << "Server says ->" << m_helloReply->payload();
        qDebug() << "=======================================";
    } else if (reply == m_separatedReply) {
        qDebug() << "Reply finished:";
        qDebug() << m_separatedReply;
        qDebug() << "Separated finished";
        qDebug() << "=======================================";
    }

    // Note: please don't forget to delete the reply
    reply->deleteLater();
}


