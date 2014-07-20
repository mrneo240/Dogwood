#include "maineditorwindow.h"
#include "HierarchyModel.h"
#include "ui_maineditorwindow.h"
#include "Widgets\GLWidget.h"
#include "Widgets\VectorEdit.h"

#include <QtWidgets>

using namespace EditorCommands;

MainEditorWindow::MainEditorWindow(QWidget *parent)
: m_ui(new Ui::MainEditorWindow)
{
    m_glWidget = new GLWidget;
    m_glWidget->setFixedSize(640, 480);
 
    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(m_glWidget);

    setWindowTitle(tr("Dogwood Editor!"));
    m_ui->textEdit_DebugOutput->append("Loading scene...");

    m_view = m_ui->treeView;

    // TODO move into "Transform" widget?
    m_vectorEdit = new VectorEdit();
    m_vectorEdit->SetTitle("Position");
    m_ui->componentLayout->addWidget(m_vectorEdit);

    // Edit menu
    connect(m_ui->actionUndo, SIGNAL(triggered()), this, SLOT(Undo()));
    connect(m_ui->actionRedo, SIGNAL(triggered()), this, SLOT(Redo()));

    // Game Object menu
    connect(m_ui->actionCreate_Game_Object, SIGNAL(triggered()), this, SLOT(CreateGameObject()));
    connect(m_ui->actionDelete_Game_Object, SIGNAL(triggered()), this, SLOT(DeleteGameObject()));
}

MainEditorWindow::~MainEditorWindow()
{
    delete m_ui;
    delete m_glWidget;
}

void MainEditorWindow::Paint()
{
    m_glWidget->update();
}

void MainEditorWindow::SetHierarchyModel(HierarchyModel* model)
{
    m_model = model;
    m_ui->treeView->setModel(m_model);
    DebugLog("Scene loaded!");
}

void MainEditorWindow::DebugLog(string text)
{
    m_ui->textEdit_DebugOutput->append(QString(text.c_str()));
}

void MainEditorWindow::Undo()
{
    // TODO disable menu option if undo is not currently available
    DebugLog("Undo");
    m_commandManager.Undo();
}

void MainEditorWindow::Redo()
{
    // TODO disable menu option if redo is not currently available
    DebugLog("Redo");
    m_commandManager.Redo();
}

void MainEditorWindow::CreateGameObject()
{
    DebugLog("Creating game object");

    QModelIndex index = m_view->selectionModel()->currentIndex();
    HierarchyModel* model = (HierarchyModel*)(m_view->model());
    CreateGameObjectCommand* command = new CreateGameObjectCommand(model, index);
    m_commandManager.ExecuteCommand(command);
}

void MainEditorWindow::DeleteGameObject()
{
    DebugLog("Deleting game object");

    QModelIndex index = m_view->selectionModel()->currentIndex();
    HierarchyModel* model = (HierarchyModel*)(m_view->model());
    DeleteGameObjectCommand* command = new DeleteGameObjectCommand(model, index);
    m_commandManager.ExecuteCommand(command);
}

namespace EditorCommands
{
    CreateGameObjectCommand::CreateGameObjectCommand(HierarchyModel* model, QModelIndex index)
    {
        m_model = model;
        m_index = index;
    }

    void CreateGameObjectCommand::Execute()
    {
        m_model->insertRow(m_index.row() + 1, m_index.parent());
    }

    void CreateGameObjectCommand::Undo()
    {
        m_model->removeRow(m_index.row() + 1, m_index.parent());
    }

    DeleteGameObjectCommand::DeleteGameObjectCommand(HierarchyModel* model, QModelIndex index)
    {
        m_model = model;
        m_index = index;

        // TODO handle null parent (deleting the root)
        m_gameObject = (GameObject*)m_index.internalPointer();
        m_parent = (GameObject*)m_index.parent().internalPointer();
        m_position = m_index.row();
    }

    void DeleteGameObjectCommand::Execute()
    {
        m_model->removeRow(m_index.row(), m_index.parent());
    }

    void DeleteGameObjectCommand::Undo()
    {
        m_model->insertChild(m_index.parent(), m_parent, m_gameObject, m_position);
    }
}