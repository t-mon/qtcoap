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

#include "corelinkparser.h"

#include <QDebug>

CoreLinkParser::CoreLinkParser(const QByteArray &data, QObject *parent) :
    QObject(parent),
    m_data(data)
{
    QList<QByteArray> linkList = data.split(',');

    foreach (const QByteArray &linkLine, linkList) {
        QList<QByteArray> valueList = linkLine.split(';');
        CoreLink link;
        foreach (const QByteArray &value, valueList) {
            if (value.startsWith("</")) {
                link.setPath(QString(value.mid(2, value.length()-3)));
            } else if (value.startsWith("rt=")) {
                link.setResourceType(QString(value.right(value.length() - 3)).remove('"'));
            } else if (value.startsWith("if=")) {
                link.setInterfaceDescription(QString(value.right(value.length() - 3)).remove('"'));
            } else if (value.startsWith("sz=")) {
                link.setMaximumSize(value.right(value.length() - 3).toInt());
            } else if (value.startsWith("ct=")) {
                link.setContentType(static_cast<CoapPdu::ContentType>(value.right(value.length() - 3).toUInt()));
            }
        }
        m_links.append(link);
    }
}

QList<CoreLink> CoreLinkParser::links() const
{
    return m_links;
}


