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
#include <functional>
#include <set>
#include <vector>
#include <deque>
#if __GNUC__ > 6
#include <queue>
#include <stack>
#include <forward_list>
#include <unordered_set>
#endif
#include "util/type.hpp"

namespace lmc {
// 普通函数参数萃取
template <typename Func>
struct function_traits;

// 普通函数特化
template <typename Ret, typename Arg, typename ...Args>
struct function_traits<Ret(Arg, Args...)> {
    static_assert(!(sizeof...(Args) > 0), "\033[93mThe callback parameter must"
                                          " be limited to one\n \033[0m");
    using ArgType = Arg;
};

template <typename Ret>
struct function_traits<Ret()> {
    using ArgType = void;
};

// 成员函数指针特化
template <typename C, typename Ret, typename ...Args>
struct function_traits<Ret(C::*)(Args...)> : function_traits<Ret(Args...)> {};

// 常量成员函数指针特化
template <typename C, typename Ret, typename ...Args>
struct function_traits<Ret(C::*)(Args...) const> : function_traits<Ret(Args...)> {};

// 基础函数特征萃取模板
template <typename F>
struct function_traits : public function_traits<decltype(&F::operator())> {};

typedef enum class STLType_ : unsigned char {
    SINGLE, //no stl                                  idx: -1
    NONE, //none                                      idx: 0
    VLD, //vector list deque                          idx: 1 2 3
    SET,   //set  multiset                            idx: 4 5
#if __GNUC__ > 6
    UNORDERED_SET, //unordered_set unordered_multiset idx: 6 7
    QUEUE,  //queue                                   idx: 8
    STACK,  //stack                                   idx: 9
    FORWARD_LIST, //forward_list                      idx: 10
#endif
} STLType;

//搜索STL容器列表索引
template <typename ...Args>
struct SearchStlType {
    constexpr static int value = Search<Args...>::value;
    constexpr static STLType stlType = value == -1 ? STLType::SINGLE :
                                       value == 0 ? STLType::NONE :
                                       value < 4 ? STLType::VLD : 
                                       value < 6 ? STLType::SET : 
#if __GNUC__ > 6
                                       value < 8 ? STLType::UNORDERED_SET : 
                                       value == 8 ? STLType::QUEUE : 
                                       value == 9 ? STLType::STACK : 
                                       STLType::FORWARD_LIST;
#else
                                       STLType::SET;
#endif
};

//容器分解，去除内部类型
template <bool, typename ...>
struct BreakDown;

template <template <class ...Args> class STL, class T, class ...Args>
struct BreakDown<true, STL<T, Args...>> {
    using type = T;
};

template <bool IsInStl, class T>
struct BreakDown<IsInStl, T> {
    using type = T;
};

//判断是否为容器
template <typename STL_T>
struct IsStl {
    using T                         = typename BreakDown<true, STL_T>::type;
    //using VoidType                  = void;
    using ListType                  = typename std::list<T>;
    using VectorType                = typename std::vector<T>;
    using DequeType                 = typename std::deque<T>;
    using SetType                   = typename std::set<T>;
    using MultiSetType              = typename std::multiset<T>;
#if __GNUC__ > 6
    using UnorderedSetType          = typename std::unordered_set<T>;
    using UnorderedMultiSetType     = typename std::unordered_multiset<T>;
    using QueueType                 = typename std::queue<T>;
    using StackType                 = typename std::stack<T>;
    using ForwardListType           = typename std::forward_list<T>;
#endif
    using EmptyStl                  = TypeList<>;
    using PushNoneType              = typename PushType<EmptyStl, 
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
#if __GNUC__ > 6
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
#else
    using STLList                   = PushMultiSetType;
#endif
    constexpr static bool status    = Search<STL_T, STLList>::status;

