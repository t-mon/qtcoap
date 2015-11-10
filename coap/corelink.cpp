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

#include "corelink.h"

#include <QMetaEnum>

CoreLink::CoreLink() :
    m_resourceType(CoapPdu::TextPlain),
    m_maximumSize(-1)
{
}

QString CoreLink::path() const
{
    return m_path;
}

void CoreLink::setPath(const QString &path)
{
    m_path = path;
}

QString CoreLink::resourceType() const
{
    return m_resourceType;
}

void CoreLink::setResourceType(const QString &resourceType)
{
    m_resourceType = resourceType;
}

QString CoreLink::interfaceDescription() const
{
    return m_interfaceDescription;
}

void CoreLink::setInterfaceDescription(const QString &interfaceDescription)
{
    m_interfaceDescription = interfaceDescription;
}

int CoreLink::maximumSize() const
{
    return m_maximumSize;
}

void CoreLink::setMaximumSize(const int &maximumSize)
{
    m_maximumSize = maximumSize;
}

CoapPdu::ContentType CoreLink::contentType() const
{
    return m_contentType;
}

void CoreLink::setContentType(const CoapPdu::ContentType &contentType)
{
    m_contentType = contentType;
}

QDebug operator<<(QDebug debug, const CoreLink &link)
{
    const QMetaObject &metaObject = CoapPdu::staticMetaObject;
    QMetaEnum contentTypeEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ContentType"));
    debug.nospace() << "CoapLink(" << link.path() << ")" << endl;
    debug.nospace() << "  Resource type: " << link.resourceType() << endl;
    debug.nospace() << "  Content type: " << contentTypeEnum.valueToKey(link.contentType()) << endl;

    if (!link.interfaceDescription().isEmpty())
        debug.nospace() << "  Interface description: " << link.interfaceDescription() << endl;

    if (link.maximumSize() >= 0)
        debug.nospace() << "  Maximum size: " << link.maximumSize() << endl;

    return debug.space();
}
