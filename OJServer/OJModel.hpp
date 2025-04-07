
#pragma once
// 根据题目mysql数据
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
        const std::string tableName = "oj_questions"; 
    public:
        Model()
        {
            assert(loadQuestionsList(questionsList));
        }

        bool queryMysql(const std::string &sql,vector<Question> *result)
        {

        }

        /**
         * @brief 获取加载后的所有题目
         * @param allQuestions :获取到的所有题目列表
         */
        bool getAllQuestions(std::vector<Question> *questionsList)
        {
            
        }

        /**
         * @brief 获取单个题目的信息
         * @param number :需要获取的题目编号
         * @param question :获取到的题目
         */
        bool getOneQuestion(const std::string &number, Question *question)
        {

        }

        ~Model() {}
    };
};