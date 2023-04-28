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

#include "util/type.hpp"

namespace lmc
{
typedef enum class STLType_ : unsigned char
{
    SINGLE, //no stl                                  idx: -1
    NONE, //none                                      idx: 0
    VLD, //vector list deque                          idx: 1 2 3
    SET,   //set  multiset                            idx: 4 5
    UNORDERED_SET, //unordered_set unordered_multiset idx: 6 7
    QUEUE,  //queue                                   idx: 8
    STACK,  //stack                                   idx: 9
    FORWARD_LIST, //forward_list                      idx: 10
} STLType;

template<typename T>
class None{};

//搜索STL容器列表索引
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

//容器分解，去除内部类型
template<bool, typename ...>
struct BreakDown;

template<template<typename ...Args> typename STL, typename T, typename ...Args>
struct BreakDown<true, STL<T, Args...>>
{
    using type = T;
};

template<bool IsInStl, typename T>
struct BreakDown<IsInStl, T>
{
    using type = T;
};

//判断是否为容器
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

    //之所以设置FinalT该类型，是因为考虑std::string这种情况，
    //因为std::string也是容器类型, std::string = basic_string<char>
    //所以要过滤这种情况
    using FinalT                    = typename BreakDown<status, STL_T>::type;
};

//容器重新绑定
template<bool, typename ...>
struct ReBind;

template<template<typename ...Args> typename STL, typename T,
         typename ...Args, typename ReplaceType>
struct ReBind<true, STL<T, Args...>, ReplaceType>
{
    using type = STL<ReplaceType>;
};

//如果传入类型参数不是容器，则使用默认容器list
template<typename T1, typename T2>
struct ReBind<true, None<T1>, T2>
{
    using type = std::list<T2>;
};

//如果传入类型参数不是容器，则使用默认容器list
template<typename T1, typename T2>
struct ReBind<false, T1, T2>
{
    using type = std::list<T2>;
};

//将新类型绑定到STL_T的容器内部
template<typename STL_T, typename NEW_T>
using STL_NEW_T = typename ReBind<IsStl<STL_T>::status, STL_T, NEW_T>::type;

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

//容器接口操作
template<typename, typename, STLType>
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

//读入字符串，转成指定类型
template<typename Target, bool>
struct Reader
{
    Target operator()(const std::string &str)
    {
        return str;
    }
};

template<typename Target>
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

//范围判断
template<typename T, typename STL_T_R, typename STLList, bool>
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
            err << "    value \"" << value << "\" is out of range \n    "
                << "param range is [ "
                << STLOperation<STL_T_R, T, stlType>::getTraverseStr(range)
                << "]";
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
                err << "    value range is " << min << " to "
                    << max << " , " << value << " is out of range";
                throw err;
                return false;
            }
        }

        return true;
    }
};

//STL容器数据转换成字符串
template<typename STL_T_R, typename T, typename STLList, bool>
struct STLDataToStr
{
    std::string operator()(STL_T_R &range)
    {
        constexpr STLType stlType = SearchStlType<STL_T_R, STLList>::stlType;
        if (!STLOperation<STL_T_R, T, stlType>::getSize(range))
            return "";

        std::string str("[ ");
        str += STLOperation<STL_T_R, T, stlType>::getTraverseStr(range);
        str += "]";
        return str;
    }
};

template<typename STL_T, typename T, typename STLList>
struct STLDataToStr<STL_T, T, STLList, true>
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
    virtual std::string getDepsStr() = 0;

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

template<//STL_T是用户定义的数据类型，有可能是容器，但也有可能不是
         typename STL_T,
         //STL_STR是选项依赖的数据类型。它是个容器，内部类型使用std::string
         //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
         typename STL_STR = STL_NEW_T<STL_T, std::string>,
         //STL_R是参数范围的数据类型。它是个容器,内部类型与用户容器内部类型保持一致
         //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
         typename STL_T_R = STL_NEW_T<STL_T, typename IsStl<STL_T>::FinalT>>
class ParamWithValue final : public ParamBase
{
    using FinalT                     = typename IsStl<STL_T>::FinalT;
    using STLList                    = typename IsStl<STL_T>::STLList;
    constexpr static bool isNum      = Search<FinalT, NumTypeList>::status;
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
            err << "option " << getShortName() << " " << getName()
                << " error : Only one parameter can be input";
            throw err;
        }
        return true;
    }

    std::string getRangeStr() override
    {
        return STLDataToStr<STL_T_R, FinalT, STLList, isNum>()(range);
    }

    void searchDeps(std::set<std::string> &set) override
    {
        return STLOperation<STL_STR, FinalT, stlType>::searchDeps(deps, set);
    }

    std::string getDepsStr() override
    {
        using DepSTLList = typename IsStl<STL_STR>::STLList;
        return STLDataToStr<STL_STR, std::string, DepSTLList, false>()(deps);
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
    template<//STL_T是用户定义的数据类型，有可能是容器，但也有可能不是
             typename STL_T = None<int>,
             //STL_STR是选项依赖的数据类型。它是个容器，内部类型使用std::string
             //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
             typename STL_STR = STL_NEW_T<STL_T, std::string>,
             //STL_R是参数范围的数据类型。它是个容器,内部类型与用户容器内部类型保持一致
             //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
             typename STL_R = STL_NEW_T<STL_T, typename IsStl<STL_T>::FinalT>>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, STL_STR dep = STL_STR(), 
             STL_R range = STL_R())
    {
        paramTable.emplace_back(std::make_shared<ParamWithValue<STL_T>>(name, 
                                                 shortName, describtion, dep, 
                                                 range));
    }

    /**
     * @brief get 获取选项信息，或者获取参数信息,注意参数只能获取一次
     * @param name 选项名称
     * @param t  获取参数信息
     * @return 返回 选项使能 true 否则 false
     */
    template<typename STL_T = None<int>>
    bool get(const std::string &name, STL_T &&t = STL_T())
    {
        for (auto &l : paramTable)
        {
            if ((l->getName() == name || l->getShortName() == name) &&
                 l->getEnable())
            {
                //移除STL_T多余的类型修饰信息，const volatile等等。
                using STL_T_ = std::__remove_cvref_t<STL_T>;
                auto p = std::dynamic_pointer_cast<ParamWithValue<STL_T_>>(l);
                t = std::move(p->get());
                return true;
            }
        }
        return false;
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