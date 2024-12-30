using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class Accelerometer : MonoBehaviour
{
    private Rigidbody m_rigidbody;
    private Vector3 m_lastVelocity;
    private Vector3 m_acceleration;
    private Vector3 m_bias;

    // Start is called before the first frame update
    void Start()
    {
        m_rigidbody = GetComponent<Rigidbody>();
        m_lastVelocity = Vector3.zero;
        m_acceleration = Vector3.zero;
        // Init m_bias based on datasheet range
    }

    void FixedUpdate()
    {
        float dt = Time.fixedDeltaTime;

        Vector3 velocity = m_rigidbody.velocity;
        m_acceleration = (velocity - m_lastVelocity) / dt;
    }

    public Vector3 GetAcceleration()
    {
        // Add bias and noise to this
        return m_acceleration;
    }
}
