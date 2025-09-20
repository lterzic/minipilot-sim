using UnityEngine;

public static class ProtobufUtils
{
    public static Mp.Pb.Vector3f CreatePbVector(Vector3 v)
    {
        return new Mp.Pb.Vector3f {X = v.x, Y = v.y, Z = v.z};
    }

    public static Mp.Pb.Vector4f CreatePbVector(Quaternion q)
    {
        return new Mp.Pb.Vector4f {W = q.w, X = q.x, Y = q.y, Z = q.z};
    }
}