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

#ifndef CORE_H
#define CORE_H

#include <QUrl>
#include <QObject>
#include <QHostInfo>
#include <QHostAddress>
#include <QTimer>

#include "coap.h"
#include "coappdu.h"
#include "coapreply.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

private:
    QTimer *m_timer;
    Coap *m_coap;

    QList<CoapReply *> m_toggleReplies;
    QList<CoapReply *> m_batteryReplies;



private slots:
    void onTimeout();
    void onReplyFinished(CoapReply *reply);

};

#endif // CORE_H
