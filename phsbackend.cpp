
#ifdef Q_OS_WIN
#	define _WINNT_WIN32 0x0775
#endif

#include <iostream>

#include <QNetworkInterface>

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#if defined(Q_OS_WIN)
#	include <Ws2tcpip.h>
#	include <Ws2def.h>
#	define AI_V4MAPPED 0x0800
#	define TCP_KEEPIDLE 5
#	define TCP_KEEPCNT 10
#	define TCP_KEEPINTVL 5
#elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
#	include <sys/socket.h>
#	include <netdb.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <netinet/tcp.h>
#endif


#include "phsbackend.h"


#define PHS_MAX_FRAME_LEN 128*1024

#define NOT_IN_FRAME ((qint32)(-1))

#define MAX_PHS_TID 0x7FFFFFFF


QByteArray SimpleAuthenticator::getAuthResponse(QByteArray const& nonce) {
    static const char* sep = ":";
    hash.reset();
    hash.addData(name.toUtf8());
    hash.addData(sep, 1);
    hash.addData(nonce);
    hash.addData(sep, 1);
    hash.addData(password.toUtf8());
    return hash.result();
}

class PHSBackend::Group {
public:
    PHSBackend* owner;
    QMap<TID, QObject*> objects;

    inline Group(PHSBackend* aOwner) : owner(aOwner) { }

    ~Group() {
        free();
        owner->groups.remove(this);
    }

    inline void add(TID id, QObject* object) {
        objects.insert(id, object);
    }

    inline void free() {
        for (auto e : objects.keys()) {
            owner->objects.remove(e);
        }
        objects.clear();
    }
};

QString PHSBackend::getIpString()
{
    QString ipInfo;
    QList<QString> possibleMatches;
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    if ( !ifaces.isEmpty() )
    {
      for(int i=0; i < ifaces.size(); i++)
      {
        possibleMatches.clear();
        unsigned int flags = ifaces[i].flags();
        bool isLoopback = (bool)(flags & QNetworkInterface::IsLoopBack);
        bool isP2P = (bool)(flags & QNetworkInterface::IsPointToPoint);
        bool isRunning = (bool)(flags & QNetworkInterface::IsRunning);

        if ( !isRunning ) continue;
        if ( !ifaces[i].isValid() || isLoopback || isP2P ) continue;

        QList<QHostAddress> addresses = ifaces[i].allAddresses(); // lista wszystkich adresów
        if (addresses.size() > 0) { ipInfo += ifaces[i].humanReadableName() + " "; }

        for(int a=0; a < addresses.size(); a++)
        {
          if ( addresses[a] == QHostAddress::LocalHost ) continue;
          if ( !addresses[a].toIPv4Address() ) continue;
          QString ip = addresses[a].toString();
          if ( ip.isEmpty() ) continue;
          bool foundMatch = false;
          for (int j=0; j < possibleMatches.size(); j++) if ( ip == possibleMatches[j] ) { foundMatch = true; break; }
          if ( !foundMatch )
          {
              possibleMatches.push_back(ip);
              QString batch = ip;
              ipInfo += batch;
          }
          if (a < addresses.size() - 1) { ipInfo += " | "; }
        }
        if (ipInfo.right(3) == " | ") { ipInfo = ipInfo.left(ipInfo.size()-3); }
        if (addresses.size() > 0) { ipInfo += "\n"; }
      }
    }

    if (ipInfo.right(1) == "\n") { ipInfo = ipInfo.left(ipInfo.size()-1); }

    return ipInfo;
}


PHSBackend::PHSBackend(QHostAddress const& host, quint16 port, IAuthenticator* authenticator, QObject *parent)
    : QObject(parent)
    , connectionHost(host)
    , connectionPort(port)
    , connectionAuthenticator(authenticator)
    , inFrameLen(NOT_IN_FRAME)
    , identifier()
{
    socket.connectToHost(connectionHost, connectionPort);
}

