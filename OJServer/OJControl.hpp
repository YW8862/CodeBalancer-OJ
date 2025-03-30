
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <mutex>
#include <cassert>
#include "./common/httplib.h"
#include "jsoncpp/json/json.h"
#include "OJView.hpp"
#include "OJModel.hpp"
#include "../common/httplib.h"
#include "../common/utils.hpp"
#include "../common/log.hpp"

namespace ns_control
{

    using namespace httplib;
    using namespace ns_utils;
    using namespace ns_model;
    using namespace ns_view;

    const std::string serviceMachine = "./conf/serviceMachine.conf";

    // 提供服务的主机
    class Machine
    {
    public:
        std::string ip;  // 主机ip
        int port;        // 主机端口
        uint64_t load;   // 主机负载
        std::mutex *mtx; // mutex是禁止拷贝的

    public:
        Machine() : ip(""), port(0), load(0), mtx(nullptr)
        {
        }

        Machine(const Machine &other) : ip(other.ip), port(other.port), load(other.load), mtx(other.mtx)
        {
        }

        ~Machine()
        {
        }

        // 增加负载
        void incLoad()
        {
            if (mtx != nullptr)
            {
                mtx->lock();
                load++;
                mtx->unlock();
            }
        }

        // 减小负载
        void decLod()
        {
            if (mex != nullptr)
            {
                mtx->lock();
                load--;
                mtx->unlock();
            }
        }

        // 获取主机负载
        uint64_t getLoad()
        {
            uint64_t _load = 0;
            mtx->lock();
            _load = load;
            mtx->unlock();
            return _load;
        }
    };

    // 负载均衡模块
    class LoadBalance
    {
    public:
        LoadBalance()
        {
            assert(loadConf(serviceMachine));
            LOG(INFO, "加载主机信息成功");
        }
        ~LoadBalance() {}

    public:
        /**
         *
         */
        bool loadConf(const std::string &machineconf)
        {
            std::ifstream in(machineconf);
            if (!in.is_open())
            {
                LOG(FATAL, "加载：%s,失败", machineconf.c_str());
                return false;
            }
            std::string line;
            while (std::getline(in, line))
            {
                std::vector<std::string> tokens;
                StringUtils::splitString(line, &tokens, ":");
                if (tokens.size() != 2)
                {
                    LOG(ERROR, "切分%s失败", line.c_str());
                    continue;
                }
                Machine machine;
                machine.ip = tokens[0];
                machine.port = atoi(tokens[1].c_str());
                machine.load = 0;
                machine.mtx = new std::mutex();
                online.push_back(machines.size());
                machines.push_back(machine);
            }

            in.close();
            return true;
        }

        // 根据负载均衡，选择主机
        bool intellientSelect(int *id, Machine **m)
        {
            // 1.使用选择好的主机并且更新主机负载
            mtx->lock();

            // 负载均衡算法
            // 轮询+哈希

            int onlineSize = online.size();
            if (onlineSize == 0)
            {
                LOG(Fatal, "所有后端编译主机已全部离线");
                mtx->unlock();
                return false;
            }

            // 通过遍历的方式找到负载最小的机器
            uint64_t minLoad = machines[online[0]].getLoad();
            *id = online[0];
            *m = &machines[online[0]];
            for (int i = 0; i < onlineSize; i++)
            {
                uint64 curLoad = machines[online[i]].getLoad();
                if (minLoad > curLoad)
                {
                    *id = online[i];
                    minLoad = curLoad;
                    *machine = &machines[online[i]]
                }
            }
            // 2.需要离线或上线该主机

            // 3.保证选择主机数据安全

            mtx->unlock();
            return true;
        }

        // 主机下线
        void offlineMachine(int id)
        {
            mtx->lobk();
            for (auto iter = online.online.begin(); iter != online.end(); iter++)
            {
                if (iter == id)
                {
                    // 需要下线的主机已经找到
                    online.erase(iter);
                    offline.push_back(*iter);
                    break;
                }
            }
            mtx->unlock();
        }

        // 主机上线
        void onlineMachine()
        {
            //当所有的主机离线，统一上线
            
        }

