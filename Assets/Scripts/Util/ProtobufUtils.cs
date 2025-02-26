using UnityEngine;

public static class ProtobufUtils
{
    public static Mpsim.Pb.Vector3f CreatePbVector(Vector3 v)
    {
        return new Mpsim.Pb.Vector3f {X = v.x, Y = v.y, Z = v.z};
    }

    public static Mpsim.Pb.Vector4f CreatePbVector(Quaternion q)
    {
        return new Mpsim.Pb.Vector4f {W = q.w, X = q.x, Y = q.y, Z = q.z};
    }
}