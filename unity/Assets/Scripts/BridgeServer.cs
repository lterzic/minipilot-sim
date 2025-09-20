using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using UnityEngine;
using Google.Protobuf;

public class BridgeServer : MonoBehaviour
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

    // Used for getting the real state of the vehicle
    public Rigidbody m_vehicleRigidbody;
    // This is updated in FixedUpdate and sent on request
    private Mp.Pb.State m_state;

    // Start is called before the first frame update
    void Start()
    {
        m_listener = new UdpClient(m_port);
        Thread listenerThread = new Thread(HandleRequests);
        listenerThread.IsBackground = true; // closes the thread on app quit
        listenerThread.Start();
        Debug.Log("Started UDP listener...");
    }

    void FixedUpdate()
    {
        m_state = new Mp.Pb.State {
            Position = ProtobufUtils.CreatePbVector(m_vehicleRigidbody.position),
            Velocity = ProtobufUtils.CreatePbVector(m_vehicleRigidbody.velocity),
            Acceleration = ProtobufUtils.CreatePbVector(m_accelerometer.GetRealAcceleration()),
            AngularVelocity = ProtobufUtils.CreatePbVector(m_gyroscope.GetRealAngularVelocity()),
            Rotation = ProtobufUtils.CreatePbVector(m_vehicleRigidbody.rotation)
        };
    }

    private void HandleRequests()
    {
        while (true) {
            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, m_port);
            byte[] buffer = m_listener.Receive(ref ipEndPoint);
            
            // Parse the received request
            Mp.Pb.Sim.Request request = Mp.Pb.Sim.Request.Parser.ParseFrom(buffer);
            Mp.Pb.Sim.Response response = new Mp.Pb.Sim.Response();
            response.Success = false;

            switch (request.RequestTypeCase) {
                case Mp.Pb.Sim.Request.RequestTypeOneofCase.ReadState:
                    response.ReadState = new Mp.Pb.Sim.ResponseReadState {State = m_state};
                    response.Success = true;
                    break;
                case Mp.Pb.Sim.Request.RequestTypeOneofCase.ReadAcc:
                    response.ReadAcc = new Mp.Pb.Sim.ResponseReadAcc {Acc = ProtobufUtils.CreatePbVector(m_accelerometer.GetAcceleration())};
                    response.Success = true;
                    break;
                case Mp.Pb.Sim.Request.RequestTypeOneofCase.ReadGyro:
                    response.ReadGyro = new Mp.Pb.Sim.ResponseReadGyro {AngVel = ProtobufUtils.CreatePbVector(m_gyroscope.GetAngularVelocity())};
                    response.Success = true;
                    break;
                case Mp.Pb.Sim.Request.RequestTypeOneofCase.WriteMotor:
                    uint motorIndex = request.WriteMotor.MotorIndex;
                    if (motorIndex >= m_motors.Length) {
                        Debug.LogError("Trying to access non-existend motor");
                        break;
                    }
                    m_motors[motorIndex].SetThrottle(request.WriteMotor.Throttle);
                    response.WriteMotor = new Mp.Pb.Sim.ResponseWriteMotor {CurrentThrottle = m_motors[motorIndex].GetThrottle()};
                    response.Success = true;
                    break;
                case Mp.Pb.Sim.Request.RequestTypeOneofCase.None:
                default:
                    break;
            }

            byte[] out_buffer = response.ToByteArray();
            if (out_buffer.Length > 0)
                m_listener.Send(out_buffer, out_buffer.Length, ipEndPoint);
        }
    }
}
