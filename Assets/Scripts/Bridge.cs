using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using UnityEngine;
using Google.Protobuf;

public class Bridge : MonoBehaviour
{
    // Listener port for creating the bridge socket
    public int m_port = 5000;
    private UdpClient m_listener;
    // Accelerometer
    public Accelerometer m_accelerometer;
    // Gyroscope
    public Gyroscope m_gyroscope;
    // Motor actuators which can be controlled by the Bridge
    public PropMotor[] m_motors;
    // @todo Add Servo[] m_servos;

    // Start is called before the first frame update
    void Start()
    {
        m_listener = new UdpClient(m_port);
        Thread listenerThread = new Thread(HandleRequests);
        listenerThread.IsBackground = true; // closes the thread on app quit
        listenerThread.Start();
        Debug.Log("Started UDP listener...");
    }

    private void HandleRequests()
    {
        while (true) {
            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, m_port);
            byte[] buffer = m_listener.Receive(ref ipEndPoint);
            
            // Parse the received request
            Mpsim.Request request = Mpsim.Request.Parser.ParseFrom(buffer);
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

            byte[] out_buffer = response.ToByteArray();
            m_listener.Send(out_buffer, response.CalculateSize(), ipEndPoint);
        }
    }
}
