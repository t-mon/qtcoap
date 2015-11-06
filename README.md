# QtCoap
Coap library written in Qt

The QtCoap lib allowes you communicatw with a CoAP (Constrained Application Protocol) server according to the [RFC7252](https://tools.ietf.org/html/rfc7252) standard.

## Features
* DNS resolving for URLs
* GET, PUT, POST, DELETE methods
* Piggybacked requests
* Separated requests
* Non-confirmable requests
* extended option delta/lenght

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
  
  qDebug() << "Reply finished" << reply->payload();
  reply->deleteLater();
}
```