    //之所以设置FinalT该类型，是因为考虑std::string这种情况，
    //因为std::string也是容器类型, std::string = basic_string<char>
    //所以要过滤这种情况
    using FinalT                    = typename BreakDown<status, STL_T>::type;
};

//容器重新绑定
template <bool, typename ...>
struct ReBind;

template <template<class ...Args> class STL, class T,
         class ...Args, class ReplaceType>
struct ReBind<true, STL<T, Args...>, ReplaceType> {
    using type = STL<ReplaceType>;
};

//如果传入类型参数不是容器，则使用默认容器list
template <typename T2>
struct ReBind<true, void, T2> {
    using type = std::list<T2>;
};

//如果传入类型参数不是容器，则使用默认容器list
template <typename T1, typename T2>
struct ReBind<false, T1, T2> {
    using type = std::list<T2>;
};

template <typename STL_T, typename NEW_T>
struct STL_NEW_T {
    using type = typename ReBind<IsStl<STL_T>::status, STL_T, NEW_T>::type;
};

template <typename STL_T>
struct STL_NEW_T<STL_T, void> {
    using type = int;
};

class CmdLineError : public std::exception {
public:
    CmdLineError(const std::string &cmdMsg = ""): msg(cmdMsg){}
    ~CmdLineError() throw() {}
    const char *what() const throw() { return msg.c_str(); }
    CmdLineError &operator << (std::string str) {
        msg += str;
        return *this;
    }

