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

#ifndef COAPPDU_H
#define COAPPDU_H

#include <QDebug>
#include <QObject>

#include "coapoption.h"

// PDU = Protocol Data Unit

/*         0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |Ver| T |  TKL  |      Code     |          Message ID           |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   Token (if any, TKL bytes) ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   Options (if any) ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |1 1 1 1 1 1 1 1|    Payload (if any) ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *  Version (Ver): 2-bit unsigned integer.  Indicates the CoAP version number.
 *  Type (T): 2-bit unsigned integer.  Indicates if this message is of
 *            type Confirmable (0), Non-confirmable (1), Acknowledgement (2), or Reset (3)
 *  Code: 8-bit unsigned integer, split into a 3-bit class (most
 *        significant bits) and a 5-bit detail (least significant bits)
 *  Message ID: 16-bit unsigned integer in network byte order.
 *  Token Length (TKL): 4-bit unsigned integer.
 *
 */

class CoapPduBlock
{
public:
    CoapPduBlock();
    CoapPduBlock(const QByteArray &blockData);

    int blockNumber() const;
    int blockSize() const;
    bool isLastBlock() const;

private:
    int m_blockNumber;
    int m_blockSize;
    bool m_isLastBlock;

};

class CoapPdu : public QObject
{
    Q_OBJECT
    Q_ENUMS(MessageType)
    Q_ENUMS(StatusCode)
    Q_ENUMS(ContentType)

public:

    enum MessageType {
        Confirmable     = 0x00,
        NonConfirmable  = 0x01,
        Acknowledgement = 0x02,
        Reset           = 0x03
    };

    // Methods:       https://tools.ietf.org/html/rfc7252#section-5.8
    // Respond codes: https://tools.ietf.org/html/rfc7252#section-12.1.2
    enum StatusCode {
        Empty                    = 0x00,  // Empty mesage
        Get                      = 0x01,  // Method GET
        Post                     = 0x02,  // Method POST
        Put                      = 0x03,  // Method PUT
        Delete                   = 0x04,  // Method DELETE
        Created                  = 0x41,  // 2.01
        Deleted                  = 0x42,  // 2.02
        Valid                    = 0x43,  // 2.03
        Changed                  = 0x44,  // 2.04
        Content                  = 0x45,  // 2.05
        BadRequest               = 0x80,  // 4.00
        Unauthorized             = 0x81,  // 4.01
        BadOption                = 0x82,  // 4.02
        Forbidden                = 0x83,  // 4.03
        NotFound                 = 0x84,  // 4.04
        MethodNotAllowed         = 0x85,  // 4.05
        NotAcceptable            = 0x86,  // 4.06
        PreconditionFailed       = 0x8c,  // 4.12
        RequestEntityTooLarge    = 0x8d,  // 4.13
        UnsupportedContentFormat = 0x8f,  // 4.15
        InternalServerError      = 0xa0,  // 5.00
        NotImplemented           = 0xa1,  // 5.01
        BadGateway               = 0xa2,  // 5.02
        ServiceUnavailabl        = 0xa3,  // 5.03
        GatewayTimeout           = 0xa4,  // 5.04
        ProxyingNotSupported     = 0xa5   // 5.05
    };

    // https://tools.ietf.org/html/rfc7252#section-12.3
    enum ContentType {
        TextPlain        = 0,
        ApplicationLink  = 40,
        ApplicationXml   = 41,
        ApplicationOctet = 42,
        ApplicationExi   = 47,
        ApplicationJson  = 50
    };

    enum Error {
        NoError,
        InvalidToken,
        InvalidPduSize,
        InvalidOptionDelta,
        InvalidOptionLength,
        UnknownOption
    };

    CoapPdu(QObject *parent = 0);
    CoapPdu(const QByteArray &data, QObject *parent = 0);

    // header fields
    quint8 version() const;
    void setVersion(const quint8 &version);

    MessageType messageType() const;
    void setMessageType(const MessageType &messageType);

    StatusCode statusCode() const;
    void setStatusCode(const StatusCode &statusCode);

    quint16 messageId() const;
    void createMessageId();
    void setMessageId(const quint16 &messageId);

    ContentType contentType() const;
    void setContentType(const ContentType &contentType);

    QByteArray token() const;
    void createToken();
    void setToken(const QByteArray &token);

    // payload
    QByteArray payload() const;
    void setPayload(const QByteArray &payload);

    QList<CoapOption> options() const;
    void addOption(const CoapOption::Option &option, const QByteArray &data);

    CoapPduBlock block() const;
    bool isBlock() const;

    void clear();
    bool isValid() const;
    bool isNull() const;

    QByteArray pack() const;

private:
    quint8 m_version;
    MessageType m_messageType;
    StatusCode m_statusCode;
    quint16 m_messageId;
    ContentType m_contentType;
    QByteArray m_token;
    QByteArray m_payload;
    QList<CoapOption> m_options;

    CoapPduBlock m_block;
    bool m_isBlock;

    Error m_error;

    void unpack(const QByteArray &data);
};

QDebug operator<<(QDebug debug, const CoapPdu &coapPdu);

#endif // COAPPDU_H
