/* This file is part of Udpraw - A library for UDP transmission.
 *
 * Copyright (C) 2011 Trevor Dodds <@gmail.com trev.dodds>
 *
 * Udpraw is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * Udpraw is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * Udpraw.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UDPRAW_H
#define UDPRAW_H

#include "udpraw_api.h"
#include <string>
#include <vector>
#ifdef _MSC_VER
  // link to ws2_32.lib, ws2_32.dll, see WSAStartup docs.
  #include <winsock2.h>
#else
  #include <netinet/in.h>
#endif

/**
 * Class for sending and receiving UDP packets.
 *
 * You can also get some basic stats, and simulate packet loss for testing
 * purposes. Start by calling either initReceiver or initSender, or both.
 */

class UDPRAW_API Udpraw {
  private:

  protected:

#ifdef _MSC_VER
    WSADATA wsaData;          /**< WSA data used by WSAStartup. */
    SOCKET recvSocket,        /**< Socket to receive from. */
           sendSocket;        /**< Socket for sending. */
    bool initWSA;             /**< True when WSAStartup has been called, false otherwise. */
#else
    int recvSocket, /**< Socket to receive from. */
        sendSocket; /**< Socket for sending. */
#endif

    bool createdRecv, /**< Have we created the receive socket? \see initReceiver */
         createdSend; /**< Have we created the send socket? \see initSender */

    fd_set readSocks;                   /**< Sockets to read from. */
    fd_set writeSocks;                  /**< Sockets to write to. */
    fd_set exceptionSocks;              /**< Sockets to watch for exceptions on. */
    int numSocksReadable;               /**< Number of sockets readable. */

    sockaddr_in recvAddr, /**< Receive from receiveAddr (i.e. specify a port to read from). */
                sendAddr, /**< Send to sendAddr. */
                fromAddr; /**< The address of the sender (according to what we get from recvfrom()). */

    std::vector <std::string> sendIp;                 /**< One or more IPs to send to. */
    std::vector<int>::iterator recv_port_iterator;    /**< Port list iterator. */
    std::vector <int> sendPort;                       /**< One or more ports to send to. */

    // generate some basic stats
    int statSent;   /**< Total bytes sent (that means if sending to multiple addresses, this is the total sent). */
    int statRecvd;  /**< Total bytes received. */

    int lastError;  /**< The last error that occurred. */
    std::string errorPrepend; /**< A string to prepend to error messages. \see writeError() */

    struct {
      float send;
      float recv;
    } simulateLoss; /**< Probability of losing a packet for send/recv. 0-1, 1 = all packets lost. 0 = disable */

    void init(); /**< This is called automatically from initSender/initReceiver. */
    bool simulatePacketLoss(float prob); /**< Returns true if packet should be dropped. False otherwise. */


  public:

    /** Enum for storing error. \see getError() \see writeError() */
    enum UdprawError {
      UDPRAW_NO_ERROR,      /**< Default value, no error. */
      UDPRAW_BIND,          /**< Error binding. */
      UDPRAW_MSGSIZE,       /**< Buffer not big enough for message, some data lost. */
      UDPRAW_NOINIT,        /**< Socket not initialised. */
      UDPRAW_NOIP,          /**< No IP address specified. */
      UDPRAW_NOPORT,        /**< No port specified. */
      UDPRAW_RECV,          /**< Error receiving (not message size). */
      UDPRAW_SELECT,        /**< Error with select. */
      UDPRAW_SEND,          /**< Error sending. */
      UDPRAW_SOCKEXCEPTION, /**< Select detected a socket exception. */
      UDPRAW_UNMATCHED      /**< The sender was initialised with multiple IP addresses and port numbers, but the number of IPs does not match the number of ports. */
    };


    Udpraw(); /**< Constructor. */
    ~Udpraw(); /**< Destructor. Calls closeAndCleanup(). \see closeAndCleanup() */


    int getError(); /**< Get and clear the last error number. */
    std::string getErrorStr(); /**< Get last error message and clear the last error number. */
    std::string getErrorStr(int error); /**< Get error message. \param error the error number */
    void writeError(); /**< Write the error message for the last error to stderr and clear the last error number. This function automatically prepends "Udpraw error: " to the error message. Use writeError(std::string prependMsg) to change this. */
    void writeError(std::string prependMsg); /**< Write the error message for the last error to stderr and clear the last error number. \param prependMsg a string to prepend to the output, e.g.\ "Udpraw error: ", or an empty string */
    void writeError(int error); /**< Write error message to stderr. \param error the error number \see getError() */


    // Call either initReceiver or initSender.
    // For the receiver, you must specify a port, because it binds in the init.
    // For the sender, you can specify a port/ip straight away, or later with the
    // methods below, because it's only used when the data is actually sent.
    // The ip/ports for sending can be changed at any time, see below for details.

