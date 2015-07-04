#pragma once

#include <qtreeview.h>

class MainEditorWindow;

class HierarchyView : public QTreeView
{
    Q_OBJECT

public:
    HierarchyView(MainEditorWindow* window);

private:
    MainEditorWindow*   m_window;
};