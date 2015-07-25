#include "ComponentModelItem.h"
#include "EditorUtil.h"
#include "GameObject.h"
#include "GameObjectMimeData.h"
#include "GameObjectReference.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/MeshInstance.h"
#include "Rendering/Texture.h"
#include "Scene/ResourceManager.h"
#include "Util.h"
#include <QBrush>
#include <QColor>
#include <QColorDialog>

ComponentModelItem::ComponentModelItem(string name)
    : m_name(name), m_isHeader(true), m_parent(NULL)
{}

ComponentModelItem::~ComponentModelItem()
{
    while(m_children.size() > 0)
    {
        ComponentModelItem* child = m_children.back();
        m_children.pop_back();
        delete child;
    }
    m_children.clear();
}

void ComponentModelItem::AddChild(ComponentModelItem* child)
{
    if (child == NULL)
        return;

    m_children.push_back(child);
    child->m_parent = this;
}

ComponentModelItem* ComponentModelItem::GetChild(int index)
{
    if (index < 0 || index >= m_children.size())
        return NULL;

    return m_children[index];
}

ComponentModelItem* ComponentModelItem::GetParent()
{
    return m_parent;
}

int ComponentModelItem::GetChildCount()
{
    return m_children.size();
}

int ComponentModelItem::GetIndexInParent()
{
    // determine the index of this game object in the parent's list of children
    if (m_parent)
    {
        vector<ComponentModelItem*>::iterator iter = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), const_cast<ComponentModelItem*>(this));
        return std::distance(m_parent->m_children.begin(), iter);
    }

    return 0;
}

void ComponentModelItem::Refresh()
{
    vector<ComponentModelItem*>::iterator iter = m_children.begin();
    for (; iter != m_children.end(); iter++)
    {
        (*iter)->Refresh();
    }
}

QVariant ComponentModelItem::GetData(ColumnType columnType, int role)
{
    if (role == Qt::BackgroundRole)
    {
        return GetBackgroundData(columnType);
    }
    else
    {
        switch (columnType)
        {
        case NAME_COLUMN:
            return QVariant(m_name.c_str());
        case VALUE_COLUMN:
            return GetValueData();
        }
    }
}

QVariant ComponentModelItem::GetValueData()
{
    return QVariant("");
}

QVariant ComponentModelItem::GetBackgroundData(ColumnType /*columnType*/)
{
    if (m_isHeader)
    {
        QColor color(230, 230, 230);
        return QBrush(color);
    }
}

bool ComponentModelItem::SetData(QVariant value)
{
    return false;
}

bool ComponentModelItem::IsEditable()
{
    return !m_isHeader;
}

bool ComponentModelItem::DropData(const QMimeData* data)
{
    return false;
}

void ComponentModelItem::OnDoubleClick()
{}

//--------------------------------------------------------------------------------

ComponentModelTransformItem::ComponentModelTransformItem(string name, GameObject* gameObject, TransformVectorType type)
    : ComponentModelItem(name), m_gameObject(gameObject), m_vectorType(type)
{
    Refresh();
}

void ComponentModelTransformItem::Refresh()
{
    ComponentModelItem::Refresh();

    m_isHeader = false;
    m_vector = m_gameObject->GetLocalTransform().GetVector(m_vectorType);
}

QVariant ComponentModelTransformItem::GetValueData()
{
    string str = WriteVector3ToString(m_vector);
    return QVariant(str.c_str());
}

bool ComponentModelTransformItem::SetData(QVariant value)
{
    m_vector = ReadVector3FromString(value.toString().toStdString());

    EditorCommands::ModifyTransformCommand* command = new EditorCommands::ModifyTransformCommand(m_gameObject, m_vector, m_vectorType);
    CommandManager::Singleton().ExecuteCommand(command);

    return true;
}

//--------------------------------------------------------------------------------

ComponentModelMeshItem::ComponentModelMeshItem(string name, MeshInstance* mesh)
    : ComponentModelItem(name), m_mesh(mesh)
{
    m_isHeader = false;

    ComponentModelShaderItem* shaderItem = new ComponentModelShaderItem("Shader", m_mesh->GetMaterial());
    AddChild(shaderItem);
}

QVariant ComponentModelMeshItem::GetValueData()
{
    string str;
    if (m_mesh == NULL)
    {
        str = "<MISSING REF>";
    }
    else
    {
        ResourceInfo* info = m_mesh->GetMesh()->GetResourceInfo();
        str = GetFriendlyAssetNameFromPath(info->path) + " (" + std::to_string(info->guid) + ")";
    }
    return QVariant(str.c_str());
}

bool ComponentModelMeshItem::IsEditable()
{
    return false;
}

bool ComponentModelMeshItem::DropData(const QMimeData* data)
{
    // TODO implement me
    return false;
}

//--------------------------------------------------------------------------------

ComponentModelShaderItem::ComponentModelShaderItem(string name, Material* material)
    : ComponentModelItem(name), m_material(material)
{
    m_isHeader = false;

    if (m_material->GetTexture() != NULL && m_material->GetTexture() != Texture::DefaultTexture())       // TODO temp hack until shader params are fixed properly
    {
        ComponentModelTextureItem* textureItem = new ComponentModelTextureItem("Texture", m_material);
        AddChild(textureItem);
    }

    // TODO add colors
}

