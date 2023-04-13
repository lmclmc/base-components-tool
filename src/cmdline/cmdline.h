#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <stack>

namespace lmc
{
template<typename ...Args>
struct TypeList;

template<typename ...Args>
struct PushType;

template<typename NewType, typename ...Args>
struct PushType<NewType, TypeList<Args...>>
{
    using type = TypeList<Args..., NewType>;
};

using EmptyTypeList = TypeList<>;
#define PUSH_UNSIGNED_CHAR  PushType<unsigned char, EmptyTypeList>::type
#define PUSH_UNSIGNED_SHORT PushType<unsigned short, PUSH_UNSIGNED_CHAR>::type
#define PUSH_SHORT          PushType<short, PUSH_UNSIGNED_SHORT>::type
#define PUSH_UNSIGNED_INT   PushType<unsigned int, PUSH_SHORT>::type
#define PUSH_INT            PushType<int, PUSH_UNSIGNED_INT>::type
#define PUSH_UNSIGNED_LONG  PushType<unsigned long, PUSH_INT>::type
#define PUSH_LONG           PushType<long, PUSH_UNSIGNED_LONG>::type
#define PUSH_FLOAT          PushType<float, PUSH_LONG>::type
#define PUSH_DOUBLE         PushType<double, PUSH_FLOAT>::type
using NumTypeList = PUSH_DOUBLE;

template<typename ...Args>
struct Search;

template<typename TargetType, typename ...Args>
struct Search<TargetType, TypeList<TargetType, Args...>>
{
    constexpr static bool status = true;
    constexpr static int value = 0;
};

template<typename TargetType, typename HeadType, typename ...Args>
struct Search<TargetType, TypeList<HeadType, Args...>>
{
    constexpr static bool status = Search<TargetType, TypeList<Args...>>::status;
    constexpr static int tmp = Search<TargetType, TypeList<Args...>>::value;
    constexpr static int value = tmp == -1 ? -1 : tmp + 1;
};

template<typename TargetType>
struct Search<TargetType, TypeList<>>
{
    constexpr static bool status = false;
    constexpr static int value = -1;
};


template<typename ...Args>
struct BreakDown;

template<template<typename ...Args> class STL, typename T, typename ...Args>
struct BreakDown<STL<T, Args...>>
{
    using type = T;
};

template<template<typename ...Args> class STL, typename T>
struct ReBind
{
    using type = STL<T>;
};

template<typename STL_T, typename T, int StlIdx>
struct PushData;

template<typename STL_T, typename T>
struct PushData<STL_T, T, 0>
{
    void operator()(STL_T &data, T &t)
    {
        
        data.push_back(t);
    }

    static bool traverse(const STL_T &range, const T &value)
    {
        for (auto &r : range)
        {
            if (value == r)
                return true;
        }

        return false;
    }

    static std::string getTraverseStr(const STL_T &range)
    {
        std::string str;
        for (auto &r : range)
        {
            str += r;
            str += ' ';
        }

        return str;
    }
};

template<typename STL_T, typename T>
struct PushData<STL_T, T, 1>
{
    void operator()(STL_T &data, T &t)
    {
        data.push_back(t);
    }

    static bool traverse(STL_T &range, T &value)
    {
        for (auto &r : range)
        {
            if (value == r)
                return true;
        }

        return false;
    }

    static std::string getTraverseStr(const STL_T &range)
    {
        std::string str;
        for (auto &r : range)
        {
            str += r;
            str += ' ';
        }

        return str;
    }
};

template<typename STL_T, typename T>
struct PushData<STL_T, T, 2>
{
    void operator()(STL_T &data, T &t)
    {
        data.insert(t);
    }

    static bool traverse(STL_T &range, T &value)
    {
        for (auto &r : range)
        {
            if (value == r)
                return true;
        }

        return false;
    }

    static std::string getTraverseStr(const STL_T &range)
    {
        std::string str;
        for (auto &r : range)
        {
            str += r;
            str += ' ';
        }

        return str;
    }
};

template<typename STL_T, typename T>
struct PushData<STL_T, T, 3>
{
    void operator()(STL_T &data, T &t)
    {
        data.push_back(t);
    }