PHSBackend::PHSBackend(QHostAddress const& host, quint16 port, IAuthenticator* authenticator, QByteArray const & _ident, QObject *parent)
    : QObject(parent)
    , connectionHost(host)
    , connectionPort(port)
    , connectionAuthenticator(authenticator)
    , inFrameLen(NOT_IN_FRAME)
    , identifier(_ident)
{
    socket.connectToHost(connectionHost, connectionPort);
}

void PHSBackend::connectionHandler() {
    QSet<Group*> oldGroups;
    switch(socket.state()) {
        case QAbstractSocket::ConnectedState:
            qInfo() << "Connected";
            for (IListModel* m : lists)
                m->clear();
            oldGroups.swap(groups);
            for (Group* g : oldGroups)
                g->free();
            emit reset();
            emit connected();
            break;
        case QAbstractSocket::UnconnectedState:
            qInfo() << "Unconnected";
            frameToSend.clear();
            inFrameLen = NOT_IN_FRAME;
            emit dataCheck();
            emit noConnection();
            reconnect.start();
            break;
        case QAbstractSocket::ConnectingState:
            qInfo() << "Connecting to " << connectionHost << ":" << connectionPort;
        break;
        default:
        break;
    }
}

void PHSBackend::getBattInfo(QByteArray battery, quint8 currentBat) {
    for (int i = 0; i < 4; i++)
    {
        batteryLevel[i] = battery.at(i);
    }
    emit batteryUpdate(battery, currentBat);
}

void PHSBackend::disconnectSocket(QString const& s = "") {
    if(s != "")
        qCritical() << "GUI forced disconnection, " << s;
    else
        qCritical() << "GUI forced disconnection";
    socket.disconnectFromHost();
}

QByteArray PHSBackend::stringToInternetAddress(const char* str) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char buf[16];

    memset(&hints, 0, sizeof(struct addrinfo));
     hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
     hints.ai_socktype = SOCK_STREAM;
     hints.ai_flags = AI_NUMERICHOST | AI_V4MAPPED;
     hints.ai_protocol = 0;
     hints.ai_canonname = NULL;
     hints.ai_addr = NULL;
     hints.ai_next = NULL;

    int err = getaddrinfo(str, NULL, &hints, &result);
    if (err != 0) {
        std::cerr << "Cannot convert " << str << " to IP address: " << gai_strerror(err) << " (" << err << ")\n";
        return QByteArray();
    }


    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET) {
            memcpy(buf, rp->ai_addr->sa_data + 2, 4);
            freeaddrinfo(result);
            return QByteArray(buf, 4);
        }
        if (rp->ai_family == AF_INET6) {
            memcpy(buf, rp->ai_addr->sa_data + 6, 16);
            freeaddrinfo(result);
            return QByteArray(buf, 16);
        }
    }

    freeaddrinfo(result);

    std::cerr << "Cannot convert " << str << " to IP address\n";
    return QByteArray();
}

PHSBackend::Group* PHSBackend::enterObjectsGroup() {
    PHSBackend::Group* group =  new PHSBackend::Group(this);
    groups.insert(group);
    return group;
}

void PHSBackend::leaveObjectsGroup(PHSBackend::Group* group) {
    if (groups.contains(group)) {
        group->free();
        groups.remove(group);
    }
}

void PHSBackend::registerObject(PHSBackend::Group* group, TID id, QObject* object) {
    objects[id].append(object);
    objectsReverse.insert(object, id);
    if (group)
        group->add(id, object);
}

PHSBackend::TID PHSBackend::registeredObjectId(QObject* object) {
    auto it = objectsReverse.find(object);
    if (it != objectsReverse.end())
        return *it;
    return PHSBackend::NO_PHS_ID;
}

void PHSBackend::registerProc(TID id, IProc* proc) {
    procs.insert(id, proc);
}

void PHSBackend::registerListModel(TID id, IListModel* model) {
    lists.insert(id, model);
}

bool PHSBackend::isConnected() {
    if(socket.state() == QAbstractSocket::ConnectedState)
        return true;
    else
        return false;
}

