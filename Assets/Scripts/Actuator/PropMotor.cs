using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class PropMotor : MonoBehaviour
{
    private Rigidbody m_rigidbody;
    private float m_throttle;
    // Amount of thrust produced at max throttle (throttle = 1)
    public float m_thrustCoeff;
    // Amount of torque produced at max throttle (throttle = 1)
    public float m_torqueCoeff;
    public bool m_spinCW;

    // Start is called before the first frame update
    void Start()
    {
        m_rigidbody = GetComponent<Rigidbody>();
        m_throttle = 0;
    }

    void FixedUpdate()
    {
        // Add desired throttle and then add 1st order filter to lerp throttle to desired throttle
        m_rigidbody.AddRelativeForce((m_throttle * m_throttle * m_thrustCoeff) * Vector3.forward);
        m_rigidbody.AddRelativeTorque((m_throttle * m_throttle * m_torqueCoeff) * (m_spinCW ? Vector3.forward : Vector3.back));
    }

    public void SetThrottle(float throttle)
    {
        float clampedThrottle = Mathf.Clamp(throttle, 0, 1);
        m_throttle = clampedThrottle; // set m_desiredThrottle instead of m_throttle
    }

    public float GetThrottle()
    {
        return m_throttle;
    }
}
