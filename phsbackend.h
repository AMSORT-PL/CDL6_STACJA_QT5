#ifndef PHSBACKEND_H
#define PHSBACKEND_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QTcpSocket>
#include <QTimer>
#include <QVector>
#include <QMap>
#include <QSet>
#include <QCryptographicHash>
#include <QDataStream>
#include <QException>
#include <QAbstractTableModel>
#include <QHostAddress>
#include <QProcess>
#include "confighandler.h"

#define DEFAULT_PHS_RECONNECT_INTERVAL 15000 /* ms */

class PHSBackend : public QObject
{
     Q_OBJECT
public:
    typedef quint32 TID;
    static constexpr TID NO_PHS_ID = (TID)(-1);
    virtual QString getIpString();

    class IProc {
    public:
        virtual void call(QVector<QVariant> const& params) = 0;
        virtual ~IProc() { }
    };

    class IListModel {
    public:
        virtual void clear() = 0;
        virtual void addOrReplaceRow(TID id, QVector<QVariant> const& cols) = 0;
        virtual void deleteRow(TID id) = 0;

        virtual ~IListModel() { }
    };

    class IAuthenticator {
    public:
        virtual QByteArray getAuthResponse(QByteArray const& nonce) = 0;
        virtual QString const& getAuthName() const = 0;
        virtual ~IAuthenticator() { }
    };

    class Group;
    bool connectionOn;

signals:
    void connectionStatus(bool connected);
    void reset();
    void locDataLoaded();
    void dataCheck();
    void wifiUpdate(int value);
    void noWifiSignal();
    void minimizeMap();
    void batteryUpdate(QByteArray battery, quint8 currentBat);
    void noConnection();
    void connected();

    void markLocation(int locationId);
    void unmarkLocation();
    void setUsername(QString name);
    void sentAndonState(bool yellow, bool red);
    void assignCarrierToLocation(QString carrierName, int locationId);
    void setCarriersOnWay(int quantity);
    void setSourceCarrier(QString name);

public:

    PHSBackend(QHostAddress const& host, quint16 port, IAuthenticator* authenticator, QByteArray const& identifier, QObject *parent = 0);

    Group* enterObjectsGroup(); // NOTE: PhsBackand is not the owner of the group, you have to do delete group;
    void leaveObjectsGroup(Group* group); // NOTE: Idempotent. You may also do delete group;

    void registerObject(Group* group, TID id, QObject* object);
    inline void registerObject(TID id, QObject* object) {
        registerObject(0, id, object);
    }
    TID registeredObjectId(QObject* object);
    inline QObject* getRegisteredObject(TID id) {
        return objects.contains(id) ? objects[id][0] : nullptr;
    }

    void registerProc(TID id, IProc* proc);

    void registerListModel(TID id, IListModel* model);

    void callPhsProc(TID id, QVector<QVariant> const& params);

    void startConnection();
    void sendFrames();
    bool isConnected();

    /** Conversion form IP (either v4 or v6) literal to its byte representation.
     *
     *  Completely lacking in Qt library.
     */
    static QByteArray stringToInternetAddress(const char* str);
private:
    const QHostAddress connectionHost;
    const quint16 connectionPort;
    IAuthenticator* const connectionAuthenticator;
    ConfigHandler* config;
    QTcpSocket socket;
    QMap<TID, IProc*> procs;
    QMap<TID, QVector<QObject*> > objects;
    QMap<QObject*, TID> objectsReverse;
    QMap<TID, IListModel*> lists;
    QSet<Group*> groups;
    QTimer timer;
    qint32 inFrameLen;
    enum State { DISCONNECTED, CONNECTING, AUTHENTICATING, CONNECTED };
    State connectionState;
    QByteArray frameToSend;
    QByteArray identifier;

    int batteryLevel[4];
    int wifiStrength;

    QProcess *process;
    QTimer reconnect;

    void onAuthenticated();
    void onProtocolFrame(QDataStream& stream);