static char inbuffer[PHS_MAX_FRAME_LEN];
void PHSBackend::onReadyRead() {
    try {
        while (true) {
            if (inFrameLen == NOT_IN_FRAME) {
                if (socket.bytesAvailable() < 4)
                    return;
                QByteArray buf = socket.read(4);
                QDataStream stream(buf);
                qint32 len;
                stream >> len;
                if (len == 0)
                    continue;
                if (len < 0 || len > PHS_MAX_FRAME_LEN)
                    throw new PHSCommunicationException
                        (QString("Bad frame length: %1 (0x%2)").
                         arg(len).
                         arg(len, 0, 16));
                inFrameLen = len;
                //std::cerr << "FRAME LEN " << inFrameLen << std::endl;
            }
            else {
                if (socket.bytesAvailable() < inFrameLen)
                    return;
                //std::cerr << "FRAME RECEIVED, len=" << inFrameLen << std::endl;
                QDataStream stream(socket.read(inFrameLen));
                onProtocolFrame(stream);
                inFrameLen = NOT_IN_FRAME;
            }
        }
    }
    catch(QException const& e) {
        std::cerr << "Exception processing received frame:\n\t" << e.what() << ".\nDisconnecting.\n";
    }
}

#define OPCODE_AUTH 0x01
#define OPAUTH_OK   0x00
#define OPAUTH_REQ  0x01
#define OPAUTH_RSP  0x02

#define OPCODE_CALL 0x02

#define OPCODE_PROP 0x03

#define OPCODE_LIST 0x04
#define OPLIST_CLEAR 0xFF
#define OPLIST_DEL   0xFE


#define TCODE_BYTES  0x10
#define TCODE_STRING 0x20
#define TCODE_INT    0x30
#define TCODE_LONG   0x40
#define TCODE_BOOL   0x50
#define TCODE_MASK   0x70

#define OPBOOL_TRUE 0xFF
#define OPBOOL_FALSE 0x00

static QByteArray getBytes(QDataStream& src, quint8 len);
static void putBytes(QByteArray const& src, QByteArray& dest, quint8 tCode);

static PHSBackend::TID getID(QDataStream& src);
static void putTID(PHSBackend::TID id, QByteArray& dest);

static qint32 getInt(QDataStream& src, quint8 len);
static qint64 getLong(QDataStream& src, quint8 len);
static void putInt(qint64 i, QByteArray& dest, quint8 code);

static void readQVariantVector(QDataStream& stream, int count, QVector<QVariant>& dest);
static void writeQVariantVector(QVector<QVariant> const& src, QByteArray& dest);

