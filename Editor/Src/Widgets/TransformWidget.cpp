#include "Widgets\TransformWidget.h"
#include "Widgets\VectorEdit.h"

#include "GameObject.h"

#include "..\GeneratedFiles\ui_transformwidget.h"

TransformWidget::TransformWidget(QWidget* parent)
: m_ui(new Ui::TransformWidget)
{
    m_ui->setupUi(this);

    // Create vector edit widgets
    m_positionWidget = new VectorEdit();
    m_positionWidget->SetTitle("Position");
    m_ui->verticalLayout->addWidget(m_positionWidget);

    m_rotationWidget = new VectorEdit();
    m_rotationWidget->SetTitle("Rotation");
    m_ui->verticalLayout->addWidget(m_rotationWidget);

    m_scaleWidget = new VectorEdit();
    m_scaleWidget->SetTitle("Scale");
    m_ui->verticalLayout->addWidget(m_scaleWidget);

    // Connect to vector edit signals
    connect(m_positionWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdatePosition(Vector3&)));
    connect(m_rotationWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdateRotation(Vector3&)));
    connect(m_scaleWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdateScale(Vector3&)));
}

void TransformWidget::SetGameObject(GameObject* gameObject)
{
    m_gameObject = gameObject;
    if (m_gameObject != NULL)
    {
        // Display the position, rotation, and scale values of this game object
        m_positionWidget->SetVector(m_gameObject->GetLocalTransform().GetPosition());
        m_rotationWidget->SetVector(m_gameObject->GetLocalTransform().GetRotation());
        m_scaleWidget->SetVector(m_gameObject->GetLocalTransform().GetScale());
    }
    else
    {
        // TODO clear the vector fields
    }
}

void TransformWidget::UpdatePosition(Vector3& position)
{
    if (m_gameObject != NULL)
    {
        m_gameObject->GetLocalTransform().SetPosition(position);
    }
}

void TransformWidget::UpdateRotation(Vector3& rotation)
{
    if (m_gameObject != NULL)
    {
        m_gameObject->GetLocalTransform().SetRotation(rotation);
    }
}

void TransformWidget::UpdateScale(Vector3& scale)
{
    if (m_gameObject != NULL)
    {
        m_gameObject->GetLocalTransform().SetScale(scale);
    }
}