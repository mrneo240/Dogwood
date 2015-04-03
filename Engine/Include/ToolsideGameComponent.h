#pragma once

#include <string>
#include <tinyxml2.h>
#include <unordered_map>

#include "Math\Algebra.h"
#include "Rendering\Colour.h"

using std::string;
using std::unordered_map;

struct ComponentParameter
{
public:
    enum ParameterType
    {
        TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_VECTOR3, TYPE_COLOR
    };

    ComponentParameter() {}
    ComponentParameter(string name, ParameterType type)
    {
        Name = name; Type = type;
    }

    bool operator==(const ComponentParameter &other) const;

    string          Name;
    ParameterType   Type;
};

// TODO this should really be a Union not a Struct, but I'm having trouble getting the Union to work
struct ComponentValue
{
public:
    ComponentValue();

    int         i;
    float       f;
    bool        b;
    string      str;
    Vector3     v;
    ColourRGB   c;
};

namespace std
{
    template <> struct hash<ComponentParameter>
    {
        size_t operator()(const ComponentParameter & c) const
        {
            using std::size_t;
            using std::hash;

            // TODO this hash function is terrible, fix it
            return ((hash<string>()(c.Name)
                ^ (hash<int>()((int)c.Type) << 1)) >> 1);
        }
    };
}

typedef unordered_map < ComponentParameter, ComponentValue > ParamMap;

class ToolsideGameComponent
{
public:
    void            Create(unsigned int guid);
    void            Load(tinyxml2::XMLElement* componentXML);
    void            Serialize();

    ParamMap*       GetParameterList();
    void            SetParameter(ComponentParameter param, ComponentValue value);

private:
    void            AddParameterToList(tinyxml2::XMLElement* paramXML);

    unsigned int    m_guid;
    ParamMap        m_map;
};

class ToolsideComponentSchema
{
public:
    bool            Load(string filename);
    ParamMap*       GetDefaultParameterList(unsigned int guid);

private:
    unordered_map<unsigned int, ParamMap*> m_schema;
};