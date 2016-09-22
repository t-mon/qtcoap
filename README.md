# QtCoap

Constrained Application Protocol (CoAP) library written in Qt.

The QtCoap lib allowes you communicate with a CoAP server according to the [RFC7252](https://tools.ietf.org/html/rfc7252) standard.

=====================================================================
## Lib features

* Queue for nonblocking multiple requests (FIFO) for each host
* DNS resolving for URLs
* GET, PUT, POST, DELETE methods
* Piggybacked requests
* Separated requests (async)
* Non-confirmable requests
* Extended option delta/lenght
* Blockwise transfers [draft-ietf-core-block-19](https://tools.ietf.org/html/draft-ietf-core-block-19)
* Observing resources [RFC7641](https://tools.ietf.org/html/rfc7641)
* [Combining Observe and Block2](https://tools.ietf.org/html/draft-ietf-core-block-19#section-3.4)
* CoRE (Constrained RESTful Environments) link format parser [RFC6690](http://tools.ietf.org/html/rfc6690)
* Tests using the online CoAP server [coap://coap.me/](http://coap.me/) and [coap://vs0.inf.ethz.ch/](http://vs0.inf.ethz.ch/)

=====================================================================
## The coap-cli tool

    $ coap-cli --help
    
    Usage: ./coap-cli [options] [method] url

    coap-cli is a command line tool which allowes to interact with a CoAP server.
    Version: 1.0.0
    Copyright © 2016 Simon Stürz <stuerz.simon@gmail.com>
    Released under the GNU GENERAL PUBLIC LICENSE Version 3.
    
    Options:
      -h, --help               Displays this help.
      -p, --payload <payload>  The payload data you want to send.
      -o, --observe            Observe the given resource.
      -d, --discover           Discover the resources of the server.
      -v, --verbose            Print the whole communication process.
    
    Arguments:
      method                   Method you want to use. Possible values are:
                                  get (default)
                                  put
                                  post
                                  delete
      url                      URL of the CoAP resource.


### Examples

* Discover a resource:

        $ coap-cli -d coap://vs0.inf.ethz.ch

* GET
 
        $ coap-cli get coap://vs0.inf.ethz.ch

    > Alternative: `$ coap-cli coap://vs0.inf.ethz.ch`

* POST
    
        $ coap-cli post coap://vs0.inf.ethz.ch/test -p "Hello world!"
        
* PUT
    
        $ coap-cli put coap://vs0.inf.ethz.ch/test -p "Hello world!"
    
* DELETE
    
        $ coap-cli delete coap://vs0.inf.ethz.ch/test
    
* Observe
    
        $ coap-cli --observe coap://vs0.inf.ethz.ch/obs
    
=====================================================================
## Use QtCoap in your project

#### Using the `libqtcoap`

You can checkout the `coapclient.pro` file or the `coaptests.pro` to see how the lib will be included into your project:

        LIBS += -L/path/to/the/lib/libqtcoap/ -lqtcoap
        INCLUDEPATH += /path/to/this/repository/coap

#### Using the `coap.pri` (like `coap-cli`)

You can copy the `coap` folder into your Qt project structure and include the `coap.pri` file in your project file:

        include(coap/coap.pri)

=====================================================================
## Example
This example shows you the basic usage of the QtCoap lib:

```cpp

#include "coapnetworkaccessmanager.h"

MyClass::MyClass(QObject *parent) : 
  QObject(parent)
{
  CoapNetworkAccessManager *coap = new CoapNetworkAccessManager(this);
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

For more examples please checkout the coaptests and the `coap-cli` source code. 
There is a test case for almost every resource on [coap://coap.me/](http://coap.me/) and observing on [coap://vs0.inf.ethz.ch/](http://vs0.inf.ethz.ch/).

=====================================================================
## License

QtCoap is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3 of the License. 

You should have received a copy of the GNU General Public License along with QtCoap. If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/). 