    int initReceiver(int port); /**< Initialise receiver. Returns -1 if there was an error binding (and sets error number), 0 on success. \see writeError() */
    void initSender(int port = 0); /**< Initialise sender. You can init sender with or without ip and port info. \param port the port number, optional (can set later) \see setSendIp() \see setSendPort() */
    void initSender(const char* ip, int port = 0); /**< Initialise sender with one ip address. \param ip the ip address to send to \param port the port number, optional (can set later) \see setSendPort() */
    void initSender(std::vector <std::string> ip_list, int port = 0); /**< Initialise sender with a list of IP addresses. \param ip_list a list of IP addresses \param port the port number, optional (can set later) \see setSendPort() */
    void initSender(std::vector <std::string> ip_list, std::vector <int> port_list); /**< Initialise sender with a list of IP addresses and a list of ports. \param ip_list a list of IP addresses \param port_list a list of ports */


    // set/change IPs and ports
    // Can be either one IP and list of ports,
    // one port and list of IPs,
    // or equal size list of IPs and corresponding ports.
    // If they are not equal size then sendRaw will return an error.

    void setSendIp(const char* ip); /**< Set a single IP address to send to. Can have one corresponding port, or a list of ports. \param ip an IP address \see setSendPort() */
    void setSendIp(std::vector <std::string> ip_list); /**< Set a list of IP addresses to send to. Can have one corresponding port, or a list of equal size. \param ip_list a list of IP addresses \see setSendPort() */
    void setSendPort(int port); /**< Set the port to use for sending. Can have one corresponding IP, or a list of IPs. \param port the port number \see setSendIp() */
    void setSendPort(std::vector <int> port_list); /**< Set a list of ports to use for sending. Can be multiple ports for one IP, or a list of equal length to a list of IP addresses, one port for each IP. \param port_list a list of port numbers \see setSendIp() */

    /**
     * Perform select call for read, write and exception sockets.
     *
     * Called by the application so it can be done once for send and receive.
     * Stores the number of sockets accessible.
     *
     * \returns 0 on success, -1 if there was an error with the select call, -2 if a socket exception was detected
     * (errors also set the error number to UDPRAW_SELECT or UDPRAW_SOCKEXCEPTION accordingly)
     *
     * \see writeError()
     */
    int doSelect();

    /**
     * Receive a UDP packet.
     *
     * \param[out] buf a pointer to a buffer to store data
     * \param len the length of the buffer, should be big enough for one message (if it's not we won't crash but some data will be lost and an error will be returned)
     * \param select true if the application wants to go by the results of the select call (app must call doSelect() first), false if the application wants to block until next packet is received
     *
     * \returns the number of bytes received
     * \returns 0 if we have no data (either socket will block and we've set select to true, or we're simulating packet loss and this one was dropped)
     * \returns -1 if the buffer was detected as too small for the message (some data was lost) and sets the error number to UDPRAW_MSGSIZE
     * \returns -2 for any other error (and sets error number to either UDPRAW_NOINIT or UDPRAW_RECV accordingly)
     *
     * \see doSelect()
     * \see writeError()
     * \see setSimulateLossRecv()
     */
    int recvRaw(char* buf, int len, bool select = true);

    /**
     * Send a UDP packet.
     *
     * \param buf a pointer to data to send
     * \param len the length of the buffer
     * \param select true if the application wants to go by the results of the select call (app must call doSelect() first), false if the application wants to block until next packet is received
     *
     * \returns the minimum bytes sent to each destination, 0 if packet was dropped (if simulating packet loss) or -1 on error (and sets error number to either UDPRAW_NOINIT, UDPRAW_NOPORT,
     * UDPRAW_NOIP, UDPRAW_UNMATCHED, or UDPRAW_SEND accordingly)
     *
     * \see doSelect()
     * \see writeError()
     * \see setSimulateLossSend()
     */
    int sendRaw(const char* buf, int len, bool select = true);


    int getStatSent(); /**< Get total bytes sent (if sending to > 1 ip this will count total sent). \see setStatSent() */
    int getStatRecvd(); /**< Get total bytes received. \see setStatRecvd() */
    void setStatSent(int bytes); /**< Set bytes sent, e.g.\ reset to 0. \see getStatSent() */
    void setStatRecvd(int bytes); /**< Set bytes received, e.g.\ reset to 0. \see getStatRecvd() */


    void setSimulateLossSend(float probabilityofloss); /**< Simulate packet loss by specifying probability of loss 0-1, 1=lose all! Set to 0 to disable. \see sendRaw() */
    void setSimulateLossRecv(float probabilityofloss); /**< Simulate packet loss during receiving. Set to 0 to disable. Still receives but will return 0 (no bytes/socket would block). \see recvRaw() */
    float getSimulateLossSend(); /**< Return the current probability of packet loss for sending. */
    float getSimulateLossRecv(); /**< Return the current probability of packet loss for receiving. */


    int closeAndCleanup(); /**< Close sockets, WSA cleanup (on Windows). Will be sorted by destructor if not called by app. */

};

#endif