    void sendAuthRspFrame(QByteArray const& nonce);
public slots:
    void getBattInfo(QByteArray battery, quint8 currentBat);
    void sendEan(QString code);
    void markLocationRequest(int locationId);
    void inventoryToteRequest();
    void inventoryProductRequest();
    void markKJRequest();
    void closeCarrierRequest();
    void reprintRequest();
    void runReceivingContainerRequest();
    void inventorySrcContainerRequest();
    /*
     *
     *     connect(buttonsWidget, &LayoutBase::inventory_tote_clicked_forward, phs, &PHSBackend::inventoryToteRequest);
    connect(buttonsWidget, &LayoutBase::inventory_product_clicked_forward, phs, &PHSBackend::inventoryProductRequest);
    connect(buttonsWidget, &LayoutBase::mark_KJ_clicked_forward, phs, &PHSBackend::markKJRequest);
    connect(buttonsWidget, &LayoutBase::close_carrier_clicked_forward, phs, &PHSBackend::closeCarrierRequest);
    connect(buttonsWidget, &LayoutBase::reprint_clicked_forward, phs, &PHSBackend::ReprintRequest);
    connect(buttonsWidget, &LayoutBase::run_receiving_container_clicked_forward, phs, &PHSBackend::runReceivingContainerRequest);
    connect(buttonsWidget, &LayoutBase::inventory_src_container_clicked_forward, phs, &PHSBackend::inventorySrcContainerRequest);
     */
private slots:
    void onReadyRead();
    void connectionHandler();
    void disconnectSocket(QString const& s);
};

class PHSCommunicationException : public QException {
    QByteArray msg;
public:
    PHSCommunicationException(QString const& message);
    virtual inline void raise() const { throw *this; }
    virtual inline PHSCommunicationException* clone() const { return new PHSCommunicationException(*this); }
    virtual inline const char* what() const noexcept { return msg.data(); }
};

class SimpleAuthenticator : public PHSBackend::IAuthenticator {
public:
    SimpleAuthenticator(QString const& aName, QString const& aPassword, QCryptographicHash::Algorithm algo = QCryptographicHash::Algorithm::Sha512)
        : name(aName)
        , password(aPassword)
        , hash(algo)
    { }

    virtual QByteArray getAuthResponse(QByteArray const& nonce);
    virtual QString const& getAuthName() const {
        return name;
    }
private:
    const QString name;
    const QString password;
    QCryptographicHash hash;
};

template<typename _T>
class PHSClosureWrapper : public PHSBackend::IProc {
public:
    typedef void (_T::* TMethod)(QVector<QVariant> const&);
    inline PHSClosureWrapper(_T* object, TMethod method)
        : _object(object)
        , _method(method)
    { }

    virtual void call(QVector<QVariant> const& params) {
        (_object->*_method)(params);
    }

private:
    _T* const _object;
    TMethod const _method;

    friend
    inline PHSBackend::IProc* PHSMethodProc(_T* object, void (_T::* method)(QVector<QVariant> const&)) {
        return new PHSClosureWrapper<_T>(object, method);
    }
};

template<typename _T>
class PHSClosureVoidWrapper : public PHSBackend::IProc {
public:
    typedef void (_T::* TMethod)();
    inline PHSClosureVoidWrapper(_T* object, TMethod method)
        : _object(object)
        , _method(method)
    { }

    virtual void call(QVector<QVariant> const& params) {
        Q_UNUSED(params)
        (_object->*_method)();
    }

private:
    _T* const _object;
    TMethod const _method;

    friend
    inline PHSBackend::IProc* PHSMethodProc(_T* object, void (_T::* method)()) {
        return new PHSClosureVoidWrapper<_T>(object, method);
    }
};

template<typename _T>
class PHSCallableWrapper : public PHSBackend::IProc {
    _T callable;

    explicit inline PHSCallableWrapper(_T const& v) : callable(v) { }

    virtual void call(QVector<QVariant> const& params) {
        callable(params);
    }

    template<typename _S>
    friend
    inline PHSBackend::IProc* PHSCallableProc(_S const& callable) {
        return new PHSCallableWrapper<_S>(callable);
    }
};

class transfer : public QObject
{
     Q_OBJECT

public:
    int a;
    QString b;


};

#endif // PHSBACKEND_H
