using System;
using UnityEngine;

public static class Noise
{
    private static System.Random m_random = new System.Random();
    
    public static double nextNormalDouble(double mean, double stdDev)
    {
        double u1 = 1.0 - m_random.NextDouble();
        double u2 = 1.0 - m_random.NextDouble();
        double randStdNormal = Math.Sqrt(-2.0 * Math.Log(u1)) *
            Math.Sin(2.0 * Math.PI * u2); //random normal(0,1)
        return mean + stdDev * randStdNormal; //random normal(mean,stdDev^2)
    }

    public static float nextNormalFloat(float mean, float stdDev)
    {
        float u1 = UnityEngine.Random.Range(1e-9f, 1);
        float u2 = UnityEngine.Random.Range(1e-9f, 1);
        float randStdNormal = Mathf.Sqrt(-2.0f * Mathf.Log(u1)) *
            Mathf.Sin(2.0f * Mathf.PI * u2);
        return mean + stdDev * randStdNormal;
    }

    public static Vector3 nextNormalVector(float mean, float stdDev)
    {
        return new Vector3(
            nextNormalFloat(mean, stdDev),
            nextNormalFloat(mean, stdDev),
            nextNormalFloat(mean, stdDev)
        );
    }


}