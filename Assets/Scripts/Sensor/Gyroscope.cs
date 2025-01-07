using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class Gyroscope : MonoBehaviour
{
    private Rigidbody m_rigidbody;
    // Currently accumulated drift
    private Vector3 m_drift;
    // Computed angular velocity that is returned on read request
    private Vector3 m_angularVelocity;

    // Start is called before the first frame update
    void Start()
    {
        m_rigidbody = GetComponent<Rigidbody>();
    }

    void FixedUpdate()
    {
        // add noise and drift
        m_angularVelocity = m_rigidbody.angularVelocity;
    }

    public Vector3 GetAngVelocity()
    {
        // TODO: Protect with a mutex
        return m_angularVelocity;
    }
}
