
#include <QtCore/QCoreApplication>

#include <memory>
#include "shm_mgr.h"
#include "shm_service.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //int size = sizeof(Mgr_Data);

    //CShmMgr* pShmMgr = new CShmMgr("F:/2.0/trunk/project/5/scada/123Mem", 1024);
    //pShmMgr->CreateShm(size);

    CSHMService* pShm = CSHMService::Instance();
    QString str("F:/2.0/trunk/project/scadastudio/");
    pShm->Initialize(str, 0);

    //size_t size = 1024;// *1024 * 1024 * 10; // 10¸öG
    //size_t size = 1024 * 1024 * 1024; // 10¸öG
    //CShmMgr* pShmMgr = new CShmMgr("F:/2.0/trunk/project/scadastudio/scada/123Mem", size);
    //pShmMgr->CreateShm(size);

    return a.exec();
}


/*#include <memory>
#include <iostream>
#include <functional>
using namespace std;

int TestFunc(int a, char c, float f)
{
    cout << a << endl;
    cout << c << endl;
    cout << f << endl;

    return a;
}

class CAinAlarm
{

};

class test
{
public:
    
    std::shared_ptr<CAinAlarm> TestFunc(int a, char c, float f)
    {
        cout << a << endl;
        cout << c << endl;
        cout << f << endl;

        return nullptr;
    }

    std::shared_ptr<CAinAlarm> GetAinAlarm(int nOccNo)
    {
        return nullptr;
    }

    void func()
    {
        m_fnGetAlarm = std::bind(&test::GetAinAlarm, this, std::placeholders::_1);
    }

    void func1()
    {
        m_fnGetAlarm = std::bind(&test::TestFunc, this, std::placeholders::_1, 'A', 100.1);
    }

    std::function< std::shared_ptr<CAinAlarm>(int nAlarmOccNo) > m_fnGetAlarm;

    std::function< std::shared_ptr<CAinAlarm>(int nAlarmOccNo) > m_fnGetAlarm1;
};
#include "scdsvc_define.h"
int main()
{
    auto bindFunc1 = bind(TestFunc, std::placeholders::_1, 'A', 100.1);
    bindFunc1(10);

    cout << "=================================\n";

    auto bindFunc2 = bind(TestFunc, std::placeholders::_2, std::placeholders::_1, 100.1);
    bindFunc2('B', 10);

    cout << "=================================\n";

    auto bindFunc3 = bind(TestFunc, std::placeholders::_2, std::placeholders::_3, std::placeholders::_1);
    bindFunc3(100.1, 30, 'C');

    return 0;
}*/