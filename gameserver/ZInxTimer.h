#pragma once
#include <zinx.h>
#include<vector>
#include<list>
using namespace std;
class ZinxTimerHandler {
public:
    int icount = -1;
    virtual void proc() = 0;
    virtual int GetTimerSec() = 0;
};
class ZInxTimerMng :
    public AZinxHandler
{
private:
    static ZInxTimerMng* single;
    int cur_index = 0;
    int size_;
    vector<list<ZinxTimerHandler*>>TimelistCircle;
    ZInxTimerMng(int num=10):size_(num) {
        for (int i = 0; i < num; ++i) {
            list<ZinxTimerHandler*>list;
            TimelistCircle.push_back(list);
        }
    };
public:
    // 通过 AZinxHandler 继承
    IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
    void AddTimeHandler(ZinxTimerHandler* handler);
    void RemoveTimeHandler(ZinxTimerHandler* handler);
    static ZInxTimerMng* GetInstance() {
        return single;
    }
};
class ZInxTimerChannel :
    public Ichannel
{
public:
    int m_timefd=-1;
    // 通过 Ichannel 继承
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
class out_hello :
    public ZinxTimerHandler {
    // 通过 ZinxTimerHanler 继承
    void proc() override;
    int GetTimerSec() override;
};
class out_bye :
    public ZinxTimerHandler {
    // 通过 ZinxTimerHanler 继承
    void proc() override;
    int GetTimerSec() override;
};

