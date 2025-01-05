using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using UnityEngine;
using Google.Protobuf;

public class Bridge : MonoBehaviour
{
    // TCP server port for creating the bridge socket
    public int m_port = 5000;
    // Accelerometer
    public Accelerometer m_accelerometer;
    // Gyroscope
    public Gyroscope m_gyroscope;
    // Motor actuators which can be controlled by the Bridge
    public PropMotor[] m_motors;
    // @todo Add Servo[] m_servos;
    private TcpListener m_listener;

    // Start is called before the first frame update
    void Start()
    {
        m_listener = new TcpListener(IPAddress.Loopback, m_port);
        m_listener.Start();
        Thread listenerThread = new Thread(ListenForClients);
        listenerThread.IsBackground = true; // closes the thread on app quit
        listenerThread.Start();
    }

    private void ListenForClients()
    {
        while (true) {
            TcpClient client = null;
            try {
                Debug.Log("Waiting for client connection...");
                client = m_listener.AcceptTcpClient();
                Debug.Log("Connection established...");
                
                HandleRequests(client.GetStream());
                client.Close();
            } catch (SocketException e) {
                Debug.LogError("Client connection broken");
                Debug.LogException(e);
                if (client != null)
                    client.Close();
            }
        }
    }

    private void HandleRequests(NetworkStream stream)
    {
        byte[] buffer = new byte[1024];
        while (true) {
            int dataSize = stream.Read(buffer);
            if (dataSize == 0)
                return;
            
            // Parse the received request
            Mpsim.Request request = Mpsim.Request.Parser.ParseFrom(buffer, 0, dataSize);
            Mpsim.Response response = new Mpsim.Response();
            response.Success = false;

            switch (request.RequestTypeCase) {
                case Mpsim.Request.RequestTypeOneofCase.ReadAcc:
                    Vector3 accel = m_accelerometer.GetAcceleration();
                    response.ReadAcc = new Mpsim.ResponseReadAcc {Acc = new Mpsim.Vector3f { X = accel.x, Y = accel.y, Z = accel.z}};
                    response.Success = true;
                    break;
                case Mpsim.Request.RequestTypeOneofCase.ReadGyro:
                    Vector3 angVel = m_gyroscope.GetAngVelocity();
                    response.ReadGyro = new Mpsim.ResponseReadGyro {AngVel = new Mpsim.Vector3f { X = angVel.x, Y = angVel.y, Z = angVel.z}};
                    response.Success = true;
                    break;
                case Mpsim.Request.RequestTypeOneofCase.WriteMotor:
                    uint motorIndex = request.WriteMotor.MotorIndex;
                    if (motorIndex >= m_motors.Length) {
                        Debug.LogError("Trying to access non-existend motor");
                        break;
                    }
                    m_motors[motorIndex].SetThrottle(request.WriteMotor.Throttle);
                    response.WriteMotor.CurrentThrottle = m_motors[motorIndex].GetThrottle();
                    response.Success = true;
                    break;
                case Mpsim.Request.RequestTypeOneofCase.None:
                default:
                    break;
            }

            response.WriteTo(stream);
        }
    }
}
