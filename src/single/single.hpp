#ifndef SINGLE_H_
#define SINGLE_H_

#include <mutex>

namespace lmc
{
template<typename T>
class TypeSingle
{
public:
    template<typename ...Args>
    inline static T *getInstance(Args &&...args)
    {
        if (nullptr == instance)
        {
            sMutex.lock();
            if (nullptr == instance)
            {
                instance = new T(args...);
            }
            sMutex.unlock();
        }

        return instance;
    }

    inline static T *getInstance()
    {
        if (nullptr == instance)
        {
            sMutex.lock();
            if (nullptr == instance)
            {
                instance = new T();
            }
            sMutex.unlock();
        }

        return instance;
    }

    static bool destory()
    {
        sMutex.lock();
        if (nullptr != instance)
        {
            delete instance;
            instance = nullptr;
        }
        sMutex.unlock();
    }

private:
    TypeSingle() = default;

private:
    static mutex sMutex;
    static T *instance;
};

template<typename T>
std::mutex TypeSingle<T>::sMutex;

template<typename T>
T *TypeSingle<T>::instance = nullptr;

};
#endif
