/*
 * @Description: 单例模块
 * @Author: luomincheng
 * @Date: 2023-04-16 15:28:55 
 * @Last Modified time: 2023-04-16 15:38:29
 */

#ifndef SINGLE_H_
#define SINGLE_HPP_

#include <mutex>
#include <unistd.h>

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
        if (!sEnable)
            return nullptr;

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

    static void destory()
    {
        sEnable = false;
        sleep(2);

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
    static std::mutex sMutex;
    static T *instance;
    static bool sEnable;
};

template<typename T>
std::mutex TypeSingle<T>::sMutex;

template<typename T>
T *TypeSingle<T>::instance = nullptr;

template<typename T>
bool TypeSingle<T>::sEnable = true;
};
#endif
