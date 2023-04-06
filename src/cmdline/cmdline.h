#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <iostream>
#include <list>
#include <memory>
#include <sstream>

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
              const std::string &describtion_,
              const std::list<std::string> &dep);
    virtual ~ParamBase() = default;

    virtual bool set(const std::string &) = 0;
    virtual bool hasParam() = 0;
    virtual std::string getRangeStr() = 0;

    void setEnable(bool);
    bool getEnable();
    std::string getName();
    std::string getShortName();
    std::string getDescription();
    std::list<std::string> &getDepList();

private:
    bool mEnable;
    std::string mName;
    std::string mShortName;
    std::string mDescribtion;
    std::list<std::string> mDep;
};

class ParamNone : public ParamBase
{
public:
    ParamNone(const std::string &name_,
              const std::string &shortName_,
              const std::string &describtion_,
              const std::list<std::string> &dep = std::list<std::string>()) :
              ParamBase(name_, shortName_, describtion_, dep){}
    ~ParamNone() = default;

protected:
    bool set(const std::string &) override
    {
        return false;
    }

    std::string getRangeStr() override
    {
        return "";
    }

    bool hasParam() override
    {
        return false;
    }
};

template<class Target, bool same>
class Reader
{
public:
    Target operator()(const std::string &str)
    {
        return str;
    }
};

template<class Target>
class Reader<Target, true>
{
public:
    Target operator()(const std::string &str)
    {
        Target ret;
        std::stringstream ss;
        if (!(ss << str && ss >> ret && ss.eof()))
        {
            CmdLineError err;
            err << "param error \"" << str << "\"";
            throw err;
        }
       
        return ret;
    }
};

template<template<typename T, typename T1 = std::allocator<T>> class STL,
         class T, bool same>
class RangeJudge
{
public:
    bool operator()(const T &value, const STL<T> &range)
    {
        if (range.size() > 0)
        {
            for (auto &r : range)
            {
                if (value == r)
                    return true;
            }
            CmdLineError err;
            err << "value \"" << value << "\" is out of range \n    ";
            for (auto &r : range)
                 err << r << "  ";
            
            throw err;
            return false;
        }
   
        return true;
    }
};

template<template<typename T, typename T1 = std::allocator<T>> class STL,
         class T>
class RangeJudge<STL, T, true>
{
public:
    bool operator()(const T &value, const STL<T> &range)
    {
        if (range.size() > 0 && (range.front() > value || range.back() < value))
        {
            CmdLineError err;
            err << "value range is " << range.front() << " to " << range.back()
                << " , " << value << " is out of range";
            throw err;
            return false;
        }
            
        return true;
    }
};

template<class T, bool same>
class RangeToStr
{
public:
    std::string operator()(const T &range)
    {
        if (!range.size())
            return "";

        std::string str("[");
        for (auto &r : range)
        {
            str += r;
            str += "  ";
        }
        str += "]";
        return str;
    }
};

template<class T>
class RangeToStr<T, true>
{
public:
    std::string operator()(const T &range)
    {
        if (!range.size())
            return "";

        return std::string("[") + std::to_string(range.front()) + " , " 
              + std::to_string(range.back()) + "]";
    }
};

template<template<typename T, typename T2 = std::allocator<T>> class STL,
         class T>
class ParamWithValue : public ParamBase
{
public:
    ParamWithValue(const std::string &name_,
                const std::string &shortName_,
                const std::string &describtion_,
                const std::list<std::string> &dep_,
                const STL<T> &range_) :
                range(range_),
                ParamBase(name_, shortName_, describtion_, dep_){}
    ~ParamWithValue() = default;

    STL<T> &get()
    {
        return data;
    }

protected:
    bool set(const std::string &value)
    {
        T ret = Reader<T, std::is_same<T, int>::value>()(value);
        if (!RangeJudge<STL, T, std::is_same<T, int>::value>()(ret, range))
            return false;

        data.push_back(ret);
        return true;
    }
    bool hasParam()
    {
        return true;
    }
    std::string getRangeStr()
    {
        return RangeToStr<STL<T>, std::is_same<T, int>::value>()(range);
    }

private:
    STL<T> data;
    STL<T> range;
};

class CmdLine
{
public:
    CmdLine() = default;
    ~CmdLine() = default;

    template<template<typename T, typename T2 = std::allocator<T>> class STL,
             class T, class S = STL<T>>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, 
             std::list<std::string> dep = std::list<std::string>(), S r = S())
    {
        paramTable.emplace_back(std::make_shared<ParamWithValue<STL, T>>(name, 
                                                            shortName,
                                                        describtion, dep, r));
    }

    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion,
             std::list<std::string> dep = std::list<std::string>())
    {
        paramTable.emplace_back(std::make_shared<ParamNone>(name, 
                                                            shortName,
                                                            describtion, dep));
    }

    template<template<typename T, typename T2 = std::allocator<T>> class STL,
             class T>
    bool get(const std::string &name, STL<T> &t)
    {
        for (auto &l : paramTable)
        {
            if ((l->getName() == name || l->getShortName() == name) &&
                 l->getEnable())
            {
                auto p = std::dynamic_pointer_cast<ParamWithValue<STL, T>>(l);
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
    void paramCheck();

private:
    std::list<std::shared_ptr<ParamBase>> paramTable;
    std::string cmd;
};
};

#endif