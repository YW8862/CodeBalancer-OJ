/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-24 14:31:39
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-24 22:25:09
 * @FilePath: /yw/projects/onlineJudge/common/utils.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>

namespace ns_utils
{
    const std::string tempPath = "./temp/";
    class PathUtils
    {
    public:
        /**
         * @brief:拼接文件路径
         * @param fileName:需要拼接的文件名，不包括路径和后缀
         * @param suffix:文件后缀
         */
        static std::string addSuffix(const std::string &fileName, const std::string &suffix)
        {
            std::string stringPath = tempPath + fileName + suffix;
            return stringPath;
        }
        /**
         * @param fileName:文件名
         */
        static std::string src(const std::string &fileName)
        {
            return addSuffix(fileName, ".cpp");
        }
        static std::string exe(const std::string &fileName)
        {
            return addSuffix(fileName, ".exe");
        }
        static std::string stderr(const std::string &fileName)
        {
            return addSuffix(fileName, ".stderr");
        }
    };

    class FileUtil
    {
    public:
        static bool isFileExists(const std::string &fileName)
        {
            struct stat statbuf;
            return stat(fileName.c_str(), &statbuf) == 0;
            // return access(fileName.c_str(),F_OK) == 0;
        }
    };
    
    class TimeUtil
    {
    public:
        static std::string getTimeStamp()
        {
            struct timeval time;
            gettimeofday(&time, nullptr);
            return std::to_string(time.tv_sec);
        }
    };
};