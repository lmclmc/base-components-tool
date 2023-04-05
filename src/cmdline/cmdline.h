#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <iostream>
#include <list>
#include <memory>

#include "log/log.h"

namespace lmc
{
class ParamBase
{
public:
    ParamBase(const std::string &name_,
              const std::string &shortName_,
              const std::string &describtion_);
    virtual ~ParamBase() = default;

    virtual void set(int) = 0;
    virtual void set(const std::string &) = 0;
    virtual bool hasParam() = 0;

    void setEnable(bool);
    bool getEnable();
    std::string getName();
    std::string getShortName();
    std::string getDescription();

private:
    bool mEnable;
    std::string mName;
    std::string mShortName;
    std::string mDescribtion;
};

class ParamNone : public ParamBase
{
public:
    ParamNone(const std::string &name_,
              const std::string &shortName_,
              const std::string &describtion_) :
              ParamBase(name_, shortName_, describtion_){}
    ~ParamNone() = default;

protected:
    void set(int) override
    {}

    void set(const std::string &) override
    {}

    bool hasParam() override;
};

template<template<typename T, typename T2 = std::allocator<T>> class STL>
class ParamInt : public ParamBase
{
public:
    ParamInt(const std::string &name_,
             const std::string &shortName_,
             const std::string &describtion_) :
             ParamBase(name_, shortName_, describtion_){}

    ~ParamInt() = default;

    STL<int> get()
    {
        return data;
    }

protected:
    void set(int value) override
    {
        data.push_back(value);
    }

    void set(const std::string &value) override
    {}

    bool hasParam() override
    {
        return true;
    }

private:
    STL<int> data;
};

template<template<typename T, typename T2 = std::allocator<T>> class STL>
class ParamStr : public ParamBase
{
public:
    ParamStr(const std::string &name_,
             const std::string &shortName_,
             const std::string &describtion_) :
             ParamBase(name_, shortName_, describtion_){}

    ~ParamStr() = default;

    STL<std::string> get()
    {
        return data;
    }

protected:
    void set(int value) override
    {}

    void set(const std::string &value) override
    {
        data.push_back(value);
    }

    bool hasParam() override
    {
        return true;
    }

private:
    STL<std::string> data;
};

class CmdLine
{
public:
    CmdLine() = default;
    ~CmdLine() = default;

    template<template<typename T, typename T2 = std::allocator<T>> class STL, 
             class T>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion)
    {
        if (std::is_same<T, int>::value)
        {
            paramTable.emplace_back(std::make_shared<ParamInt<STL>>(name, 
                                                                shortName,
                                                            describtion));
        }
        else if (std::is_same<T, std::string>::value)
        {
            paramTable.emplace_back(std::make_shared<ParamStr<STL>>(name, 
                                                                shortName,
                                                            describtion));
        }
    }

    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion)
    {
        paramTable.emplace_back(std::make_shared<ParamNone>(name, 
                                                            shortName,
                                                            describtion));
    }

    template<template<typename T, typename T2 = std::allocator<T>> class STL>
    bool get(const std::string &name, STL<int> &t)
    {
        for (auto &l : paramTable)
        {
            if ((l->getName() == name || l->getShortName() == name) &&
                 l->getEnable())
            {
                auto p = std::dynamic_pointer_cast<ParamInt<STL>>(l);
                t.clear();
                t = p->get();
                return true;
            }
        }
        return false;
    }

    template<template<typename T, typename T2 = std::allocator<T>> class STL>
    bool get(const std::string &name, STL<std::string> &t)
    {
        for (auto &l : paramTable)
        {
            if ((l->getName() == name || l->getShortName() == name) &&
                 l->getEnable())
            {
                auto p = std::dynamic_pointer_cast<ParamStr<STL>>(l);
                t.clear();
                t = p->get();
                return true;
            }
        }
        return false;
    }

    bool get(const std::string &name)
    {
        for (auto &l : paramTable)
        {
            if ((l->getName() == name || l->getShortName() == name) &&
                 l->getEnable())
                return true;
        }

        return false;
    }

    void parse(int, char *[]);

private:
    void showHelp();

private:
    std::list<std::shared_ptr<ParamBase>> paramTable;
    std::string cmd;
};
};

#endif