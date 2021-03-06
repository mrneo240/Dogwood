#pragma once

#include "Math/Algebra.h"

class Collider;
class BoxCollider;
class CapsuleCollider;      // TODO implement me
class SphereCollider;

struct CollisionContact
{
public:
    Collider*           ColliderA;
    Collider*           ColliderB;

    Vector3             ContactPoint;
    Vector3             ContactNormal;
    float               Penetration;
};

struct CollisionData
{
    CollisionData(int maxContacts);
    ~CollisionData();

    CollisionContact*   ClaimNextContact();
    void                Reset();

    CollisionContact*   Contacts;
    CollisionContact*   NextEmptyContact;

    int                 ContactsUsed;
    int                 ContactsRemaining;
    int                 MaxContacts;
};

class CollisionDetection
{
public:
    static unsigned int SphereAndSphere(SphereCollider* a, SphereCollider* b, CollisionData* data);
    static unsigned int SphereAndBox(SphereCollider* s, BoxCollider* b, CollisionData* data);
    static unsigned int BoxAndBox(BoxCollider* a, BoxCollider* b, CollisionData* data);

private:
    static float    ProjectToAxis(BoxCollider* box, Vector3& axis);
    static float    PenetrationOnAxis(BoxCollider* a, BoxCollider* b, Vector3& axis, Vector3& centerAToCenterB);
    static void     SetFaceVertexContactData(BoxCollider* faceBox, BoxCollider* vertexBox, Vector3& centerToCenter, CollisionData* data, Vector3 axis, float bestOverlap);
    static void     SetEdgeEdgeContactData(BoxCollider* a, BoxCollider* b, CollisionData* data, int oneAxisIndex, int twoAxisIndex, Vector3& centerToCenter, float bestOverlap);
    static Vector3  GetContactPoint(Vector3& axisOne, Vector3& axisTwo, Vector3& pointOnEdgeOne, Vector3& pointOnEdgeTwo);
};