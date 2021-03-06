// This is an example for a heaer file
// It defines the square of a number function

#include "OptitrackServerConfig.h"

#ifndef __OptitrackServer_h
#define __OptitrackServer_h

#include "igtlServerSocket.h"
#include "igtlSocket.h"
#include "igtlMessageHeader.h"

#include "OptitrackTracker.h"
#include "OptitrackTool.h"

// ITK Libs
#include <itkMultiThreader.h>
#include <itkFastMutexLock.h>
#include <itkObject.h>


/**
 * \class Optitrack Server
 * \brief This class performs broadcasting of the Optitrack tracking data using the OpenIGTLink
 * communication protocol.
 *
 */

namespace Optitrack{

    class BiiGOptitrackControl_EXPORT OptitrackServer
    {
    public:

        friend class OptitrackTracker;

        OptitrackServer();

        ~OptitrackServer();

        void Connections();

        std::string ReceiveInstructions(igtl::Socket::Pointer instSocket);
        int SendPosition(igtl::Socket * socket);
        int SendStatus(igtl::Socket * socket, int status);

    protected:

        void SendToolTransform(int i);

#if OpenIGTLink_PROTOCOL_VERSION >= 2
        int SendString(igtl::Socket * socket, std::string Str);
        std::string ReceiveString(igtl::Socket * socket, igtl::MessageHeader * header);
#endif //OpenIGTLink_PROTOCOL_VERSION >= 2

    private:

        /**
        * \brief Start the Tracking Thread to send the transform for every tool
        */

        static ITK_THREAD_RETURN_TYPE ThreadSendNavigationInfo(void* pInfoStruct);

        void GetRandomTestVectors(float* position, float* quaternion);
        void GetRandomTestMatrix(igtl::Matrix4x4& matrix);
        void GetOptitrackToolTransformMatrix(igtl::Matrix4x4& matrix, int i);


        Optitrack::OptitrackTracker::Pointer m_Tracker;
        Optitrack::OptitrackTool::Pointer m_Tool;

        igtl::ServerSocket::Pointer m_ServerSocket;
        igtl::Socket::Pointer m_Socket;
        //std::vector<igtl::Socket::Pointer> m_ListOfSockets;

        unsigned int m_PortNumber = 18944;
        int m_Interval = 10;

        /**
        * \brief MultiThreader that starts continuous tracking update
        */
        itk::FastMutexLock::Pointer m_RequestMessageMutex;
        std::string m_RequestMessage;

        /**
        * \brief MultiThreader that starts continuous tracking update
        */
        itk::MultiThreader::Pointer m_MultiThreader;
        /**
        * \brief ThreadID number identification
        */
        int m_ThreadID;
    };

}
#endif //__OptitrackServer_h