void PHSBackend::onProtocolFrame(QDataStream& stream) {
    quint8 opCode;
    quint8 opParam;
    QVector<QVariant> params;
    TID id;
    std::cerr << "onProtocolFrame:";
    while (!stream.atEnd()) {
        params.clear();
        stream >> opCode;
        stream >> opParam;
        /*
        std::cerr << QString("OPCODE %1 (0x%2) OPPAR %3 (0x%4)")
                     .arg(opCode).arg(opCode, 0, 16)
                     .arg(opParam).arg(opParam, 0, 16)
                     .toUtf8().data()
                  << std::endl;
        */
        switch(opCode) {
        case OPCODE_AUTH:
            std::cerr << " OPCODE AUTH,";
            readQVariantVector(stream, opParam, params);
            switch(opParam) {
            case OPAUTH_OK:
                qInfo() << "OK";
                break;
            case OPAUTH_REQ:
                qInfo() << "REQ";
                if (params[0].type() != QVariant::ByteArray)
                    throw PHSCommunicationException("OPAUTH_REQ bad parameter type");
                QByteArray ba;
                sendAuthRspFrame(params[0].toByteArray());
            };
            break;
        case OPCODE_CALL:
        {
            id = getID(stream);
            std::cerr << "OPCODE CALL(" << id << ")" << std::endl;
            readQVariantVector(stream, opParam, params);
            auto proc = procs.find(id);

            switch(id) {
            case 10: // mark location

                break;

            case 11: // unmark location

                break;

            case 12: // set user name

                break;

            case 13: // set andon state

                break;

            case 14: // assign carrier to location

                break;

            case 15: // set cerriet on way

                break;

            case 16: // set source carrier

                break;
            }

            if (proc != procs.end())
                (*proc)->call(params);
        }
            break;
        case OPCODE_PROP:
            id = getID(stream);
            std::cerr << " OPCODE PROP(" << id << "),";

            readQVariantVector(stream, opParam, params);
            if (params.length() != 2)
                throw PHSCommunicationException(QString("Invalid OPCODE_PROP parameters count %1 != %2").arg(opParam).arg(params.size()));
            if (params[0].type() != QVariant::String)
                throw PHSCommunicationException("Invalid OPCODE_PROP parameter type");
        {
            QString propName = params[0].toString();
            QVariant propVal = params[1];
            QVariant propValS = propVal;
            propValS.convert(QVariant::String);
            auto objs = objects.find(id);
            if (objs != objects.end()) {

                /*
                std::cerr << QString("Setting property %1 of object ID %2 to %3")
                             .arg(propName).arg(id).arg(propValS.toString())
                             .toUtf8().data()
                          << std::endl;
                */
                for (auto obj : *objs)
                    obj->setProperty(propName.toUtf8().data(), propVal);
            }
        }
            break;
        case OPCODE_LIST:
            id = getID(stream);
            std::cerr << " OPCODE LIST(" << id << "),";
            {
                auto list = lists.find(id);

                switch (opParam) {
                case OPLIST_CLEAR:
                    std::cerr << "OPLIST_CLEAR" << std::endl;
                    if (list != lists.end())
                        (*list)->clear();
                    break;
                case OPLIST_DEL:
                    std::cerr << "OPLIST_DEL" << std::endl;
                    id = getID(stream);
                    if (list != lists.end())
                        (*list)->deleteRow(id);
                    break;
                default:
                    bool locTableFlag = false;
                    if (id==1)
                    {
                        locTableFlag=true;
                    }
                    id = getID(stream);
                    readQVariantVector(stream, opParam, params);

                    if (list != lists.end())
                    {
                        (*list)->addOrReplaceRow(id, params);
                        if (locTableFlag)
                            emit locDataLoaded();
                    }

                    break;
                };
            }
            break;
        default:
            throw PHSCommunicationException(
                        QString("onProtocolFrame:Bad OPCODE %1 (0x%2)").
                        arg(((int) opCode) & 0xFF).
                        arg(((int) opCode) & 0xFF, 0, 16));
        }
    }
    std::cerr << "\n";
}

void PHSBackend::sendAuthRspFrame(QByteArray const& nonce) {
    static const quint8 opCode = OPCODE_AUTH;
    static const quint8 opParam = identifier.isEmpty() ? 2 : 3;

    frameToSend.append(opCode);
    frameToSend.append(opParam);

    putBytes(connectionAuthenticator->getAuthName().toUtf8(), frameToSend, TCODE_STRING);
    putBytes(connectionAuthenticator->getAuthResponse(nonce), frameToSend, TCODE_BYTES);
    if (! identifier.isEmpty())
        putBytes(identifier, frameToSend, TCODE_BYTES);

    sendFrames();
}

void PHSBackend::callPhsProc(TID id, const QVector<QVariant> &params) {

    const quint8 opCode = OPCODE_CALL;
    const quint8 opParam = params.size();

    /*
    std::cerr << QString("Sending OPCODE CALL(ID%1, #P %2)").arg(id).arg(opParam)
                 .toUtf8().data()
              << std::endl;
    */

    frameToSend.append(opCode);
    frameToSend.append(opParam);
    putTID(id, frameToSend);
    writeQVariantVector(params, frameToSend);
    sendFrames();
}

