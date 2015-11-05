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

Core::Core(QObject *parent) : QObject(parent)
{
    m_coap = new Coap(this);
    connect(m_coap, &Coap::replyFinished, this, &Core::onReplyFinished);

    CoapRequest request(QUrl("coap://coap.me/hello"));
    qDebug() << request.url().toString();

    CoapReply *reply = m_coap->get(request);

    if (reply->isFinished()) {
        qDebug() << "---------------------------------------";
        if (reply->error() != CoapReply::NoError) {
            qDebug() << "Reply finished with error" << reply->errorString();
        } else {
            qDebug() << "Reply finished";
            qDebug() << reply->payload();
        }
        reply->deleteLater();
    }

}

void Core::onReplyFinished(CoapReply *reply)
{
    qDebug() << "---------------------------------------";
    if (reply->error() != CoapReply::NoError) {
        qDebug() << "Reply finished with error" << reply->errorString();
    } else {
        qDebug() << "Reply finished";
        qDebug() << reply->payload();
    }
    reply->deleteLater();
}


