#pragma once

#include <float.h>
#include <string>
#include <vector>

#include "Math\Algebra.h"
#include "Math\Transform.h"

using std::string;
using std::vector;

class GameComponent;
class MeshInstance;

class GameObject
{
public:
    GameObject(string name = "", GameObject* parent = NULL);

    Transform&      GetLocalTransform();
    Transform       GetWorldTransform();
    void            SetLocalTransform(Transform& t);
    void            SetLocalTransform(Matrix4x4& m);

    int             GetID();
    string          GetName();
    void            SetName(string name);
    void            SetName(const char* name);

    GameObject*     GetParent();
    void            SetParent(GameObject* parent, int index = -1);
    void            AddComponent(GameComponent* component);

    GameObject*     GetChild(int index);
    vector<GameObject*>& GetChildren();
    int             GetChildNumber();
    int             GetChildCount();

    bool            InsertChildren(int position, int count);
    bool            InsertChild(int position, GameObject* child);  // TODO this needs some cleanup
    bool            RemoveChildren(int position, int count);

    GameObject*     DeepCopy(GameObject* parent = NULL);

    void            Start();
    void            Update(float deltaTime);
    void            OnEnable();
    void            OnDisable();

    void            Render(Transform& parentWorldTransform, bool dirty, bool wireframe = false);

    void            SetMesh(MeshInstance* mesh);
    MeshInstance*   GetMesh();

    void            SetSelected(bool selected);
    GameObject*     BoundingSphereRaycast(Vector3 rayOrigin, Vector3 rayDirection, Transform& parentWorldTransform, float& distance);

    static vector<GameObject*> ActiveGameObjects;

private:

    void            AddChild(GameObject* child, int index = -1);
    void            RemoveChild(GameObject* child);

    static int      NextID;

    int                     m_id;
    string                  m_name;

    Transform               m_localTransform;
    Transform               m_worldTransform;
    bool                    m_dirty;
    bool                    m_selected;

    MeshInstance*           m_mesh;         // TODO this should be part of regular component list

    GameObject*             m_parent;
    vector<GameObject*>     m_children;

    vector<GameComponent*>  m_components;
};