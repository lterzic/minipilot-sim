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
    // Power consumed proportional to throttle^2
    public float m_electricPowerConstant;
    // Spin direction
    public bool m_spinCW;
    // This script generates a force and torque in the forward direction of the rigidbody
    private Rigidbody m_rigidbody;
    // Current throttle simulated with the motor time constant
    private float m_throttle;
    // Target throttle for simulating non-zero time response
    private float m_throttleTarget;

    // Start is called before the first frame update
    void Start()
    {
        m_rigidbody = GetComponent<Rigidbody>();
        m_throttle = 0;
        m_throttleTarget = 0;
    }

    void FixedUpdate()
    {
        // First order transfer function discrete approximation for throttle response
        float dt = Time.fixedDeltaTime;
        float coeffIn = dt / (dt + m_timeConstant);
        float coeffPrev = m_timeConstant / (dt + m_timeConstant);
        m_throttle = coeffIn * m_throttleTarget + coeffPrev * m_throttle;

        m_rigidbody.AddRelativeForce((m_throttle * m_throttle * m_thrustCoeff) * Vector3.forward);
        m_rigidbody.AddRelativeTorque((m_throttle * m_throttle * m_torqueCoeff) * (m_spinCW ? Vector3.forward : Vector3.back));
    }

    public void SetThrottle(float throttle)
    {
        float clampedThrottle = Mathf.Clamp(throttle, 0, 1);
        m_throttleTarget = clampedThrottle;
    }

    public float GetThrottle()
    {
        return m_throttle;
    }
}
