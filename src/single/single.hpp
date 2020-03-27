#ifndef SINGLE_H_
#define SINGLE_H_

#include <mutex>

using namespace std;

namespace lmc
{

template<typename T>
class TypeSingle
{
public:
    static T *getInstance()
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

private:
    TypeSingle() = default;

private:
    static mutex sMutex;
    static T *instance;
};

template<typename T>
mutex TypeSingle<T>::sMutex;

template<typename T>
T *TypeSingle<T>::instance = nullptr;

};
#endif