/*
 * @Description: 单例模块
 * @Author: luomincheng
 * @Date: 2023-04-16 15:28:55 
 * @Last Modified time: 2023-04-16 15:38:29
 */

#ifndef SINGLE_HPP_
#define SINGLE_HPP_

#include <mutex>
#include <unistd.h>
#include <string.h>
#include "type.hpp"
namespace lmc {
template<typename T>
class TypeSingle {
public:
    template<typename ...Args>
    inline static T *getInstance(Args &&...args) {
         constexpr bool isPointerRefFT = std::is_pointer<T>::value ||
                                         std::is_lvalue_reference<T>::value ||
                                         std::is_rvalue_reference<T>::value ||
                                         lmc::Search<T, NumTypeList>::status;
        static_assert(!isPointerRefFT,
                      "\033[93mReferences, pointers and fundamental type "
                      "\n are not allowed as parameters\n  \033[0m");

        if (nullptr == instance) {
            sMutex.lock();
            if (nullptr == instance) {
                instance = new (std::nothrow) T(args...);
                if (!instance) {
                    std::cout << "create " << typeid(T).name()
                              << " error, reason is " << strerror(errno)
                              << ", exit now... " << std::endl;
                    exit(0);
                } 
            }
            sMutex.unlock();
        }

        return instance;
    }

    inline static void destory() {
        if (instance) {
            sMutex.lock();
            if (instance) {
                T *tmpInstance = instance;
                instance = nullptr;
                delete tmpInstance;
            }
            sMutex.unlock();
        }
    }

    TypeSingle() = delete;
    TypeSingle(const TypeSingle &) = delete;
    TypeSingle(TypeSingle &&) = delete;
    TypeSingle &operator=(TypeSingle &&) = delete;
    ~TypeSingle() = delete;

private:
    static std::mutex sMutex;
    static T *instance;
};

template<typename T>
std::mutex TypeSingle<T>::sMutex;

template<typename T>
T *TypeSingle<T>::instance = nullptr;
};

#endif