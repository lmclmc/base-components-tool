#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <iostream>
#include <list>
#include <memory>

#include "log/log.h"

namespace lmc
{
enum class CmdType : unsigned char
{
    None,           //选项下无参数模式
    StringNoRange,  //选项下参数为字符串类型，并没有参数范围
    StringRange,    //选项下参数为字符串类型，并且有参数范围
    IntRange,       //选项下参数为数字类型，并有参数范围
    IntRepeat,      //可以重复设置该选项
    Help,           //当标志该类型时，该选项将变成帮助选项
};

class ParamBase
{
public:
    ParamBase();
    virtual std::string getName() = 0;
    virtual std::string getShortName() = 0;
    virtual std::string getDescription() = 0;
    virtual CmdType getCmdType() = 0;
    bool getEnable();
    bool setEnable();

private:
    bool enable;
};

class ParamNone : public ParamBase
{
public:
    ParamNone(const std::string &, const std::string &, const std::string &);
    CmdType getCmdType() override;
    std::string getName() override;
    std::string getShortName() override;
    std::string getDescription() override;

private:
    std::string mName;
    std::string mShortName;
    std::string mDescryption;
};

class ParamStringNoRange : public ParamNone
{
public:
    ParamStringNoRange(const std::string &, const std::string &, 
                       const std::string &);
    CmdType getCmdType() override;

    std::string getStr();
    void setStr(std::string str);

private:
    std::string mStr;
};

class ParamIntRepeat : public ParamNone
{
public:
    ParamIntRepeat(const std::string &, const std::string &, 
                   const std::string &, int, int);
    CmdType getCmdType() override;
    int getMin();
    int getMax();
    void pushBack(int num);
    std::list<int> &getList();

private:
    std::list<int> mList;
    int mMin;
    int mMax;
};

class ParamHelp final : public ParamNone
{
public:
    ParamHelp(const std::string &, const std::string &, const std::string &);
    CmdType getCmdType() override;
};

class CmdLine
{
public:
    /*
    * @brief 添加参数设置
    * @param 第一个参数为详细选项，第二个参数为简短选项，第三个参数为该选项描述信息
    *        第四个参数为选项参数类型，后面的参数均为动态参数。
    * @return void
    */
    template<typename ...Args>
    void add(Args &&...args)
    {
        addPriv(args...);
    }

    bool get(std::string name);

    bool get(std::string name, std::list<int> &list);

    bool get(std::string name, std::string &str);

    bool parse(int argc, char *argv[]);

private:
    template<typename ...Args>
    void addPriv(std::string str, Args &&...args)
    {
        switch (mTmp)
        {
            case 0:
            mShortName = str;
            break;
            case 1:
            mName = str;
            break;
            case 2:
            mDescryption = str;
            break;
        }
        mTmp++;

        addPriv(args...);
    }

    template<typename ...Args>
    void addPriv(CmdType &cmd, Args &...args)
    {
        mCmdType = cmd;
        switch (cmd)
        {
            case CmdType::Help:{
            auto ptr = std::make_shared<ParamHelp>(mName, 
                                                mShortName, 
                                                mDescryption);
            mParamList.emplace_back(ptr);
            break;}
            case CmdType::None:{
            auto ptr = std::make_shared<ParamNone>(mName, 
                                                mShortName, 
                                                mDescryption);
            mParamList.emplace_back(ptr);
            break;}
        }
        mTmp = 0;

        addPriv(args...);
    }

    void addPriv(int &min, int &max)
    {
        mMin = min;
        mMax = max;
        if (mCmdType == CmdType::IntRepeat)
        {
            auto ptr = std::make_shared<ParamIntRepeat>(mName,
                                                        mShortName, 
                                                        mDescryption,
                                                        mMin, mMax);
            mParamList.emplace_back(ptr);
        }
    }

    void addPriv(std::string &str)
    {
        if (mCmdType == CmdType::StringNoRange)
        {
            auto ptr = std::make_shared<ParamStringNoRange>(mName,
                                                        mShortName, 
                                                        mDescryption);
            mParamList.emplace_back(ptr);
        }
    }

    void addPriv(){}

    void showHelp();

private:
    std::list<std::shared_ptr<ParamBase>> mParamList;

    std::string mName;
    std::string mShortName;
    std::string mDescryption;

    CmdType mCmdType;

    int mMin, mMax;
    int mTmp;
};
}

#endif