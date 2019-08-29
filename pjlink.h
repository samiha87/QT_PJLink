#ifndef PJLINK_H
#define PJLINK_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QByteArray>
#include <QTimer>

#include "tcpsocket.h"
#define PJLINK_PORT 4352
#define PJLINK_CLASS '1'

/* Authentication test on ubuntu
 * 1. Download packet sender
 * 2. Connect to PJLink projector
 * 3. send packet with persistent connection
 * 4. PJlink will reply with PJLINK <class> <random number> <CR>
 * 5. Create MD5 hash of wth the random number and password.
 *    https://www.md5hashgenerator.com/
 *    MD5( (random number) + your password).
 *    The hashed string is constructed as follows <random_numer><your_password>
 * 6. Send the hashed string and command in same packet to Projector
 * 7. Projetor response random response, if PJLINK ERRA is returend. Hashed string was wrong.
 * Example, Password 1323, command %1POWR 1
 * Connect
 * ResponsePJLINK 1 00003a9e
 * MD5 hash(00003a9e1323)
   Send: 3f89cff497594c407e98ab892561b87a%1POWR 1\r
   Response %1POWR=ERR3
   Responded error because projector was already on
*/

class PJLink : public QObject
{
    Q_OBJECT

    enum PJLINK_Response_codes {
        Successfull_execution,
        Undefinied_command,
        Out_of_parameter,
        Unavailable_time,
        Projector_failure
    };

    enum Projector_Channels {
        Channel_RGB,
        Channel_VIDEO,
        Channel_DIGITAL,
        Channel_STORAGE,
        Channel_NETWORK
    };

public:
    explicit PJLink(QObject *parent = nullptr);
    void setPower(bool state);

    void setVideoMute(bool state);
    void setAudioMute(bool state);
    void setAVMute(bool state);

    void setInput(Projector_Channels input);
    void setIpAddress(QString ip);
    void setPassword(QString pass);
    void setPort(int port);

signals:

public slots:
    void response(QByteArray msg);

private:
    QString ipAddress;
    QString password;
    QString pendingCommand;

    bool connected;
    int port;
    tcpsocket *sock;

    void sendCommand(QString cmd);
    void requestStatus();
    QByteArray md5hash(QByteArray message);

    int lampHours;
    bool powerState;


};

#endif // PJLINK_H
