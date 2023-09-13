#ifndef TYPE_HPP_
#define TYPE_HPP_

#include <type_traits>

namespace lmc
{
template<typename T>
using RemoveREF = typename std::remove_reference<T>::type;

template<typename T>
using RemoveCVREF = typename std::remove_cv<RemoveREF<T>>::type;

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
#define PUSH_UNSIGNED_L_I   PushType<unsigned long long int, PUSH_DOUBLE>::type 
using NumTypeList = PUSH_UNSIGNED_L_I;

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
    constexpr static bool status = Search<TargetType, 
                                          TypeList<Args...>>::status;
    constexpr static int tmp = Search<TargetType, TypeList<Args...>>::value;
    constexpr static int value = tmp == -1 ? -1 : tmp + 1;
};

template<typename TargetType>
struct Search<TargetType, TypeList<>>
{
    constexpr static bool status = false;
    constexpr static int value = -1;
};
};

#endif