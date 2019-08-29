#include "pjlink.h"
#include <qcryptographichash.h>
// https://pjlink.jbmia.or.jp/english/data/5-1_PJLink_eng_20131210.pdf


PJLink::PJLink(QObject *parent) : QObject(parent)
{
    port = PJLINK_PORT;
    sock = new tcpsocket(this);
    QObject::connect(sock, SIGNAL(response(QByteArray)), this, SLOT(response(QByteArray)));
}
void PJLink::setPower(bool state) {
    if(state) {
        //send power on
        sendCommand("%1POWR 1");
    } else {
        sendCommand("%1POWR 0");
        // Send power off
    }
}

void PJLink::setVideoMute(bool state) {
    if(state) {
        // Set on
        sendCommand("%1AVMT 11");
    } else {
        // Set off
        sendCommand("%1AVMT 10");
    }
}

void PJLink::setAudioMute(bool state) {
    if(state) {
        // Set on
        sendCommand("%1AVMT 21");
    } else {
        // Set off
        sendCommand("%1AVMT 20");
    }
}

void PJLink::setAVMute(bool state) {
    if(state) {
        // Set on
        sendCommand("%1AVMT 31");
    } else {
        // Set off
        sendCommand("%1AVMT 30");
    }
}

void PJLink::setInput(Projector_Channels input) {
    switch (input) {
    case Channel_DIGITAL:
        sendCommand("%1INPT 31");
    break;
    case Channel_NETWORK:
        sendCommand("%1INPT 51");
    break;
    case Channel_RGB:
        sendCommand("%1INPT 11");
    break;
    case Channel_STORAGE:
        sendCommand("%1INPT 41");
    break;
    case Channel_VIDEO:
        sendCommand("%1INPT 22");
    break;
    default:
        break;
    }
}

void PJLink::requestStatus() {
    // request power status
    sendCommand("POWR?");
    // Request input
    sendCommand("INPT?");
}

void PJLink::sendCommand(QString command) {
   //1. Connect and wait for response
    pendingCommand = command;
    sock->connect(ipAddress, port);
}

void PJLink::setIpAddress(QString ip) {
    ipAddress = ip;
}

void PJLink::setPassword(QString pass) {
    password = pass;
}

void PJLink::setPort(int port) {
    port = port;
}
void PJLink::response(QByteArray msg) {
    qDebug() << "PJLINK::response()" << msg;
    // Parse
    if(msg.contains("PJLINK 1")) {
        // Remove PJLINK 1 , 9 bits
        QByteArray random_number = msg.remove(0, 9);
        random_number.chop(1);
        qDebug() << "PJLink::response() Authentication number " << random_number;
        random_number.append(password.toUtf8());
        qDebug() << "PJLink::response() Random number is" << random_number;
        QByteArray hashedMessage = md5hash(random_number);
        hashedMessage.append(pendingCommand);
        hashedMessage.append('\r');
        sock->send(hashedMessage);
        pendingCommand.clear();
    }
}

QByteArray PJLink::md5hash(QByteArray message) {
    return QCryptographicHash::hash(message,QCryptographicHash::Md5).toHex();
}
