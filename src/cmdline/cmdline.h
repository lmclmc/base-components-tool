/*
 * @Description: 命令行解析头文件，支持容器list,vector,set,deque,stack,queue
 *               forward_list,unordered_set,multiset,unordered_multiset
 *               支持多次，不同类型参数输入。
 * @Author: luomincheng
 * @Date: 2023-04-16 15:28:55 
 * @Last Modified time: 2023-04-16 16:45:48
 */

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
#include <forward_list>
#include <unordered_set>

#include "type/type.h"

namespace lmc
{
typedef enum class STLType_ : unsigned char
{
    SINGLE, //no stl                idx: -1
    NONE, //none                    idx: 0
    VLD, //vector list deque        idx: 1 2 3
    SET,   //set                    idx: 4 5
    UNORDERED_SET, //unordered_set  idx: 6
    QUEUE,  //queue                 idx: 7
    STACK,  //stack                 idx: 8
    FORWARD_LIST, //forward_list    idx: 9
} STLType;

template<typename T>
class None{};

template<typename ...Args>
struct SearchStlType
{
    constexpr static int value = Search<Args...>::value;
    constexpr static STLType stlType = value == -1 ? STLType::SINGLE :
                                       value == 0 ? STLType::NONE :
                                       value < 4 ? STLType::VLD : 
                                       value < 6 ? STLType::SET : 
                                       value < 8 ? STLType::UNORDERED_SET : 
                                       value == 8 ? STLType::QUEUE : 
                                       value == 9 ? STLType::STACK : 
                                       STLType::FORWARD_LIST;
};

template<bool IsInStl, typename ...Args>
struct BreakDown;

template<template<typename ...Args> class STL, typename T, typename ...Args>
struct BreakDown<true, STL<T, Args...>>
{
    using type = T;
};

template<bool IsInStl, typename T>
struct BreakDown<IsInStl, T>
{
    using type = T;
};

template<typename STL_T>
struct IsStl
{
    using T                         = typename BreakDown<true, STL_T>::type;
    using NoneType                  = None<T>;
    using ListType                  = typename std::list<T>;
    using VectorType                = typename std::vector<T>;
    using DequeType                 = typename std::deque<T>;
    using SetType                   = typename std::set<T>;
    using MultiSetType              = typename std::multiset<T>;
    using UnorderedSetType          = typename std::unordered_set<T>;
    using UnorderedMultiSetType     = typename std::unordered_multiset<T>;
    using QueueType                 = typename std::queue<T>;
    using StackType                 = typename std::stack<T>;
    using ForwardListType           = typename std::forward_list<T>;
    using EmptyStl                  = TypeList<>;
    using PushNoneType              = typename PushType<NoneType, 
                                               EmptyStl>::type;
    using PushListType              = typename PushType<ListType, 
                                               PushNoneType>::type;
    using PushVectorType            = typename PushType<VectorType, 
                                               PushListType>::type;
    using PushDequeType             = typename PushType<DequeType, 
                                               PushVectorType>::type;
    using PushSetType               = typename PushType<SetType, 
                                               PushDequeType>::type;
    using PushMultiSetType          = typename PushType<MultiSetType, 
                                               PushSetType>::type;
    using PushUnorderedSetType      = typename PushType<UnorderedSetType, 
                                               PushMultiSetType>::type;
    using PushUnorderedMultiSetType = typename PushType<UnorderedMultiSetType, 
                                               PushUnorderedSetType>::type;
    using PushQueueType             = typename PushType<QueueType, 
                                               PushUnorderedMultiSetType>::type;
    using PushStackType             = typename PushType<StackType, 
                                               PushQueueType>::type;
    using PushForwardListType       = typename PushType<ForwardListType, 
                                               PushStackType>::type;
    using STLList                   = PushForwardListType;
    constexpr static bool status    = Search<STL_T, STLList>::status;
    using FinalT                    = typename BreakDown<status, STL_T>::type;
};

template<bool IsStl, typename ...Args>
struct ReBind;

template<template<typename ...Args> class STL, typename T, 
         typename ...Args, typename ReplaceType>
struct ReBind<true, STL<T, Args...>, ReplaceType>
{
    using type = STL<ReplaceType>;
};

template<typename T1, typename T2>
struct ReBind<true, None<T1>, T2>
{
    using type = std::list<T2>;
};

template<typename T1, typename T2>
struct ReBind<false, T1, T2>
{
    using type = std::list<T2>;
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

template<typename STL_T, typename T, STLType t>
struct STLOperation;

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::NONE>
{
    static void push(STL_T &data, T &t){}

    static void searchDeps(const STL_T &deps, const std::set<std::string> &set)
    {
        for (auto &d : deps)
        {
            if (set.find(d) == set.end())
            {
                CmdLineError err;
                err << d;
                throw err;
            }
        }

        return;
    }

    static int getSize(STL_T &data)
    {
        return 0;
    }

    static T getMin(STL_T &range)
    {
        return T();
    }

    static T getMax(STL_T &range)
    {
        return T();
    }
};

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::VLD> :
       public STLOperation<STL_T, T, STLType::NONE>
{
    static int getSize(STL_T &data)
    {
        return data.size();
    }

    static void push(STL_T &data, T &t)
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

    static T getMin(STL_T &range)
    {
        return *range.begin();
    }

    static T getMax(STL_T &range)
    {
        return *(--range.end());
    }
};

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::SINGLE> : 
       public STLOperation<STL_T, T, STLType::VLD>
{
    static void push(STL_T &data, T &t)
    {
        data = t;
    }
};

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::SET> : 
       public STLOperation<STL_T, T, STLType::VLD>
{
    static void push(STL_T &data, T &t)
    {
        data.insert(t);
    }
};

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::QUEUE> : 
       public STLOperation<STL_T, T, STLType::VLD>
{
    static void push(STL_T &data, T &t)
    {
        data.push(t);
    }

    static bool traverse(STL_T &range, T &value)
    {
        STL_T tmp = range;
        int size = tmp.size();
        for (int i = 0; i < size; i++)
        {
            if (value == tmp.front())
                return true;

            tmp.pop();
        }

        return false;
    }

    static void searchDeps(const STL_T &deps, const std::set<std::string> &set)
    {
        STL_T tmp = deps;
        int size = tmp.size();
        for (int i = 0; i < size; i++)
        {
            if (set.find(tmp.front()) == set.end())
            {
                CmdLineError err;
                err << tmp.front();
                throw err;
            }

            tmp.pop();
        }

        return;
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

    static T getMin(STL_T &range)
    {
        return range.front();
    }

    static T getMax(STL_T &range)
    {
        STL_T tmp = range;
        tmp.pop();
        return tmp.front();
    }
};

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::STACK> :
       public STLOperation<STL_T, T, STLType::QUEUE>
{
    static bool traverse(STL_T &range, T &value)
    {
        STL_T tmp = range;
        int size = tmp.size();
        for (int i = 0; i < size; i++)
        {
            if (value == tmp.top())
                return true;

            tmp.pop();
        }

        return false;
    }

    static void searchDeps(const STL_T &deps, const std::set<std::string> &set)
    {
        STL_T tmp = deps;
        int size = tmp.size();
        for (int i = 0; i < size; i++)
        {
            if (set.find(tmp.top()) == set.end())
            {
                CmdLineError err;
                err << tmp.top();
                throw err;
            }

            tmp.pop();
        }

        return;
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

    static int getMin(STL_T &range)
    {
        STL_T tmp = range;
        tmp.pop();
        return tmp.top();
    }

    static int getMax(STL_T &range)
    {
        return range.top();
    }
};

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::FORWARD_LIST> : 
       public STLOperation<STL_T, T, STLType::VLD>
{
    static void push(STL_T &data, T &t)
    {
        data.push_front(t);
    }

    static int getSize(STL_T &data)
    {
        return std::distance(std::begin(data), std::end(data));
    }

    static T getMax(STL_T &range)
    {
        return *(++range.begin());
    }
};

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::UNORDERED_SET> :
       public STLOperation<STL_T, T, STLType::SET>
{
    static T getMax(STL_T &range)
    {
        return *(range.begin());
    }

    static T getMin(STL_T &range)
    {
        return *(++range.begin());
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

template<typename T, typename STL_T_R, typename STLList, bool IsNum>
struct RangeJudge
{
    bool operator()(T &value, STL_T_R &range)
    {
        constexpr STLType stlType = SearchStlType<STL_T_R, STLList>::stlType;
        if (STLOperation<STL_T_R, T, stlType>::getSize(range) > 0)
        {
            if (STLOperation<STL_T_R, T, stlType>::traverse(range, value))
                return true;

            CmdLineError err;
            err << "value \"" << value << "\" is out of range \n    ";
            
            throw err;
            return false;
        }
   
        return true;
    }
};

template<typename T, typename STL_T, typename STLList>
struct RangeJudge<T, STL_T, STLList, true>
{
    bool operator()(T &value, STL_T &range)
    {
        constexpr STLType stlType = SearchStlType<STL_T, STLList>::stlType;
        if (STLOperation<STL_T, T, stlType>::getSize(range) > 0)
        {
            T min = STLOperation<STL_T, T, stlType>::getMin(range);
            T max = STLOperation<STL_T, T, stlType>::getMax(range);
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

template<class STL_T_R, class T, typename STLList, bool IsNum>
struct RangeToStr
{
    std::string operator()(STL_T_R &range)
    {
        constexpr STLType stlType = SearchStlType<STL_T_R, STLList>::stlType;
        if (!STLOperation<STL_T_R, T, stlType>::getSize(range))
            return "";

        std::string str("[");
        str += STLOperation<STL_T_R, T, stlType>::getTraverseStr(range);
        str += "]";
        return str;
    }
};

template<class STL_T, class T, typename STLList>
struct RangeToStr<STL_T, T, STLList, true>
{
    std::string operator()(STL_T &range)
    {
        constexpr STLType stlType = SearchStlType<STL_T, STLList>::stlType;
        if (!STLOperation<STL_T, T, stlType>::getSize(range))
            return "";

        int min = STLOperation<STL_T, T, stlType>::getMin(range);
        int max = STLOperation<STL_T, T, stlType>::getMax(range);
        return std::string("[") + std::to_string(min) + " , " 
                                + std::to_string(max) + "]";

    }
};

class ParamBase
{
public:
    ParamBase(const std::string &name_,
              const std::string &shortName_,
              const std::string &describtion_);
    virtual ~ParamBase() = default;

    virtual bool set(const std::string &) = 0;
    virtual void searchDeps(std::set<std::string> &) = 0;
    virtual std::string getRangeStr() = 0;

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

template<class STL_T, 
         class STL_STR = typename ReBind<IsStl<STL_T>::status,
                                         STL_T, std::string>::type,
         class STL_T_R = typename ReBind<IsStl<STL_T>::status, STL_T, 
                                         typename IsStl<STL_T>::FinalT>::type>
class ParamWithValue final : public ParamBase
{
    using T                          = typename IsStl<STL_T>::T;
    using FinalT                     = typename IsStl<STL_T>::FinalT;
    using STLList                    = typename IsStl<STL_T>::STLList;
    constexpr static bool isNum      = Search<T, NumTypeList>::status;
    constexpr static STLType stlType = SearchStlType<STL_T, STLList>::stlType;

public:
    ParamWithValue(const std::string &name_,
                   const std::string &shortName_,
                   const std::string &describtion_,
                   const STL_STR &dep_,
                   const STL_T_R &range_) :
                   range(range_),
                   deps(dep_),
                   singleParamStatus(true),
                   ParamBase(name_, shortName_, describtion_){}
    ~ParamWithValue() = default;

    STL_T &get()
    {
        return data;
    }

protected:
    bool set(const std::string &value) override
    {
        FinalT ret = Reader<FinalT, isNum>()(value);
        if (!RangeJudge<FinalT, STL_T_R, STLList, isNum>()(ret, range))
            return false;
        
        if (singleParamStatus)
        {
            STLOperation<STL_T, FinalT, stlType>::push(data, ret);
            singleParamStatus = stlType == STLType::SINGLE ? false : true;
        }
        else
        {
            CmdLineError err;
            err << "option " << getName()
                << " error : Only one parameter can be input";
            throw err;
        }
        return true;
    }

    std::string getRangeStr() override
    {
       return RangeToStr<STL_T_R, FinalT, STLList, isNum>()(range);
    }

    void searchDeps(std::set<std::string> &set) override
    {
        return STLOperation<STL_STR, FinalT, stlType>::searchDeps(deps, set);
    }

private:
    bool singleParamStatus;
    STL_T data;
    STL_T_R range;
    STL_STR deps;
};

class CmdLine
{
public:
    CmdLine() = default;
    ~CmdLine() = default;
    
    /**
     * @brief add 设置命令行选项以及附带参数
     * @param shortName 选项短名称
     * @param name 选项长名称
     * @param describtion 选项描述
     * @param dep 可选参数,选项依赖
     * @param range 可选参数,参数范围
     * @return 返回无
     */
    template<class STL_T = None<int>, 
             class STL_STR = typename ReBind<IsStl<STL_T>::status,
                                             STL_T, std::string>::type,
             class STL_T_R = typename ReBind<IsStl<STL_T>::status, STL_T, 
                                      typename IsStl<STL_T>::FinalT>::type>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, STL_STR dep = STL_STR(), 
             STL_T_R range = STL_T_R())
    {
        paramTable.emplace_back(std::make_shared<ParamWithValue<STL_T>>(name, 
                                                 shortName, describtion, dep, 
                                                 range));
    }

    /**
     * @brief get 获取选项信息，以及参数信息,注意参数只能获取一次
     * @param name 选项名称
     * @param t  参数
     * @return 返回 选项使能 true 否则 false
     */
    template<class STL_T>
    bool get(const std::string &name, STL_T &t)
    {
        for (auto &l : paramTable)
        {
            if ((l->getName() == name || l->getShortName() == name) &&
                 l->getEnable())
            {
                auto p = std::dynamic_pointer_cast<ParamWithValue<STL_T>>(l);
                t = std::move(p->get());
                return true;
            }
        }
        return false;
    }

    /**
     * @brief get 重载函数 获取选项信息
     * @param name 选项名称
     * @return 返回 选项使能 true 否则 false
     */
    bool get(const std::string &name)
    {
        None<int> n;
        return get(name, n);
    }

    /**
     * @brief parse 解析命令行参数
     * @param noParam 没有命令行参数，是否继续运行，true ：是 false: 否
     * @param argc 命令行参数数量
     * @param argv 命令行参数列表
     * @return 返回 选项使能 true 否则 false
     */
    void parse(bool noParam, int argc, char *argv[]);

private:
    void showHelp();
    void paramCheck();

private:
    std::list<std::shared_ptr<ParamBase>> paramTable;
    std::string cmd;
};
};

#endif