        // 测试函数
        void printOnline()
        {
            mtx->lock();
            std::cout << "在线主机：";
            for (int i = 0; i < online.size(); i++)
            {
                std::cout<<online[i]<<" ";
            }
            std::cout<<std::endl;
            mtx->unlock();
        }

        void printOffline()
        {
            mtx->lock();
            std::cout << "离线主机：";
            for (int i = 0; i < online.size(); i++)
            {
                std::cout<<offline[i]<<" ";
            }
            std::cout<<std::endl;
            mtx->unlock();
        }

    private:
        std::vector<Machine> machines; // 管理可以提供编译服务的主机，使用主机来标识机器下标
        std::vector<int> online;       // 代表在线主机
        std::vector<int> offline;      // 表示离线主机
        std::mutex *mtx;
    };

    // 核心逻辑控制器
    class Control
    {
    private:
        Model _model;
        View view;
        LoadBalance loadBalance;

    public:
        /**
         * @brief :根据题目数据构建html网页
         * @param html:输出型参数，构建好的html文档源码
         */
        bool allQuestions(std::string *html)
        {
            std::vector<Question> allQuestion;
            if (_model.getAllQuestions(&allQuestion))
            {
                // 获取题目信息成功，将所有的题目数据构建成网页返回
                view.allExpandHtml(allQuestion, html);
            }
            else
            {
                *html = "获取题目列表失败";
                return false;
            }

            return true;
        }

        /**
         * @brief :根据题目数据构建html网页
         * @param number:输入的他题目编号
         * @param html:输出型参数，构建好的html文档源码
         */
        bool oneQuestion(const std::string &number, std::string *html)
        {
            Question question;
            if (_model.getOneQuestion(number, &question))
            {
                // 获取指定编号题目成功，构建网页并且返回
                view.oneExpandHtml(question, html);
            }
            else
            {
                *html = "指定题目获取失败:" + number + "不存在";
                return false;
            }
            return true;
        }

        /**
         * @brief :编译判断传入的代码
         * @param number:题号
         * @param inJson:传入的序列化字符串
         * @param outJson:输出的序列化字符串
         */
        void judge(const std::string &number, const std::string inJson, std::string *outJson)
        {
            // 0.根据题号获得题目细节
            Question question;
            model.getOneQuestion(number, &question);
            // 1.反序列化，得到提交的源码和input
            Json::Read reader;
            Json::Value inValue;
            reader.parse(inJson, inValue);

            std::string code = inValue["code"].asString();

            // 2.重新拼接用户代码和测试用例代码，形成新的代码
            Json::value compileValue;
            compileValue["input"] = inValue["input"].asString();
            compileValue["code"] = code + question.tail;
            compileValue["cpuLimit"] = q.cpuLimit;
            compileValue["memLimit"] = q.memLimit;
            Json::FastWriter writer;
            std::string compileString = writer.write(compileValue);

            // 3.选择负载最低的主机
            // 一直选择，直到主机可用
            while (true)
            {
                int id = 0;
                Machine *m = nullptr;
                if (!loadBalance.intellientSelect(&id, &m))
                {
                    break;
                }
                LOG(INFO, "选择主机成功: %d --> %s:%d", id, m->ip.c_str(), m->port);

                // 4.发起http请求，获取运行结果
                Client client(m->ip, m->port);

                // 更新主机负载
                m->incLoad();
                if (auto res = client.Post("/compileAndRun", compileString, "application/json;charset=utf-8"))
                {
                    // 5.将运行结果输出到outJson
                    // 只有响应状态码为200，才算陈成功
                    if (res->status == 200)
                    {
                        *outJson = res->body;
                        LOG(INFO, "请求编译和运行成功");
                        m->decLoad();
                        break;
                    }
                    m->decLoad();
                }
                else
                {
                    // 请求失败，可能该主机已经离线
                    LOG(ERROR, "当前主机已经离线,id：%d 详情：%s:%d", id, m->ip.c_str(), m->ip);
                    loadBalance.offlineMachine(id);
                    loadBalance.printOnline();
                    loadBalance.printOffline();
                }
            }
        }
    };

};
