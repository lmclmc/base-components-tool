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
    virtual std::string getRange() = 0;

    void setEnable(bool);
    bool getEnable();
    std::string getName();
    std::string getShortName();
    std::string getDescription();
    std::string getRangeStr();
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
    // bool set(int) override
    // {
    //     return false;
    // }

    bool set(const std::string &) override
    {
        return false;
    }

    std::string getRange() override
    {
        return "";
    }

    bool hasParam() override
    {
        return false;
    }
};

template<class Target, class Source, bool same>
class TypeCast
{
public:
    static Target cast(const Source &source)
    {
        Target ret;
        std::stringstream ss;
        ss << source;
        ss >> ret;   
        return ret;
    }
};

template<class Target>
class TypeCast<Target, std::string, true>
{
public:
    static Target cast(const std::string &source)
    {
        return source;
    }
};

template<class Target>
struct Reader
{
    Target operator()(const std::string &str)
    {
        return TypeCast<Target, std::string, 
                        std::is_same<Target, std::string>::value>::cast(str);
    }
};

// template<template<typename T, typename T2 = std::allocator<T>> class STL,
//          class Target, bool same>
// class RangeTypeJudge
// {
// public:
//     static Target judge(const Source &source)
//     {
//     }
// };

// template<template<typename T, typename T2 = std::allocator<T>> class STL,
//          class Target, bool same>
// class RangeTypeJudge<STL, std::string, true>
// {
// public:
//     static Target judge(const Source &source)
//     {
//     }
// };

// template<template<typename T, typename T2 = std::allocator<T>> class STL,
//          class T>
// struct RangeJudge
// {
//     bool operator()(const T value, STL<T> range)
//     {

//     }
// };

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
        T ret = Reader<T>()(value);
        // if (!RangeJudge<STL, T>(ret, range))
        //     return false;

        data.push_back(ret);
        return true;
    }
    bool hasParam()
    {
        return true;
    }
    std::string getRange()
    {
        return "out of range";
    }

private:
    STL<T> data;
    STL<T> range;
};

template<template<typename T, typename T2 = std::allocator<T>> class STL>
class ParamInt : public ParamBase
{
public:
    ParamInt(const std::string &name_,
             const std::string &shortName_,
             const std::string &describtion_,
             const STL<int> &r,
             const std::list<std::string> &dep) :
             ParamBase(name_, shortName_, describtion_, dep),
             range(r)
             {}

    ~ParamInt() = default;

    STL<int> get()
    {
        return data;
    }

protected:
    // bool set(int value) override
    // {
    //     if (range.size() > 0 && (range.front() > value || range.back() < value))
    //     {
    //         CmdLineError err;
    //         err << "value range is " << range.front() << " to " << range.back()
    //             << " , " << value << " is out of range";
    //         throw err;
    //         return false;
    //     }
            
    //     data.push_back(value);
    //     return true;
    // }

    bool set(const std::string &value) override
    {
        return false;
    }

    std::string getRange() override
    {
        if (!range.size())
            return "";

        return std::string("[") + std::to_string(range.front()) + " , " 
              + std::to_string(range.back()) + "]";
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
             const STL<std::string> &r,
             const std::list<std::string> &dep) :
             ParamBase(name_, shortName_, describtion_, dep),
             range(r)
             {}

    ~ParamStr() = default;

    STL<std::string> get()
    {
        return data;
    }

protected:
    // bool set(int value) override
    // {
    //     return false;
    // }

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

    std::string getRange() override
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