    static bool traverse(STL_T &range, T &value)
    {
        for (auto &r : range)
        {
            if (value == r)
                return true;
        }

        return false;
    }

    static std::string getTraverseStr(const STL_T &range)
    {
        std::string str;
        for (auto &r : range)
        {
            str += r;
            str += ' ';
        }

        return str;
    }
};

template<typename STL_T, typename T>
struct PushData<STL_T, T, 4>
{
    void operator()(STL_T &data, T &t)
    {
        data.push(t);
    }

    static bool traverse(STL_T &range, T &value)
    {
        bool status = false;
        for (int i = 0; i < range.size(); i++)
        {
            if (value == range.front())
                status = true;

            range.push(range.front());
            range.pop();
        }

        return status;
    }

    static std::string getTraverseStr(STL_T &range)
    {
        std::string str;
        STL_T tmp = range;
        int size = tmp.size();
        for (int i = 0; i < size; i++)
        {
            str += tmp.front();
            str += ' ';
            tmp.pop();
        }     

        return str;
    }
};


template<typename STL_T, typename T>
struct PushData<STL_T, T, 5>
{
    void operator()(STL_T &data, T &t)
    {
        data.push(t);
    }

    static bool traverse(STL_T &range, T &value)
    {
        bool status = false;
        STL_T tmp = range;
        int size = tmp.size();
        for (int i = 0; i < size; i++)
        {
            if (value == tmp.top())
                status = true;

            tmp.pop();
        }

        return status;
    }

