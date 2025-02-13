using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class PropMotor : MonoBehaviour
{
    // Amount of thrust produced at max RPM (throttle = 1)
    public float m_thrustCoeff;
    // Amount of torque produced at max RPM (throttle = 1)
    public float m_torqueCoeff;
    // Constant k assuming a transfer function G(s) = 1/(1+ks)
    public float m_timeConstant;
    // Spin direction
    public bool m_spinCW;
    // This script generates a force and torque in the forward direction of the rigidbody
    private Rigidbody m_rigidbody;
    // Current throttle simulated with the motor time constant
    private FirstOrderResponse m_throttleResponse;
    // Target throttle for simulating non-zero time response
    [Range(0, 1)]
    public float m_throttleTarget;

    // Start is called before the first frame update
    void Start()
    {
        m_rigidbody = GetComponent<Rigidbody>();
        m_throttleResponse = new FirstOrderResponse(m_timeConstant, 0);
        m_throttleTarget = 0;
    }

    void FixedUpdate()
    {
        float throttle = m_throttleResponse.Update(m_throttleTarget, Time.fixedDeltaTime);

        m_rigidbody.AddRelativeForce((throttle * throttle * m_thrustCoeff) * Vector3.forward);
        m_rigidbody.AddRelativeTorque((throttle * throttle * m_torqueCoeff) * (m_spinCW ? Vector3.forward : Vector3.back));
    }

    public void SetThrottle(float throttle)
    {
        m_throttleTarget = Mathf.Clamp(throttle, 0, 1);
    }

    public float GetThrottle()
    {
        return m_throttleResponse.GetOutput();
    }
}
