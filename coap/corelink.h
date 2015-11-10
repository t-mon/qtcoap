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

#ifndef CORELINK_H
#define CORELINK_H

#include <QObject>
#include <QDebug>

#include "coappdu.h"

class CoreLink
{
public:
    CoreLink();

    QString path() const;
    void setPath(const QString &path);

    QString resourceType() const;
    void setResourceType(const QString &resourceType);

    QString interfaceDescription() const;
    void setInterfaceDescription(const QString &interfaceDescription);

    int maximumSize() const;
    void setMaximumSize(const int &maximumSize);

    CoapPdu::ContentType contentType() const;
    void setContentType(const CoapPdu::ContentType &contentType);

private:
    QString m_path;
    QString m_resourceType;
    QString m_interfaceDescription;
    int m_maximumSize;
    CoapPdu::ContentType m_contentType;

};

QDebug operator<<(QDebug debug, const CoreLink &link);

#endif // CORELINK_H
