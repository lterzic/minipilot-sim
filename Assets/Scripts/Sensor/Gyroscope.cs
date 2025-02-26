using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class Gyroscope : MonoBehaviour
{
    private Rigidbody m_rigidbody;
    private Vector3 m_angularVelocity;
    // Currently accumulated drift
    private Vector3 m_drift;
    // Computed angular velocity that is returned on read request
    private Vector3 m_output;

    // Start is called before the first frame update
    void Start()
    {
        m_rigidbody = GetComponent<Rigidbody>();
    }

    void FixedUpdate()
    {
        m_angularVelocity = transform.InverseTransformDirection(m_rigidbody.angularVelocity);
        // add noise and drift
        m_output = m_angularVelocity;
    }

    // TODO: Rename to Read() or ReadAngularVelocity()
    public Vector3 GetAngularVelocity()
    {
        // TODO: Protect with a mutex
        return m_output;
    }

    public Vector3 GetRealAngularVelocity()
    {
        return m_angularVelocity;
    }
}
