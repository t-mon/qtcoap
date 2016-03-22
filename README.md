# QtCoap

Constrained Application Protocol (CoAP) library written in Qt.

The QtCoap lib allowes you communicate with a CoAP server according to the [RFC7252](https://tools.ietf.org/html/rfc7252) standard.

=====================================================================
## Features

* Queue for nonblocking multiple requests (FIFO)
* DNS resolving for URLs
* GET, PUT, POST, DELETE methods
* Piggybacked requests
* Separated requests (async)
* Non-confirmable requests
* Extended option delta/lenght
* Blockwise notifications
* Blockwise transfers [draft-ietf-core-block-18](https://tools.ietf.org/html/draft-ietf-core-block-18)
* Observing resources [RFC7641](https://tools.ietf.org/html/rfc7641)
* CoRE (Constrained RESTful Environments) link format parser [RFC6690](http://tools.ietf.org/html/rfc6690)
* Tests using the online CoAP server [coap://coap.me/](http://coap.me/) and [http://vs0.inf.ethz.ch/]

=====================================================================
## Use QtCoap in your project

#### Using the `libqtcoap`

You can checkout the `coapclient.pro` file or the `coaptests.pro` to see how the lib will be included into your project:

        LIBS += -L/path/to/the/lib/libqtcoap/ -lqtcoap
        INCLUDEPATH += /path/to/this/repository/coap

#### Using the `coap.pri`

You can copy the `coap` folder into your Qt project structure and include the `coap.pri` file in your project file:

        include(coap/coap.pri)

=====================================================================
## Example
This example shows you the basic usage of the QtCoap lib:

```cpp
MyClass::MyClass(QObject *parent) : 
  QObject(parent)
{
  Coap *coap = new Coap(this);
  connect(m_coap, SIGNAL(replyFinished(CoapReply*)), this, SLOT(onReplyFinished(CoapReply*)));

  CoapRequest request(QUrl("coap://coap.me/hello"));
  coap->get(request);
}
```

```cpp
void MyClass::onReplyFinished(CoapReply *reply)
{
  if (reply->error() != CoapReply::NoError) {
    qWarning() << "Reply finished with error" << reply->errorString();
  }
  
  qDebug() << "Reply finished" << reply;
  reply->deleteLater();
}
```

For more examples please checkout the coaptests and the example folder. There is a test case for almost every resource on [coap://coap.me/](http://coap.me/).

=====================================================================
## License

QtCoap is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3 of the License. 

You should have received a copy of the GNU General Public License along with QtCoap. If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/). 