void PHSBackend::sendFrames() {
    if (frameToSend.length() <= 0 || !isConnected())
        return;
    qInfo() << "SEND_FRAME";
    quint32 fLen = frameToSend.length();
    char bLen[] = {
        (char) ((fLen >> 24) & 0xFF),
        (char) ((fLen >> 16) & 0xFF),
        (char) ((fLen >> 8) & 0xFF),
        (char) (fLen & 0xFF)
    };
    frameToSend.prepend(bLen, 4);
    qint64 sentLen = socket.write(frameToSend);
    if (sentLen < frameToSend.length())
        throw PHSCommunicationException(QString("Cannot send fame (%1 sent, %2 expected)").arg(sentLen).arg(frameToSend.length()));
    frameToSend.clear();
}

static void readQVariantVector(QDataStream& stream, int count, QVector<QVariant>& dest) {
    quint8 code;
    quint8 len;
    while(count-- > 0) {
        stream >> code;
        len = code & ~TCODE_MASK;
        switch(code & TCODE_MASK) {
        case TCODE_BOOL:
            if (len == 0)
                dest.push_back(QVariant(QVariant::Bool));
            else if (len == 1) {
                stream >> code;
                if (code == OPBOOL_TRUE)
                    dest.push_back(QVariant(true));
                else if (code == OPBOOL_FALSE)
                    dest.push_back(QVariant(false));
                else
                    throw PHSCommunicationException("Invalid bool OPCODE");
            }
            else
                throw PHSCommunicationException("Invalid bool data length");
            break;
        case TCODE_BYTES:
            if (len == 0)
                dest.push_back(QVariant(QVariant::ByteArray));
            else
                dest.push_back(getBytes(stream, len));
            break;
        case TCODE_STRING:
            if (len == 0)
                dest.push_back(QVariant(QVariant::String));
            else
                dest.push_back(QString::fromUtf8(getBytes(stream, len)));
            break;
        case TCODE_INT:
            if (len == 0)
                dest.push_back(QVariant(QVariant::Int));
            else {
                int i = getInt(stream, len);
                dest.push_back(QVariant(i));
            }
            break;
        case TCODE_LONG:
            if (len == 0)
                dest.push_back(QVariant(QVariant::LongLong));
            else {
                long long i = getLong(stream, len);
                dest.push_back(QVariant(i));
            }
            break;
        default:
            throw PHSCommunicationException(QString("Undefined TCODE %1 (0x%2)").arg(code).arg(code, 0, 16));
        };
        //std::cerr << "VARIANT " << dest.last().type() << " " << dest.last().toString().toUtf8().data() << std::endl;
    }
}

static void writeQVariantVector(QVector<QVariant> const& src, QByteArray& dest) {
    quint8 tcode;
    for (QVariant const& v : src) {
        switch(v.type()) {
        case QVariant::Bool:
            tcode = TCODE_BOOL;
            if (v.isNull()) {
                dest.append(tcode);
                break;
            }
            tcode |= 0x01;
            dest.append(tcode);
            tcode = v.toBool() ? OPBOOL_TRUE : OPBOOL_FALSE;
            dest.append(tcode);
            break;
        case QVariant::Int:
            tcode = TCODE_INT;
            if (v.isNull()) {
                dest.append(tcode);
                break;
            }
            putInt(v.toInt(), dest, TCODE_INT);
            break;
        case QVariant::LongLong:
            tcode = TCODE_LONG;
            if (v.isNull()) {
                dest.append(tcode);
                break;
            }
            putInt(v.toLongLong(), dest, TCODE_LONG);
            break;
        case QVariant::String:
            tcode = TCODE_STRING;
            if (v.isNull()) {
                dest.append(tcode);
                break;
            }
            putBytes(v.toString().toUtf8(), dest, TCODE_STRING);
            break;
        case QVariant::ByteArray:
            tcode = TCODE_BYTES;
            if (v.isNull()) {
                dest.append(tcode);
                break;
            }
            putBytes(v.toByteArray(), dest, TCODE_BYTES);
            break;
        default:
            throw PHSCommunicationException(QString("Unsupported QVariant TYPE = %1").arg((int) v.type()));
        };
    }
}

