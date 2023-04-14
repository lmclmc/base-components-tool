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

typedef enum class STLType_ : unsigned char
{
    NONE, //none                    idx:-1
    VLD, //vector list deque        idx:0 1 2
    SET,   //set                    idx: 3
    UNORDERED_SET, //unordered_set  idx: 5
    QUEUE,  //queue                 idx: 6
    STACK,  //stack                 idx: 7
    FORWARD_LIST, //forward_list    idx: 8
} STLType;

template<typename ...Args>
struct Search;

template<typename TargetType, typename ...Args>
struct Search<TargetType, TypeList<TargetType, Args...>>
{
    constexpr static bool status = true;
    constexpr static int value = 0;
    constexpr static STLType typeIdx = STLType::VLD;
};

template<typename TargetType, typename HeadType, typename ...Args>
struct Search<TargetType, TypeList<HeadType, Args...>>
{
    constexpr static bool status = Search<TargetType, TypeList<Args...>>::status;
    constexpr static int tmp = Search<TargetType, TypeList<Args...>>::value;
    constexpr static int value = tmp == -1 ? -1 : tmp + 1;
    constexpr static STLType typeIdx = value < 3 ? STLType::VLD : 
                                       value < 5 ? STLType::SET : 
                                       value < 7 ? STLType::UNORDERED_SET : 
                                       value == 7 ? STLType::QUEUE : 
                                       value == 8 ? STLType::STACK : 
                                       STLType::FORWARD_LIST;
};

template<typename TargetType>
struct Search<TargetType, TypeList<>>
{
    constexpr static bool status = false;
    constexpr static int value = -1;
    constexpr static STLType typeIdx = STLType::NONE;
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

template<typename STL_T, typename T, STLType t>
struct STLOperation;

template<typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::VLD>
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

    static std::string getTraverseStr(STL_T &range)
    {
        std::string str;
        STL_T tmp = range;
        int size = 0;//getSize(tmp);
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

template<typename T, typename STL_T, typename STLList, bool IsNum>
struct RangeJudge
{
    bool operator()(T &value, STL_T &range)
    {
        constexpr STLType typeIdx = Search<STL_T, STLList>::typeIdx;
        if (STLOperation<STL_T, T, typeIdx>::getSize(range) > 0)
        {
            if (STLOperation<STL_T, T, typeIdx>::traverse(range, value))
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
        constexpr STLType typeIdx = Search<STL_T, STLList>::typeIdx;
        if (STLOperation<STL_T, T, typeIdx>::getSize(range) > 0)
        {
            T min = STLOperation<STL_T, T, typeIdx>::getMin(range);
            T max = STLOperation<STL_T, T, typeIdx>::getMax(range);
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

template<class STL_T, class T, typename STLList, bool IsNum>
struct RangeToStr
{
    std::string operator()(STL_T &range)
    {
        constexpr STLType typeIdx = Search<STL_T, STLList>::typeIdx;
        if (!STLOperation<STL_T, T, typeIdx>::getSize(range))
            return "";

        std::string str("[");
        str += STLOperation<STL_T, T, typeIdx>::getTraverseStr(range);
        str += "]";
        return str;
    }
};

template<class STL_T, class T, typename STLList>
struct RangeToStr<STL_T, T, STLList, true>
{
    std::string operator()(STL_T &range)
    {
        constexpr STLType typeIdx = Search<STL_T, STLList>::typeIdx;
        if (!STLOperation<STL_T, T, typeIdx>::getSize(range))
            return "";

        int min = STLOperation<STL_T, T, typeIdx>::getMin(range);
        int max = STLOperation<STL_T, T, typeIdx>::getMax(range);
        return std::string("[") + std::to_string(min) + " , " 
                                + std::to_string(max) + "]";

    }
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

template<class STL_T>
class ParamWithValue final : public ParamBase
{
    using T                         = typename BreakDown<STL_T>::type;
    using ListType                  = typename ReBind<std::list, 
                                               T>::type;
    using VectorType                = typename ReBind<std::vector, 
                                               T>::type;
    using DequeType                 = typename ReBind<std::deque, 
                                               T>::type;
    using SetType                   = typename ReBind<std::set, T
                                               >::type;
    using MultiSetType              = typename ReBind<std::multiset, 
                                               T>::type;
    using UnorderedSetType          = typename ReBind<std::unordered_set, 
                                               T>::type;
    using UnorderedMultiSetType     = typename ReBind<std::unordered_multiset, 
                                               T>::type;
    using QueueType                 = typename ReBind<std::queue, 
                                               T>::type;
    using StackType                 = typename ReBind<std::stack, 
                                               T>::type;
    using ForwardListType           = typename ReBind<std::forward_list,
                                               T>::type;
    using EmptyStl                  = TypeList<>;
    using PushListType              = typename PushType<ListType, 
                                               EmptyStl>::type;
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
    constexpr static bool isNum     = Search<T, NumTypeList>::status;
    constexpr static STLType typeIdx = Search<STL_T, STLList>::typeIdx;
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
        T ret = Reader<T, isNum>()(value);
        if (!RangeJudge<T, STL_T, STLList, isNum>()(ret, range))
            return false;
        
        STLOperation<STL_T, T, typeIdx>::push(data, ret);
        return true;
    }

    bool hasParam() override
    {
        return true;
    }

    std::string getRangeStr() override
    {
       return RangeToStr<STL_T, T, STLList, isNum>()(range);
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
    
    /**
     * @brief add 设置命令行选项以及附带参数
     * @param shortName 选项短名称
     * @param name 选项长名称
     * @param describtion 选项描述
     * @param dep 选项依赖
     * @param range 参数范围
     * @return 返回无
     */
    template<class STL_T>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, 
             std::list<std::string> dep = std::list<std::string>(), 
             STL_T range = STL_T())
    {
        paramTable.emplace_back(std::make_shared<ParamWithValue<STL_T>>(name, 
                                                 shortName,
                                                 describtion, dep, range));
    }

    /**
     * @brief add 重载函数 设置命令行选项
     * @param shortName 选项短名称
     * @param name 选项长名称
     * @param describtion 选项描述
     * @param dep 选项依赖
     * @return 返回无
     */
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion,
             std::list<std::string> dep = std::list<std::string>())
    {
        paramTable.emplace_back(std::make_shared<ParamNone>(name, 
                                                            shortName,
                                                            describtion, dep));
    }

    /**
     * @brief get 获取选项信息，以及参数信息
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
                t = p->get();
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