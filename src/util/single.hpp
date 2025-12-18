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
#include <atomic>
#include <cxxabi.h>
#include "type.hpp"
namespace lmc {


// 获取可读的类型名
template<typename T>
std::string getTypeName() {
    int status = 0;
    char* demangled = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    std::string name = (status == 0) ? demangled : typeid(T).name();
    free(demangled); // 释放abi分配的内存
    return name;
}


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

        T *inst = instance.load(std::memory_order_acquire);
        if (inst == nullptr) {
            std::lock_guard<std::mutex> lock(sMutex);
            inst = instance.load(std::memory_order_relaxed);
            if (inst == nullptr) {
                inst = new (std::nothrow) T(std::forward<Args>(args)...);
                if (!inst) {
                    std::cout << "create " << typeid(T).name()
                              << " error, reason is " << strerror(errno)
                              << ", exit now... " << std::endl;
                    exit(0);
                }
                instance.store(inst, std::memory_order_release); // 原子存储
            }
        }

        return inst;
    }

    inline static void destory() {
        T* inst = instance.load(std::memory_order_acquire);
        if (inst) {
            std::lock_guard<std::mutex> lock(sMutex);
            inst = instance.load(std::memory_order_relaxed);
            if (inst) {
                instance.store(nullptr, std::memory_order_release);
                delete inst;
                std::cout << "Singleton [" << getTypeName<T>() << "] destroyed" << std::endl;
            }
        }
    }

    TypeSingle() = delete;
    TypeSingle(const TypeSingle &) = delete;
    TypeSingle(TypeSingle &&) = delete;
    TypeSingle &operator=(TypeSingle &&) = delete;
    ~TypeSingle() = delete;

private:
    static std::mutex sMutex;
    static std::atomic<T *> instance;
};

template<typename T>
std::mutex TypeSingle<T>::sMutex;

template<typename T>
std::atomic<T *> TypeSingle<T>::instance{nullptr};
}

#endif