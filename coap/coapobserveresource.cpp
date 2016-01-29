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

#include "coapobserveresource.h"

CoapObserveResource::CoapObserveResource()
{
}

CoapObserveResource::CoapObserveResource(const QUrl &url, const QByteArray &token):
    m_url(url),
    m_token(token)
{
}

CoapObserveResource::CoapObserveResource(const CoapObserveResource &other)
{
    m_url = other.url();
    m_token = other.token();
}

QUrl CoapObserveResource::url() const
{
    return m_url;
}

QByteArray CoapObserveResource::token() const
{
    return m_token;
}
