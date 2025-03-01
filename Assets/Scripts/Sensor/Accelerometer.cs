using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class Accelerometer : MonoBehaviour
{
    // Noise density in radians per second
    public float m_NoiseDensity;

    private Rigidbody m_rigidbody;
    private Vector3 m_lastVelocity;
    private Vector3 m_acceleration;
    private Vector3 m_output;
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
        m_lastVelocity = velocity;

        // Add bias
        Vector3 noise = Noise.nextNormalVector(0, m_NoiseDensity * Mathf.Sqrt(Time.fixedDeltaTime));
        m_output = transform.InverseTransformDirection(m_acceleration + noise - Physics.gravity);
    }

    // TODO: Rename to Read() or ReadAcceleration()
    public Vector3 GetAcceleration()
    {
        return m_output;
    }

    public Vector3 GetRealAcceleration()
    {
        return m_acceleration;
    }
}
