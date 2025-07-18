# base-components-tool
     零依赖，快速构建部署的小工具，目前支持线程池 定时器（支持多任务） log日志 cmdline 命令行解析 等功能 
## 一、构建方法
    ./build.sh

## 二、cmdline 命令行解析组件  
### 功能概述  
cmdline 是一个支持多类型参数解析的命令行工具，主要用于解析程序运行时的命令行参数。其核心能力包括：  
- 支持为基础类型（如 `int`、`float`）及多种 STL 容器类型（如 `std::unordered_multiset<int>`、`std::string`、`std::deque`、`std::queue`、`std::stack` 等）定义命令行选项；  
- 支持通过 `add` 方法灵活定义短选项（如 `-n`）、长选项（如 `--none`）、描述信息、依赖选项（如 `{"-s", "-us", "-i"}`）及参数范围（如枚举值 `{"aaa", "vvv", "bbb"}` 或数值区间 `[44, 99]`）；  
- 提供参数依赖检查、参数范围限制等功能，确保输入参数的合法性与逻辑一致性。  

### 核心优点  
- 多类型支持：跨平台、简单易用，兼容基础类型与几乎所有 STL 容器类型，满足复杂场景下的参数解析需求；  
- 依赖检查：可定义选项间的依赖关系（如 `--singlestring` 依赖 `{"-s", "-us", "-i"}`），确保参数逻辑一致；  
- 参数范围限制：支持数值区间（如 `[44, 99]`）或枚举值（如 `{"aaa", "vvv"}`）限制，避免非法参数输入；  
- 使用便捷：通过 `add` 方法链式定义选项，解析后通过 `get` 方法直接获取参数值，接口简单易用。  
- 轻量级、现代 C++ 风格（头文件仅），支持子命令、类型安全、自动生成帮助信息。


## 三、timer 定时器组件  
### 功能概述  
timer 是一个支持多任务的定时器组件，用于管理定时任务的添加、清除等操作。其行为可通过命令行选项控制（如 `./build/install/demo/timer/timer -a` 添加定时器，`-c` 延迟清除所有定时器），支持多任务并发执行。  

### 核心优点  
- 跨平台、简单易用。
- 多任务支持：可同时管理多个定时任务，适用于需要并行执行多个定时逻辑的场景（如监控、周期性数据同步）；  
- 灵活控制：提供 `add`（添加）、`clear`（延迟清除）等操作接口，支持动态调整定时任务生命周期；  
- 模式简化：内置 `default` 默认模式，降低基础场景的使用门槛，无需复杂配置即可快速启动定时器。  


## 四、threadpool 线程池组件  
### 功能概述  
threadpool 是一个线程管理组件，通过预创建线程并复用的方式处理任务队列中的任务，避免频繁创建/销毁线程带来的性能损耗，高效管理线程生命周期。  

### 核心优点（基于通用线程池特性及项目背景推测）  
- 跨平台、简单易用，
- 性能优化：通过线程复用减少线程创建/销毁的开销，提升高并发场景下的任务处理效率；  
- 资源可控：可限制线程数量上限，避免资源耗尽（如 CPU/内存过载），保障程序稳定性；  
- 任务队列管理：支持任务排队机制（可按顺序或优先级处理），避免任务堆积导致的程序崩溃；  
- 易用性：提供简单接口（如提交任务到线程池），隐藏线程管理细节，降低多线程编程复杂度。