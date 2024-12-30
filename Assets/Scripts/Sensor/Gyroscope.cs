using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class Gyroscope : MonoBehaviour
{
    private Rigidbody m_rigidbody;
    private Vector3 m_drift;

    // Start is called before the first frame update
    void Start()
    {
        m_rigidbody = GetComponent<Rigidbody>();
    }

    public Vector3 GetAngVelocity()
    {
        // add noise and drift
        return m_rigidbody.angularVelocity;
    }
}