QVariant ComponentModelShaderItem::GetValueData()
{
    string str;
    if (m_material == NULL)
    {
        str = "<MISSING REF>";
    }
    else
    {
        ResourceInfo* info = m_material->GetShader()->GetResourceInfo();
        str = GetFriendlyAssetNameFromPath(info->path) + " (" + std::to_string(info->guid) + ")";
    }
    return QVariant(str.c_str());
}

bool ComponentModelShaderItem::IsEditable()
{
    return false;
}

bool ComponentModelShaderItem::DropData(const QMimeData* data)
{
    // TODO implement me
    return false;
}

//--------------------------------------------------------------------------------

ComponentModelTextureItem::ComponentModelTextureItem(string name, Material* material)
    : ComponentModelItem(name), m_material(material)
{
    m_isHeader = false;
}

QVariant ComponentModelTextureItem::GetValueData()
{
    string str;
    if (m_material == NULL)
    {
        str = "<MISSING REF>";
    }
    else
    {
        ResourceInfo* info = m_material->GetTexture()->GetResourceInfo();
        str = GetFriendlyAssetNameFromPath(info->path) + " (" + std::to_string(info->guid) + ")";
    }
    return QVariant(str.c_str());
}

bool ComponentModelTextureItem::IsEditable()
{
    return false;
}

bool ComponentModelTextureItem::DropData(const QMimeData* data)
{
    // TODO implement me
    return false;
}

//--------------------------------------------------------------------------------

ComponentModelScriptItem::ComponentModelScriptItem(ToolsideGameComponent* component, int paramIndex)
    : ComponentModelItem(""), m_component(component), m_paramIndex(paramIndex)
{
    Refresh();
}

void ComponentModelScriptItem::Refresh()
{
    ComponentModelItem::Refresh();

    ParamList params = m_component->GetParameterList();
    ParamPair& pair = params[m_paramIndex];

    m_name = pair.first.Name;
    m_valueType = pair.first.Type;
    m_value = pair.second;

    m_isHeader = false;
}

QVariant ComponentModelScriptItem::GetValueData()
{
    string str;

    switch(m_valueType)
    {
    case ComponentParameter::TYPE_GAMEOBJECT:
    {
        // For game object values, we show the object's name in addition to the guid
        GameObject* go = GameObjectReference::GetGameObject(m_value.g);
        if (go == NULL)
            str = "<MISSING REF>";
        else
            str = go->GetName() + " (" + std::to_string(m_value.g) + ")";
        break;
    }
    case ComponentParameter::TYPE_COLOR:
        str = "";
        break;
    default:
        str = m_value.GetValueString(m_valueType);
        break;
    }

    return QVariant(str.c_str());
}

QVariant ComponentModelScriptItem::GetBackgroundData(ColumnType columnType)
{
    if (m_valueType == ComponentParameter::TYPE_COLOR)
    {
        if (m_isHeader)
        {
            return ComponentModelItem::GetBackgroundData(columnType);
        }

        switch (columnType)
        {
        case ComponentModelItem::NAME_COLUMN:
            return QVariant();
            break;
        case ComponentModelItem::VALUE_COLUMN:
            ColourRGB c = m_value.c;
            QColor qcolor(c.r*255, c.g*255, c.b*255);
            return QBrush(qcolor);
            break;
        }
    }
    else
    {
        return ComponentModelItem::GetBackgroundData(columnType);
    }
}

bool ComponentModelScriptItem::SetData(QVariant value)
{
    // TODO set data using Editor Commands
    m_value.SetValue(m_valueType, value.toString().toStdString());
    m_component->SetParameter(ComponentParameter(m_name, m_valueType), m_value);

    return false;
}

bool ComponentModelScriptItem::IsEditable()
{
    if (m_isHeader)
        return false;

    if (m_valueType == ComponentParameter::TYPE_GAMEOBJECT ||   // reference types can't be edited directly
        m_valueType == ComponentParameter::TYPE_COLOR)          // colors are edited using the color window
        return false;

    return true;
}

bool ComponentModelScriptItem::DropData(const QMimeData* data)
{
    if (m_isHeader)
        return false;

    if (m_valueType != ComponentParameter::TYPE_GAMEOBJECT)
        return false;       // only reference types support drag & drop

    // TODO set data using Editor Commands
    GameObjectMimeData* goData = (GameObjectMimeData*)(data);
    m_value.SetValue(m_valueType, std::to_string(goData->getGameObject()->GetID()));
    m_component->SetParameter(ComponentParameter(m_name, m_valueType), m_value);

    return true;
}

void ComponentModelScriptItem::OnDoubleClick()
{
    switch (m_valueType)
    {
    case ComponentParameter::TYPE_COLOR:
    {
        // Open a ColorDialog to let the user pick a color
        QColor newColor = QColorDialog::getColor(ColourRGBToQColor(m_value.c));

        if (newColor.isValid())                             // color will be invalid if user hits "cancel" on dialog
        {
            // TODO set data using Editor Commands
            m_value.c = QColorToColourRGB(newColor);
            m_component->SetParameter(ComponentParameter(m_name, m_valueType), m_value);
        }

        break;
    }
    default:
        break;
    }
}