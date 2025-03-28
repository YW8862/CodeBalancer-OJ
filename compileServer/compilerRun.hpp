/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-24 14:05:18
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-27 16:51:19
 * @FilePath: /yw/projects/onlineJudge/compileServer/compilerRun.hpp
 * @Description: 提供编译并运行服务，需要确保文件唯一性
 */
#pragma once
#include <unordered_map>
#include <signal.h>
#include "compiler.hpp"
#include "runner.hpp"
#include <jsoncpp/json/json.h>
#include "../common/utils.hpp"
#include "../common/log.hpp"

namespace ns_compileAndRun
{
    using namespace ns_utils;
    using namespace ns_compile;
    using namespace ns_runner;

    class compileAndRun
    {
    public:
        /**
         * @brief:清理临时文件
         */
        static void removeTempFile(const std::string &fileName)
        {
            // 清理文件
            std::string _src = PathUtils::src(fileName);
            if (FileUtil::isFileExists(_src))
                unlink(_src.c_str());

            std::string _compilerError = PathUtils::compilererr(fileName);
            if (FileUtil::isFileExists(_compilerError))
                unlink(_compilerError.c_str());

            std::string _execute = PathUtils::exe(fileName);
            if (FileUtil::isFileExists(_execute))
                unlink(_execute.c_str());

            std::string _stdin = PathUtils::stdin(fileName);
            if (FileUtil::isFileExists(_stdin))
                unlink(_stdin.c_str());

            std::string _stdout = PathUtils::stdout(fileName);
            if (FileUtil::isFileExists(_stdout))
                unlink(_stdout.c_str());

            std::string _stderr = PathUtils::stderr(fileName);
            if (FileUtil::isFileExists(_stderr))
                unlink(_stderr.c_str());
        }

        /**
         * @brief :获取错误描述
         * @param statusCode:退出状态码
         * statusCode > 0:运行收到信号被终止
         * statusCode < 0:非运行报错
         * statusCode = 0:正常运行
         */
        static std::string signoToDesc(int statusCode)
        {

            std::string desc;
            switch (statusCode)
            {
            case 0:
                desc = "运行成功";
                break;
            case -1:
                desc = "用户提交代码为空";
                break;
            case -2:
                desc = "未知错误";
                break;
            case -3:
                desc = "编译错误";
                break;
            case SIGABRT:
                desc = "内存超出";
                break; // 6号信号
            case SIGXCPU:
                desc = "运行超时";
                break; // 24号信号
            case SIGFPE:
                desc = "浮点数溢出";
                break;
            default:
                desc = "未知错误 " + std::to_string(statusCode);
                break;
            }

            return desc;
        }

        /**
         * @brief
         * @param inputJson:输入序列化数据
         * 输入参数:
         * code：用户提交的代码
         * input：用户给自己提交的代码对应的输入，不做处理
         * cpuLimit:时间限制
         * memLimit:空间限制
         * @param outputJson:输出序列化结果
         * 输出参数：
         * status：状态码
         * reson：请求结果
         * stdout：执行结果
         * stderr：错误结果
         */
        static void start(const std::string &inputJson, std::string *outputJson)
        {
            // 1.反序列化，解析json为多个字符串
            Json::Value inputValue;
            Json::Value outputValue;
            Json::Reader reader;

            reader.parse(inputJson, inputValue); // 最后处理差错问题

            std::string code = inputValue["code"].asString();   // 用户提交代码
            std::string input = inputValue["input"].asString(); // 输入
            int cpuLimit = inputValue["cpuLimit"].asInt();      // 运行时间限制
            int memLimit = inputValue["memLimit"].asInt();      // 虚拟内存限制

            int statusCode = 0;   // 编译运行状态码
            std::string fileName; // 定义文件名
            int result = 0;       // 运行结束状态码
            // 说明用户没有输入代码
            if (code.size() == 0)
            {
                // 代码为空
                statusCode = -1;
                goto End;
            }

            // 形成唯一的文件名，没有目录和后缀
            // 毫秒级时间戳+原子性递增的唯一值来保证唯一性
            fileName = FileUtil::UniqFileName(code);

            // 形成临时源文件,写入文件内容
            if (!FileUtil::writeFile(PathUtils::src(fileName), code))
            {
                // 如果写入失败
                statusCode = -2;
                // 序列化结果
                goto End;
            }

            // 编译程序
            if (!Compiler::Compile(fileName))
            {
                // 如果编译失败
                statusCode = -3;
                std::string compileDesc;
                if (!FileUtil::readFile(PathUtils::compilererr(fileName), &compileDesc, true))
                {
                    // 打开错误文件失败
                    LOG(ERROR, "读取文件%s.stderr失败", fileName);
                }
                goto End;
            }

            // 运行程序
            result = Runner::run(fileName, cpuLimit, memLimit);
            if (result < 0)
            {
                statusCode = -2;
                goto End;
            }
            else if (result > 0)
            {
                // 运行错误
                statusCode = result;
                goto End;
            }
            else
            {
                statusCode = 0;
            }

        End:
            outputValue["status"] = statusCode;
            outputValue["reason"] = compileAndRun::signoToDesc(statusCode);
            if (statusCode >= 0)
            {
                std::string stdoutFile;
                if (!FileUtil::readFile(PathUtils::stdout(fileName), &stdoutFile, true))
                {
                    // 打开错误文件失败
                    LOG(ERROR, "读取文件%s.stderr失败", fileName);
                }
                outputValue["stdout"] = stdoutFile;

                std::string stderrFile;
                if (!FileUtil::readFile(PathUtils::stderr(fileName), &stderrFile, true))
                {
                    // 打开错误文件失败
                    LOG(ERROR, "打开文件%s.stderr失败", fileName);
                }
                outputValue["stderr"] = stderrFile;
            }
            Json::StyledWriter writer;
            *outputJson = writer.write(outputValue);
            std::cout << "------------------------------" << std::endl;
            removeTempFile(fileName);
        };
    };
};