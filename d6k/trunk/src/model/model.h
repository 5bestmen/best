#ifndef MODEL_H
#define MODEL_H

#include <QtWidgets/QMainWindow>
#include <QLibrary>

//#include "../equipmentmodel/equipmentmodel.h"
#include "../object_model/modelconfigwgt.h"
//class CObjectModelWindow;
class CModelWindow : public QMainWindow
{
    Q_OBJECT

public:
    CModelWindow(QWidget *parent = 0);
    ~CModelWindow();

private:
    // 初始化模型工具
    void InitModelTool();

    // 释放
    void FiniModelTool();

    // 加载模型工具
    void LoadModelTool();

private:
    QLibrary* m_pModelLib;

    CModelConfigWgt* m_pModelDll;
};

#endif // MODEL_H
