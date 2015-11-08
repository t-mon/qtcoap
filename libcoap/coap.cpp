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

#include "coap.h"
#include "coappdu.h"
#include "coapoption.h"

Coap::Coap(QObject *parent) :
    QObject(parent)
{
    m_socket = new QUdpSocket(this);

    if (!m_socket->bind(QHostAddress::Any, 5683)) {
        qWarning() << "Could not bind " << m_socket->errorString();
        return;
    }
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

CoapReply *Coap::ping(const CoapRequest &request)
{
    CoapReply *reply = new CoapReply(request, this);
    reply->setRequestMethod(CoapPdu::Empty);

    connect(reply, &CoapReply::timeout, this, &Coap::onReplyTimeout);
    connect(reply, &CoapReply::finished, this, &Coap::onReplyFinished);

    if (request.url().scheme() != "coap") {
        reply->setError(CoapReply::InvalidUrlSchemeError);
        reply->m_isFinished = true;
        return reply;
    }

    int lookupId = QHostInfo::lookupHost(request.url().host(), this, SLOT(hostLookupFinished(QHostInfo)));
    m_runningHostLookups.insert(lookupId, reply);
    return reply;
}

CoapReply *Coap::get(const CoapRequest &request)
{
    CoapReply *reply = new CoapReply(request, this);
    reply->setRequestMethod(CoapPdu::Get);

    connect(reply, &CoapReply::timeout, this, &Coap::onReplyTimeout);
    connect(reply, &CoapReply::finished, this, &Coap::onReplyFinished);

    if (request.url().scheme() != "coap") {
        reply->setError(CoapReply::InvalidUrlSchemeError);
        reply->m_isFinished = true;
        return reply;
    }

    int lookupId = QHostInfo::lookupHost(request.url().host(), this, SLOT(hostLookupFinished(QHostInfo)));
    m_runningHostLookups.insert(lookupId, reply);
    return reply;
}

CoapReply *Coap::put(const CoapRequest &request, const QByteArray &data)
{
    CoapReply *reply = new CoapReply(request, this);
    reply->setRequestMethod(CoapPdu::Put);
    reply->setRequestPayload(data);

    connect(reply, &CoapReply::timeout, this, &Coap::onReplyTimeout);
    connect(reply, &CoapReply::finished, this, &Coap::onReplyFinished);

    if (request.url().scheme() != "coap") {
        reply->setError(CoapReply::InvalidUrlSchemeError);
        reply->m_isFinished = true;
        return reply;
    }

    int lookupId = QHostInfo::lookupHost(request.url().host(), this, SLOT(hostLookupFinished(QHostInfo)));
    m_runningHostLookups.insert(lookupId, reply);
    return reply;
}

CoapReply *Coap::post(const CoapRequest &request, const QByteArray &data)
{
    CoapReply *reply = new CoapReply(request, this);
    reply->setRequestMethod(CoapPdu::Post);
    reply->setRequestPayload(data);

    connect(reply, &CoapReply::timeout, this, &Coap::onReplyTimeout);
    connect(reply, &CoapReply::finished, this, &Coap::onReplyFinished);

    if (request.url().scheme() != "coap") {
        reply->setError(CoapReply::InvalidUrlSchemeError);
        reply->m_isFinished = true;
        return reply;
    }

    int lookupId = QHostInfo::lookupHost(request.url().host(), this, SLOT(hostLookupFinished(QHostInfo)));
    m_runningHostLookups.insert(lookupId, reply);
    return reply;
}

CoapReply *Coap::deleteResource(const CoapRequest &request)
{
    CoapReply *reply = new CoapReply(request, this);
    reply->setRequestMethod(CoapPdu::Delete);

    connect(reply, &CoapReply::timeout, this, &Coap::onReplyTimeout);
    connect(reply, &CoapReply::finished, this, &Coap::onReplyFinished);

    if (request.url().scheme() != "coap") {
        reply->setError(CoapReply::InvalidUrlSchemeError);
        reply->m_isFinished = true;
        return reply;
    }

    int lookupId = QHostInfo::lookupHost(request.url().host(), this, SLOT(hostLookupFinished(QHostInfo)));
    m_runningHostLookups.insert(lookupId, reply);
    return reply;
}

void Coap::sendRequest(CoapReply *reply, const bool &lookedUp)
{
    CoapPdu pdu;
    pdu.setMessageType(reply->request().messageType());
    pdu.setStatusCode(reply->requestMethod());
    pdu.createMessageId();
    pdu.createToken();

    if (lookedUp)
        pdu.addOption(CoapOption::UriHost, reply->request().url().host().toUtf8());

    QStringList urlTokens = reply->request().url().path().split("/");
    urlTokens.removeAll(QString());

    foreach (const QString &token, urlTokens)
        pdu.addOption(CoapOption::UriPath, token.toUtf8());

    if (reply->request().url().hasQuery())
        pdu.addOption(CoapOption::UriQuery, reply->request().url().query().toUtf8());

    if (reply->requestMethod() == CoapPdu::Get)
        pdu.addOption(CoapOption::Block2, CoapPduBlock::createBlock(0));

    if (reply->requestMethod() == CoapPdu::Post || reply->requestMethod() == CoapPdu::Put)
        pdu.addOption(CoapOption::ContentFormat, QByteArray(1, ((quint8)reply->request().contentType())));

    pdu.setPayload(reply->requestPayload());

    QByteArray pduData = pdu.pack();
    reply->setRequestData(pduData);
    reply->m_lockedUp = lookedUp;
    reply->m_timer->start();

    qDebug() << "--->" << pdu;

    // create reply
    if (reply->request().messageType() == CoapPdu::NonConfirmable) {
        sendData(reply->hostAddress(), 5683, pduData);
        reply->setFinished();
    } else {
        m_repliesId.insert(pdu.messageId(), reply);
        m_repliesToken.insert(pdu.token(), reply);
        sendData(reply->hostAddress(), 5683, pduData);
    }
}

void Coap::sendData(const QHostAddress &hostAddress, const quint16 &port, const QByteArray &data)
{
    m_socket->writeDatagram(data, hostAddress, port);
}

void Coap::sendCoapPdu(const QHostAddress &hostAddress, const quint16 &port, const CoapPdu &pdu)
{
    qDebug() << "--->" << pdu;
    m_socket->writeDatagram(pdu.pack(), hostAddress, port);
}

void Coap::processResponse(const CoapPdu &pdu)
{
    qDebug() << "<---" << pdu;

    CoapReply *reply = m_repliesId.take(pdu.messageId());
    if (!pdu.isValid()) {
        qWarning() << "Got invalid PDU";
        reply->setError(CoapReply::InvalidPduError);
        reply->setFinished();
        return;
    }

    // check if the message is a response to a reply (message id based check)
    if (reply) {
        // check if this is an empty ACK response (which indicates a separated response)
        if (pdu.statusCode() == CoapPdu::Empty && pdu.messageType() == CoapPdu::Acknowledgement) {
            reply->m_timer->stop();
            qDebug() << "Got empty ACK. Data will be sent separated.";
            return;
        }
        // check if this is a blocked pdu
        if (pdu.statusCode() == CoapPdu::Content && pdu.messageType() == CoapPdu::Acknowledgement && pdu.isBlock()) {
            qDebug() << "got block" << pdu.block().blockNumber();
            reply->appendPayloadData(pdu.payload());

            // check if this was the last block
            if (!pdu.block().moreFlag()) {
                qDebug() << "Block finished";
                reply->setStatusCode(pdu.statusCode());
                reply->setContentType(pdu.contentType());
                reply->setFinished();
                return;
            }

            CoapPdu nextBlockRequest;
            nextBlockRequest.setContentType(reply->request().contentType());
            nextBlockRequest.setMessageType(reply->request().messageType());
            nextBlockRequest.setStatusCode(reply->requestMethod());
            nextBlockRequest.setMessageId(pdu.messageId() + 1);
            nextBlockRequest.setToken(pdu.token());

            if (reply->m_lockedUp)
                nextBlockRequest.addOption(CoapOption::UriHost, reply->request().url().host().toUtf8());

            QStringList urlTokens = reply->request().url().path().split("/");
            urlTokens.removeAll(QString());

            foreach (const QString &token, urlTokens)
                nextBlockRequest.addOption(CoapOption::UriPath, token.toUtf8());

            if (reply->request().url().hasQuery())
                nextBlockRequest.addOption(CoapOption::UriQuery, reply->request().url().query().toUtf8());

            nextBlockRequest.addOption(CoapOption::Block2, CoapPduBlock::createBlock(pdu.block().blockNumber() + 1, 2, false));

            QByteArray pduData = nextBlockRequest.pack();
            reply->setRequestData(pduData);
            reply->m_timer->start();

            m_repliesId.insert(nextBlockRequest.messageId(), reply);

            qDebug() << "--->" << nextBlockRequest;
            sendData(reply->hostAddress(), 5683, pduData);
            return;
        }

        // Piggybacked response
        m_repliesToken.remove(pdu.token());
        reply->setStatusCode(pdu.statusCode());
        reply->setContentType(pdu.contentType());
        reply->appendPayloadData(pdu.payload());
        reply->setFinished();
        return;
    } else {
        // check if we know the message by token (message token based check)
        reply = m_repliesToken.take(pdu.token());
        if (reply) {
            // Separate Response
            CoapPdu responsePdu;
            responsePdu.setMessageType(CoapPdu::Acknowledgement);
            responsePdu.setStatusCode(CoapPdu::Empty);
            responsePdu.setMessageId(pdu.messageId());
            sendCoapPdu(reply->hostAddress(), 5683, responsePdu);

            reply->setStatusCode(pdu.statusCode());
            reply->setContentType(pdu.contentType());
            reply->appendPayloadData(pdu.payload());
            reply->setFinished();
            return;
        }
    }

    qDebug() << "Got message without request";
}

void Coap::hostLookupFinished(const QHostInfo &hostInfo)
{
    CoapReply *reply = m_runningHostLookups.take(hostInfo.lookupId());;

    if (hostInfo.error() != QHostInfo::NoError) {
        qDebug() << "Host lookup for" << reply->request().url().host() << "failed:" << hostInfo.errorString();
        reply->setError(CoapReply::HostNotFoundError);
        reply->setFinished();
        return;
    }

    QHostAddress hostAddress;
    foreach (const QHostAddress &address, hostInfo.addresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            hostAddress = address;
            break;
        }
    }
    reply->setHostAddress(hostAddress);

    // check if the url had to be looked up
    if (reply->request().url().host() != hostAddress.toString()) {
        qDebug() << reply->request().url().host() << " -> " << hostAddress.toString();
        sendRequest(reply, true);
    } else {
        sendRequest(reply);
    }
}

void Coap::onReadyRead()
{
    QHostAddress hostAddress;
    QByteArray data;
    quint16 port;

    while (m_socket->hasPendingDatagrams()) {
        data.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(data.data(), data.size(), &hostAddress, &port);
    }

    CoapPdu pdu(data);
    processResponse(pdu);
}

void Coap::onReplyTimeout()
{
    CoapReply *reply = qobject_cast<CoapReply *>(sender());
    if (reply->m_retransmissions < 5) {
        qDebug() << QString("Reply timeout: resending message %1/4").arg(reply->m_retransmissions);
    }
    reply->resend();
    m_socket->writeDatagram(reply->requestData(), reply->hostAddress(), 5683);
}

void Coap::onReplyFinished()
{
    CoapReply *reply = qobject_cast<CoapReply *>(sender());
    emit replyFinished(reply);
}