    static std::string getTraverseStr(STL_T &range)
    {
        std::string str;
        STL_T tmp = range;
        int size = tmp.size();
        for (int i = 0; i < size; i++)
        {
            str += tmp.top();
            str += ' ';
            tmp.pop();
        }     

        return str;
    }
};

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

template<class Target, bool IsNum>
struct Reader
{
    Target operator()(const std::string &str)
    {
        return str;
    }
};

template<class Target>
struct Reader<Target, true>
{
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

template<typename T, typename STL_T, int idx, bool IsNum>
struct RangeJudge
{
    bool operator()(T &value, STL_T &range)
    {
        if (range.size() > 0)
        {
            if (PushData<STL_T, T, idx>::traverse(range, value))
                return true;

            CmdLineError err;
            err << "value \"" << value << "\" is out of range \n    ";
            
            throw err;
            return false;
        }
   
        return true;
    }
};

template<typename T, typename STL_T, int idx>
struct RangeJudge<T, STL_T, idx, true>
{
    bool operator()(const T &value, const STL_T &range)
    {
        if (range.size() > 0 && (*range.begin() > value || 
            *(--range.end()) < value))
        {
            CmdLineError err;
            err << "value range is " << *range.begin() << " to " 
                << *(--range.end()) << " , " << value << " is out of range";
            throw err;
            return false;
        }
            
        return true;
    }
};

template<typename T, typename STL_T>
struct RangeJudge<T, STL_T, 4, true>
{
    bool operator()(T &value, STL_T &range)
    {
        if (range.size() > 0)
        {
            STL_T tmp = range;
            int min = tmp.front();
            range.pop();
            int max = tmp.front();

            if (min > value || max < value)
            {
                CmdLineError err;
                err << "value range is " << min << " to " 
                    << max << " , " << value << " is out of range";
                throw err;
                return false;
            }
        }
    
        return true;
    }
};

template<typename T, typename STL_T>
struct RangeJudge<T, STL_T, 5, true>
{
    bool operator()(T &value, STL_T &range)
    {
        STL_T tmp = range;
        if (range.size() > 0)
        {
            int max = tmp.top();
            tmp.pop();
            int min = tmp.top();

            if (min > value || max < value)
            {
                CmdLineError err;
                err << "value range is " << min << " to " 
                    << max << " , " << value << " is out of range";
                throw err;
                return false;
            }
        }
    
        return true;
    }
};

template<class STL_T, class T, int idx, bool IsNum>
struct RangeToStr
{
    std::string operator()(STL_T &range)
    {
        if (!range.size())
            return "";

        std::string str("[");
        str += PushData<STL_T, T, idx>::getTraverseStr(range);
        str += "]";
        return str;
    }
};

template<class STL_T, class T, int idx>
struct RangeToStr<STL_T, T, idx, true>
{
    std::string operator()(STL_T &range)
    {
        if (!range.size())
            return "";

        return std::string("[") + std::to_string(*range.begin()) + " , " 
              + std::to_string(*(--range.end())) + "]";

    }
};

template<class STL_T, class T>
struct RangeToStr<STL_T, T, 4, true>
{
    std::string operator()(STL_T &range)
    {
        STL_T tmp = range;
        if (!tmp.size())
            return "";
        
        int min = tmp.front();
        tmp.pop();
        int max = tmp.front();

        return std::string("[") + std::to_string(min) + " , " 
              + std::to_string(max) + "]";
    }
};

template<class STL_T, class T>
struct RangeToStr<STL_T, T, 5, true>
{
    std::string operator()(STL_T &range)
    {
        STL_T tmp = range;
        if (!tmp.size())
            return "";
        
        int min = tmp.top();
        tmp.pop();
        int max = tmp.top();

        return std::string("[") + std::to_string(min) + " , " 
              + std::to_string(max) + "]";
    }
};

template<class STL_T>
class ParamWithValue final : public ParamBase
{
    using T = typename BreakDown<STL_T>::type;
    using ListType = typename ReBind<std::list, T>::type;
    using VectorType = typename ReBind<std::vector, T>::type;
    using SetType = typename ReBind<std::set, T>::type;
    using DequeType = typename ReBind<std::deque, T>::type;
    using QueueType = typename ReBind<std::queue, T>::type;
    using StackType = typename ReBind<std::stack, T>::type;
    using EmptyStl = TypeList<>;
    using PushListType = typename PushType<ListType, EmptyStl>::type;
    using PushVectorType = typename PushType<VectorType, PushListType>::type;
    using PushSetType = typename PushType<SetType, PushVectorType>::type;
    using PushDequeType = typename PushType<DequeType, PushSetType>::type;
    using PushQueueType = typename PushType<QueueType, PushDequeType>::type;
    using PushStackType = typename PushType<StackType, PushQueueType>::type;
    using STLList = PushStackType;
public:
    ParamWithValue(const std::string &name_,
                const std::string &shortName_,
                const std::string &describtion_,
                const std::list<std::string> &dep_,
                const STL_T &range_) :
                range(range_),
                ParamBase(name_, shortName_, describtion_, dep_){}
    ~ParamWithValue() = default;

    STL_T &get()
    {
        return data;
    }

protected:
    bool set(const std::string &value) override
    {
        T ret = Reader<T, Search<T, NumTypeList>::status>()(value);
        if (!RangeJudge<T, STL_T, Search<STL_T, STLList>::value, 
                        Search<T, NumTypeList>::status>()(ret, range))
            return false;
        
        PushData<STL_T, T, Search<STL_T, STLList>::value>()(data, ret);
        return true;
    }

    bool hasParam() override
    {
        return true;
    }

    std::string getRangeStr() override
    {
       return RangeToStr<STL_T, T, Search<STL_T, STLList>::value, Search<T, NumTypeList>::status>()(range);
    }

private:
    STL_T data;
    STL_T range;
};

class CmdLine
{
public:
    CmdLine() = default;
    ~CmdLine() = default;

    template<class STL_T>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, 
             std::list<std::string> dep = std::list<std::string>(), STL_T r = STL_T())
    {
        paramTable.emplace_back(std::make_shared<ParamWithValue<STL_T>>(name, 
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

    template<class STL_T>
    bool get(const std::string &name, STL_T &t)
    {
        for (auto &l : paramTable)
        {
            if ((l->getName() == name || l->getShortName() == name) &&
                 l->getEnable())
            {
                auto p = std::dynamic_pointer_cast<ParamWithValue<STL_T>>(l);
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