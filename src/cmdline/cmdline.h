#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <iostream>
#include <list>
#include <memory>

namespace lmc
{
class CmdLineError : public std::exception {
public:
    CmdLineError(const std::string &msg = ""): msg(msg){}
    ~CmdLineError() throw() {}
    const char *what() const throw() { return msg.c_str(); }
    CmdLineError &operator << (std::string str)
    {
        msg += str;
        return *this;
    }

    CmdLineError &operator << (const char *str)
    {
        msg += str;
        return *this;
    }

    CmdLineError &operator << (int num)
    {
        msg += std::to_string(num);
        return *this;
    }

private:
    std::string msg;
};

class ParamBase
{
public:
    ParamBase(const std::string &name_,
              const std::string &shortName_,
              const std::string &describtion_);
    virtual ~ParamBase() = default;

    virtual bool set(int) = 0;
    virtual bool set(const std::string &) = 0;
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
    bool set(int) override
    {
        return false;
    }

    bool set(const std::string &) override
    {
        return false;
    }

    bool hasParam() override;
};

template<template<typename T, typename T2 = std::allocator<T>> class STL>
class ParamInt : public ParamBase
{
public:
    ParamInt(const std::string &name_,
             const std::string &shortName_,
             const std::string &describtion_,
             const STL<int> &r) :
             ParamBase(name_, shortName_, describtion_),
             range(r)
             {}

    ~ParamInt() = default;

    STL<int> get()
    {
        return data;
    }

protected:
    bool set(int value) override
    {
        if (range.size() > 0 && (range.front() > value || range.back() < value))
        {
            CmdLineError err;
            err << "value range is " << range.front() << " to " << range.back()
                << " , " << value << " is out of range";
            throw err;
            return false;
        }
            
        data.push_back(value);
        return true;
    }

    bool set(const std::string &value) override
    {
        return false;
    }

    bool hasParam() override
    {
        return true;
    }

private:
    STL<int> data;
    STL<int> range;
};

template<template<typename T, typename T2 = std::allocator<T>> class STL>
class ParamStr : public ParamBase
{
public:
    ParamStr(const std::string &name_,
             const std::string &shortName_,
             const std::string &describtion_,
             const STL<std::string> &r) :
             ParamBase(name_, shortName_, describtion_),
             range(r)
             {}

    ~ParamStr() = default;

    STL<std::string> get()
    {
        return data;
    }

protected:
    bool set(int value) override
    {
        return false;
    }

    bool set(const std::string &value) override
    {
        if (range.size() > 0)
        {
            for (auto &r : range)
            {
                if (value == r)
                {
                    data.push_back(value);
                    return true;
                }    
            }
            CmdLineError err;
            err << "value \"" << value << "\" is out of range \n    ";
            for (auto &r : range)
            {
                 err << r << "  ";
            }
            throw err;
            return false;
        }
        data.push_back(value);
        return true;
    }

    bool hasParam() override
    {
        return true;
    }

private:
    STL<std::string> data;
    STL<std::string> range;
};

class CmdLine
{
public:
    CmdLine() = default;
    ~CmdLine() = default;

    template<template<typename T, typename T2 = std::allocator<T>> class STL, 
             class T>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, STL<int> r)
    {
        paramTable.emplace_back(std::make_shared<ParamInt<STL>>(name, 
                                                            shortName,
                                                        describtion, r));
    }

    template<template<typename T, typename T2 = std::allocator<T>> class STL, 
             class T>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, STL<std::string> r)
    {
        paramTable.emplace_back(std::make_shared<ParamStr<STL>>(name, 
                                                            shortName,
                                                        describtion, r));
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