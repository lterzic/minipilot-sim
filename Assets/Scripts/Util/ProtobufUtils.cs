using UnityEngine;

public static class ProtobufUtils
{
    public static Pb.Mp.Vector3f CreatePbVector(Vector3 v)
    {
        return new Pb.Mp.Vector3f {X = v.x, Y = v.y, Z = v.z};
    }

    public static Pb.Mp.Vector4f CreatePbVector(Quaternion q)
    {
        return new Pb.Mp.Vector4f {W = q.w, X = q.x, Y = q.y, Z = q.z};
    }
}