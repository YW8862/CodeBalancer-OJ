
#pragma once
// 根据题目list文件，加载所有的题目信息到内存里面
// model:用来和数据进行交互，对外提供数据的接口

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <unordered_map>
#include "../common/log.hpp"
#include "../common/utils.hpp"

namespace ns_model
{
    using namespace ns_utils;

    const std::string questionsList = "./questions/questions.list";
    const std::string questionPath = "./questions/";

    struct Question
    {
        std::string number;      // 题目编号
        std::string title;       // 题目标题
        std::string desc;        // 题目描述
        std::string difficulity; // 题目难度
        std::string header;      // 题目预设代码
        std::string tail;        // 题目测试用例

        int cpuLimit;           // 时间限制
        int memLimit;           // 空间限制
    };

    class Model
    {
    private:
        std::unordered_map<std::string, Question> questions;

    public:
        Model()
        {
            assert(loadQuestionsList(questionsList));
        }

        /**
         * @brief: 将题目加载到哈希题目列表中
         * @param questionsList :题目列表路径
         */
        bool loadQuestionsList(const std::string &questionsList)
        {
            std::ifstream in(questionsList);
            if (!in.is_open())
            {

                LOG(ERROR, "打开题目列表失败，请检查路径");
                return false;
            }
            std::string str;
            while (std::getline(in, str))
            {
                std::vector<std::string> tokens;
                StringUtils::splitString(str, &tokens, " ");

                if (tokens.size() != 5)
                {
                    LOG(ERROR, "加载题目失败，题目列表格式错误，检查题目列表格式");
                    continue;
                }
                // 创建并且填充题目对象
                Question question;
                question.number = tokens[0];
                question.title = tokens[1];
                question.difficulity = tokens[2];
                question.cpuLimit = atoi(tokens[3].c_str());
                question.memLimit = atoi(tokens[4].c_str());
                std::string questionNumPath = questionPath + tokens[0] + "/";

                FileUtil::readFile(questionNumPath + "desc.txt", &question.desc, true);
                FileUtil::readFile(questionNumPath + "header.cpp", &question.header, true);
                FileUtil::readFile(questionNumPath + "tail.cpp", &question.tail, true);
                questions.insert({question.number, question});
                //std::cout<<"11111111111"<<question.desc<<std::endl;
                LOG(INFO, "加载题目成功:%s",question.number.c_str());
            }
            
            
            // 关闭文件
            in.close();
            return true;
        }

        /**
         * @brief 获取加载后的所有题目
         * @param allQuestions :获取到的所有题目列表
         */
        bool getAllQuestions(std::vector<Question> *questionsList)
        {
            // 如果题目列表数量为0，获取题目失败，返回
            if (questions.size() == 0)
            {
                LOG(ERROR, "获取题库失败");
                return false;
            }
            for (auto &iter : questions)
            {
                questionsList->push_back(iter.second);
            }
            return true;
        }

        /**
         * @brief 获取单个题目的信息
         * @param number :需要获取的题目编号
         * @param question :获取到的题目
         */
        bool getOneQuestion(const std::string &number, Question *question)
        {
            if (questions.find(number) == questions.end())
            {
                LOG(ERROR, "获取题目失败，题目不存在，题目编号：%s", number.c_str());
                return false;
            }
            
            *question = questions[number];
            return true;
        }

        ~Model() {}
    };
};