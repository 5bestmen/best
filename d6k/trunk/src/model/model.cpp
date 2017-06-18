
#include "model.h"
#include "../object_model/modelconfigwgt.h"
#include <QMessageBox>


CModelWindow::CModelWindow(QWidget *parent)
    : QMainWindow(parent)
{
    InitModelTool();

    LoadModelTool();
}

CModelWindow::~CModelWindow()
{
    FiniModelTool();
}

// 初始化模型工具
void CModelWindow::InitModelTool()
{
    m_pModelLib = nullptr;
    m_pModelDll = nullptr;
}

// 释放
void CModelWindow::FiniModelTool()
{
    if (nullptr != m_pModelLib)
    {
        delete m_pModelLib;
        m_pModelLib = nullptr;
    }

    if (nullptr != m_pModelDll)
    {
        delete m_pModelDll;
        m_pModelDll = nullptr;
    }
}

void CModelWindow::LoadModelTool()
{
    if (nullptr != m_pModelDll)
    {
        delete m_pModelDll;
        m_pModelDll = nullptr;
    }

    if (nullptr != m_pModelLib)
    {
        delete m_pModelLib;
        m_pModelLib = nullptr;
    }

#ifdef _DEBUG
    m_pModelLib = new QLibrary("object_model");
#else
    m_pModelLib = new QLibrary("object_model");
#endif

    //判断是否正确加载
    if (!m_pModelLib->load())
    {
        QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("Model加载失败!!!"));
        warning.exec();
        return;
    }

    typedef CModelConfigWgt* (*ModelTool)();
    // 获取模型工具
    ModelTool getModelToolDll = (ModelTool)m_pModelLib->resolve("GetModelToolDll");
    if (getModelToolDll)
    {
        m_pModelDll = getModelToolDll();
        m_pModelDll->show();
    }
}