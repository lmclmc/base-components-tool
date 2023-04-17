#ifndef TYPE_H_
#define TYPE_H_

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
};

#endif