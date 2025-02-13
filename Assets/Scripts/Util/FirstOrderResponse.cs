using System;

class FirstOrderResponse {

    public FirstOrderResponse(float timeConstant, float initialOutput)
    {
        m_timeConstant = timeConstant;
        m_output = initialOutput;
    }

    public float Update(float input, float dt)
    {
        float coeffInput = dt / (dt + m_timeConstant);
        float coeffOutput = m_timeConstant / (dt + m_timeConstant);

        m_output = coeffInput * input + coeffOutput * m_output;
        return m_output;
    }

    public float GetOutput()
    {
        return m_output;
    }

    private float m_output;
    private float m_timeConstant;
}