static QByteArray getBytes(QDataStream& src, quint8 len) {
    quint8 byte;
    quint32 dataLen = 0;
    while (len-- > 0) {
        src >> byte;
        dataLen <<= 8;
        dataLen |= byte;
    }
    if (dataLen >= PHS_MAX_FRAME_LEN)
        throw PHSCommunicationException(
                QString("getBytes: structure too large: %1 octets (0x%2), %3 bytes len")
                .arg(dataLen)
                .arg(dataLen, 0, 16)
                .arg((int)len)
              );
    qint32 readLen = src.readRawData(inbuffer, dataLen);
    if (readLen < (qint32)dataLen)
        throw PHSCommunicationException(QString("getBytes: unable to receive payload read %u, expected %d").arg(readLen).arg(dataLen));
    return QByteArray(inbuffer, dataLen);
}

static void putBytes(QByteArray const& src, QByteArray& dest, quint8 tCode) {
    quint8 tParam;
    if (src.length() <= 255) {
        tParam = tCode | 1;
        dest.append(tParam);
        tParam = src.length();
        dest.append(tParam);
    }
    else if (src.length() <= 65535) {
        tParam = tCode | 2;
        dest.append(tParam);
        tParam = (src.length() >> 8) & 0xFF;
        dest.append(tParam);
        tParam = src.length() & 0xFF;
        dest.append(tParam);
    }
    else if (src.length() <= 1677215) {
        tParam = tCode | 3;
        dest.append(tParam);
        tParam = (src.length() >> 16) & 0xFF;
        dest.append(tParam);
        tParam = (src.length() >> 8) & 0xFF;
        dest.append(tParam);
        tParam = src.length() & 0xFF;
        dest.append(tParam);
    }
    else
        throw PHSCommunicationException("putBytes for too large array (3 octets exceeded)");
    dest.append(src);
}

static void putTID(PHSBackend::TID id, QByteArray& dest) {
    if (id > MAX_PHS_TID)
        throw PHSCommunicationException(
                QString("putTID: PHS ID %1 (0x%2) too large")
                .arg(id)
                .arg(id, 0, 16)
              );
    if (id > 0x7FFF) {
        dest.append(((id >> 24) & 0xFF) | 0x80);
        dest.append(((id >> 16) & 0xFF));
        dest.append(((id >> 8) & 0xFF));
        dest.append(id & 0xFF);
    }
    else {
        dest.append(((id >> 8) & 0x7F));
        dest.append(id & 0xFF);
    }
}

static PHSBackend::TID getID(QDataStream& src) {
    quint16 t;
    src >> t;
    if (t & 0x8000) {
        PHSBackend::TID ret = (t & 0x7FFF);
        ret <<= 16;
        src >> t;
        ret |= t;
        return ret;
    }
    else
        return t;
}


static qint32 getInt(QDataStream& src, quint8 len) {
    qint8 byte;
    src >> byte;
    len--;
    qint32 data = byte;
    while (len-- > 0) {
        src >> byte;
        data <<= 8;
        data |= ((qint32) byte) & 0xFF;
    }
    return data;
}

static qint64 getLong(QDataStream& src, quint8 len) {
    qint8 byte;
    src >> byte;
    len--;
    qint64 data = byte;
    while (len-- > 0) {
        src >> byte;
        data <<= 8;
        data |= ((qint64) byte) & 0xFF;
    }
    return data;
}


static void putInt(qint64 i, QByteArray& dest, quint8 code) {
    quint8 len = 1;
    qint64 range = 0x80;
    while (len < 8 && (i < -range || i > range-1)) {
        len++;
        range <<= 8;
    }

    code |= len;
    dest.append(code);
    while (len-- > 0) {
        quint8 b = (i >> (8*len)) & 0xFF;
        dest.append(b);
    }
}



PHSCommunicationException::PHSCommunicationException(QString const& message)
    : msg(message.toUtf8())
{
    //static void* buffer[1024];
    //int nptrs = backtrace(buffer, 1024);
    //backtrace_symbols_fd(buffer, nptrs, 2);
}