    CmdLineError &operator << (int num) {
        msg += std::to_string(num);
        return *this;
    }

private:
    std::string msg;
};

//容器接口操作
template <typename, typename, STLType>
struct STLOperation;

template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::NONE> {
    static void push(STL_T &data, T &t) {
        (void)data;
        (void)t;
    }

    static void searchDeps(const STL_T &deps, const std::set<std::string> &set) {
        for (auto &d : deps) {
            if (set.find(d) == set.end()) {
                CmdLineError err;
                err << d;
                throw err;
            }
        }

        return;
    }

    static int getSize(STL_T &data) {
        return 0;
    }

    static T getMin(STL_T &range) {
        return T();
    }

    static T getMax(STL_T &range) {
        return T();
    }
};

template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::VLD> :
       public STLOperation<STL_T, T, STLType::NONE> {
    static size_t getSize(STL_T &data) {
        return data.size();
    }

    static void push(STL_T &data, T &t) {
        data.push_back(t);
    }

    static bool traverse(const STL_T &range, const T &value) {
        for (auto &r : range) {
            if (value == r)
                return true;
        }

        return false;
    }

    static std::string getTraverseStr(const STL_T &range) {
        std::string str;
        for (auto &r : range) {
            str += r;
            str += ' ';
        }

        return str;
    }

    static T getMin(STL_T &range) {
        return *range.begin();
    }

    static T getMax(STL_T &range) {
        return *(--range.end());
    }
};

template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::SINGLE> : 
       public STLOperation<STL_T, T, STLType::VLD> {
    static void push(STL_T &data, T &t) {
        data = t;
    }
};

template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::SET> : 
       public STLOperation<STL_T, T, STLType::VLD> {
    static void push(STL_T &data, T &t) {
        data.insert(t);
    }
};
#if __GNUC__ > 6
template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::QUEUE> : 
       public STLOperation<STL_T, T, STLType::VLD> {
    static void push(STL_T &data, T &t) {
        data.push(t);
    }

    static bool traverse(STL_T &range, T &value) {
        STL_T tmp = range;
        size_t size = tmp.size();
        for (size_t i = 0; i < size; i++) {
            if (value == tmp.front())
                return true;

            tmp.pop();
        }

        return false;
    }

    static void searchDeps(const STL_T &deps, const std::set<std::string> &set) {
        STL_T tmp = deps;
        size_t size = tmp.size();
        for (size_t i = 0; i < size; i++) {
            if (set.find(tmp.front()) == set.end()){
                CmdLineError err;
                err << tmp.front();
                throw err;
            }

            tmp.pop();
        }

        return;
    }

    static std::string getTraverseStr(STL_T &range) {
        std::string str;
        STL_T tmp = range;
        size_t size = tmp.size();
        for (size_t i = 0; i < size; i++) {
            str += tmp.front();
            str += ' ';
            tmp.pop();
        }     

        return str;
    }

    static T getMin(STL_T &range) {
        return range.front();
    }

    static T getMax(STL_T &range) {
        STL_T tmp = range;
        tmp.pop();
        return tmp.front();
    }
};

template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::STACK> :
       public STLOperation<STL_T, T, STLType::QUEUE> {
    static bool traverse(STL_T &range, T &value) {
        STL_T tmp = range;
        size_t size = tmp.size();
        for (size_t i = 0; i < size; i++) {
            if (value == tmp.top())
                return true;

            tmp.pop();
        }

        return false;
    }

    static void searchDeps(const STL_T &deps, const std::set<std::string> &set) {
        STL_T tmp = deps;
        size_t size = tmp.size();
        for (size_t i = 0; i < size; i++) {
            if (set.find(tmp.top()) == set.end()) {
                CmdLineError err;
                err << tmp.top();
                throw err;
            }

            tmp.pop();
        }

        return;
    }

    static std::string getTraverseStr(STL_T &range) {
        std::string str;
        STL_T tmp = range;
        size_t size = tmp.size();
        for (size_t i = 0; i < size; i++) {
            str += tmp.top();
            str += ' ';
            tmp.pop();
        }     

        return str;
    }

    static int getMin(STL_T &range) {
        STL_T tmp = range;
        tmp.pop();
        return tmp.top();
    }

    static int getMax(STL_T &range) {
        return range.top();
    }
};

template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::FORWARD_LIST> : 
       public STLOperation<STL_T, T, STLType::VLD> {
    static void push(STL_T &data, T &t) {
        data.push_front(t);
    }

    static int getSize(STL_T &data) {
        return static_cast<int>(std::distance(std::begin(data), std::end(data)));
    }

    static T getMax(STL_T &range) {
        return *(++range.begin());
    }
};

template <typename STL_T, typename T>
struct STLOperation<STL_T, T, STLType::UNORDERED_SET> :
       public STLOperation<STL_T, T, STLType::SET> {
    static T getMax(STL_T &range) {
        return *(range.begin());
    }

    static T getMin(STL_T &range) {
        return *(++range.begin());
    }
};
#endif
//读入字符串，转成指定类型
template <typename Target, bool>
struct Reader {
    Target operator()(const std::string &str) {
        return str;
    }
};

template <typename Target>
struct Reader<Target, true> {
    Target operator()(const std::string &str) {
        Target ret;
        std::stringstream ss;
        if (!(ss << str && ss >> ret && ss.eof())) {
            CmdLineError err;
            err << "param error \"" << str << "\"";
            throw err;
        }
       
        return ret;
    }
};

//范围判断
template <typename T, typename STL_T_R, typename STLList, bool>
struct RangeJudge {
    bool operator()(T &value, STL_T_R &range) {
        constexpr STLType stlType = SearchStlType<STL_T_R, STLList>::stlType;
        if (STLOperation<STL_T_R, T, stlType>::getSize(range) > 0) {
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

template <typename T, typename STL_T, typename STLList>
struct RangeJudge<T, STL_T, STLList, true> {
    bool operator()(T &value, STL_T &range) {
        constexpr STLType stlType = SearchStlType<STL_T, STLList>::stlType;
        if (STLOperation<STL_T, T, stlType>::getSize(range) > 0) {
            T min = STLOperation<STL_T, T, stlType>::getMin(range);
            T max = STLOperation<STL_T, T, stlType>::getMax(range);
            if (min > value || max < value) {
                CmdLineError err;
                err << "    value range is " << static_cast<int>(min) 
                    << " to " << static_cast<int>(max) << " , " 
                    << static_cast<int>(value) << " is out of range";
                throw err;
                return false;
            }
        }

        return true;
    }
};

//STL容器数据转换成字符串
template <typename STL_T_R, typename T, typename STLList, bool>
struct STLDataToStr {
    std::string operator()(STL_T_R &range) {
        constexpr STLType stlType = SearchStlType<STL_T_R, STLList>::stlType;
        if (!STLOperation<STL_T_R, T, stlType>::getSize(range))
            return "";

        std::string str("[ ");
        str += STLOperation<STL_T_R, T, stlType>::getTraverseStr(range);
        str += "]";
        return str;
    }
};

template <typename STL_T, typename T, typename STLList>
struct STLDataToStr<STL_T, T, STLList, true> {
    std::string operator()(STL_T &range) {
        constexpr STLType stlType = SearchStlType<STL_T, STLList>::stlType;
        if (!STLOperation<STL_T, T, stlType>::getSize(range))
            return "";

        int min = static_cast<int>(STLOperation<STL_T, T, stlType>::getMin(range));
        int max = static_cast<int>(STLOperation<STL_T, T, stlType>::getMax(range));
        return std::string("[") + std::to_string(min) + " , " 
                                + std::to_string(max) + "]";

    }
};

class ParamBase {
public:
    ParamBase(const std::string &name_,
              const std::string &shortName_,
              const std::string &describtion_);
    virtual ~ParamBase() = default;

    //用在在控制台输入的参数，通过该接口输入到选项内部
    virtual bool set(const std::string &) = 0;
    //判断当前选项的依赖项，是否已经使能开启
    virtual void searchDeps(std::set<std::string> &) = 0;
    //将当前选项的参数范围转化为字符串
    virtual std::string getRangeStr() = 0;
    //将当前选项所依赖的选项列表转化为字符串
    virtual std::string getDepsStr() = 0;

    //调用参数回调函数
    virtual void callBackFunc() = 0;

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

template <typename Func,
          //STL_T是用户定义的数据类型，有可能是容器，但也有可能不是
          typename STL_T = RemoveCVREF<typename function_traits<Func>::ArgType>,
          //STL_STR是选项依赖的数据类型。它是个容器，内部类型使用std::string
          //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
          typename STL_STR = typename STL_NEW_T<STL_T, std::string>::type,
          //STL_R是参数范围的数据类型。它是个容器,内部类型与用户容器内部类型保持一致
          //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
          typename STL_T_R = typename STL_NEW_T<STL_T, typename IsStl<STL_T>::FinalT>::type>
class ParamWithValue final : public ParamBase {
    using FinalT                     = typename IsStl<STL_T>::FinalT;
    using STLList                    = typename IsStl<STL_T>::STLList;
    constexpr static bool isNum      = Search<FinalT, NumTypeList>::status;
    constexpr static STLType stlType = SearchStlType<STL_T, STLList>::stlType;
public:
    ParamWithValue(const std::string &name_,
                   const std::string &shortName_,
                   const std::string &describtion_,
                   const Func &func_,
                   const STL_STR &dep_,
                   const STL_T_R &range_) :
                   ParamBase(name_, shortName_, describtion_),
                   singleParamStatus(true),
                   func(func_),
                   range(range_),
                   deps(dep_) {}
    ~ParamWithValue() = default;

protected:
    bool set(const std::string &value) override {
        FinalT ret = Reader<FinalT, isNum>()(value);

        //判断参数是否在当前选项的指定参数范围内
        if (!RangeJudge<FinalT, STL_T_R, STLList, isNum>()(ret, range))
            return false;
        
        //有的选项只需要一个参数即可，用户在控制台输入多于一个的参数将出错。
        if (singleParamStatus) {
            //将控制台的参数输入到当前选项里。
            STLOperation<STL_T, FinalT, stlType>::push(data, ret);
            //判断当前选项是单参数选项还是多参数选项。
            singleParamStatus = stlType == STLType::SINGLE ? false : true;
        } else {
            CmdLineError err;
            err << "option " << getShortName() << " " << getName()
                << " error : Only one parameter can be input";
            throw err;
        }
        return true;
    }

    std::string getRangeStr() override {
        return STLDataToStr<STL_T_R, FinalT, STLList, isNum>()(range);
    }

    void searchDeps(std::set<std::string> &set) override {
        return STLOperation<STL_STR, FinalT, stlType>::searchDeps(deps, set);
    }

    std::string getDepsStr() override {
        using DepSTLList = typename IsStl<STL_STR>::STLList;
        return STLDataToStr<STL_STR, std::string, DepSTLList, false>()(deps);
    }

    void callBackFunc() override {
        func(data);
    }
private:
    bool singleParamStatus;
    std::function<void(typename function_traits<Func>::ArgType)> func;
    STL_T_R range;
    STL_STR deps;
    STL_T data;
};

class ParamWithOutValue final : public ParamBase {
public:
ParamWithOutValue(const std::string &name_,
                  const std::string &shortName_,
                  const std::string &describtion_,
                  const std::function<void()> &func_,
                  const std::list<std::string> &dep_) :
                  ParamBase(name_, shortName_, describtion_),
                  func(func_),
                  deps(dep_) {}
    ~ParamWithOutValue() = default;

protected:
    bool set(const std::string &value) override {
        (void)value; //忽略value参数
        return true;
    }

    std::string getRangeStr() override {
        return "";
    }

    void searchDeps(std::set<std::string> &set) override {
        return STLOperation<std::list<std::string>, std::string, STLType::VLD>::searchDeps(deps, set);
    }

    std::string getDepsStr() override {
        using DepSTLList = typename IsStl<int>::STLList;
        return STLDataToStr<std::list<std::string>, std::string, DepSTLList, false>()(deps);
    }

    void callBackFunc() override {
        func();
    }
private:
    std::function<void(void)> func;
    std::list<std::string> deps;
};

template <bool, typename Func, typename STL_STR, typename STL_R>
struct PushParam {
    static void push(std::list<std::shared_ptr<ParamBase>> &paramTable,
                     const std::string &shortName, 
                     const std::string &name,
                     const std::string &describtion, 
                     const Func &func,
                     const STL_STR &dep, 
                     const STL_R &range) {
        paramTable.emplace_back(std::make_shared<ParamWithValue<Func>>(shortName, 
                                                                       name, 
                                                                       describtion, 
                                                                       func, 
                                                                       dep, 
                                                                       range ));
    }
};

template <typename Func, typename STL_STR, typename STL_R>
struct PushParam<true, Func, STL_STR, STL_R> {
    static void push(std::list<std::shared_ptr<ParamBase>> &paramTable,
                     const std::string &shortName, 
                     const std::string &name,
                     const std::string &describtion, 
                     const Func &func,
                     STL_STR dep, STL_R range) {
        (void)range; //忽略range参数
        paramTable.emplace_back(std::make_shared<ParamWithOutValue>(shortName, 
                                                                    name, 
                                                                    describtion, 
                                                                    func, 
                                                                    dep));
    }
};

class CmdLine final {
public:
    CmdLine() = default;
    ~CmdLine();

    CmdLine(const CmdLine &) = delete;
    CmdLine(CmdLine &&) = delete;
    CmdLine &operator=(CmdLine &&) = delete;
    
    /**
     * @brief add 设置命令行选项以及附带参数
     * @param shortName 选项短名称
     * @param name 选项长名称
     * @param describtion 选项描述
     * @param dep 可选参数,选项依赖
     * @param range 可选参数,参数范围
     * @return 返回无
     */
    template<typename Func,
             //STL_T是用户定义的数据类型，有可能是容器，但也有可能不是
             typename STL_T = RemoveCVREF<typename function_traits<Func>::ArgType>,
             //STL_STR是选项依赖的数据类型。它是个容器，内部类型使用std::string
             //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
             typename STL_STR = typename STL_NEW_T<STL_T, std::string>::type,
             //STL_R是参数范围的数据类型。它是个容器,内部类型与用户容器内部类型保持一致
             //具体使用哪个容器与STL_T容器保持一致，如果STL_T不是容器则默认使用list
             typename STL_R = typename STL_NEW_T<STL_T, typename IsStl<STL_T>::FinalT>::type>
    void add(const std::string &shortName, const std::string &name,
             const std::string &describtion, const Func &func, 
             STL_STR dep = STL_STR(), STL_R range = STL_R()) {
        PushParam<std::is_same<STL_T, void>::value, 
                  Func, STL_STR, STL_R>::push(paramTable, shortName, name, 
                                              describtion, func, dep, range);
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