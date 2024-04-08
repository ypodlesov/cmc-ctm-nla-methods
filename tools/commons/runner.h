#pragma once

#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include <optional>

class Runner {
public:
    explicit Runner(uint64_t num_iterations) : num_iterations_{num_iterations} {
    }

    template <class Function, class... Args>
    void Do(Function&& func, Args&&... args) {
        threads_.emplace_back([this](Function&& func, Args&&... args) mutable {
            while (iteration_.fetch_add(1, std::memory_order::relaxed) < num_iterations_) {
                func(std::forward<Args>(args)...);
            }
        }, std::forward<Function>(func), std::forward<Args>(args)...);
    }

    void Wait() {
        threads_.clear();
    }

private:
    const uint64_t num_iterations_;
    std::atomic<uint64_t> iteration_;
    std::vector<std::jthread> threads_;
};

template <class Function, class OnExitFunction>
class TaskWithExit {
public:
    TaskWithExit(Function func, OnExitFunction on_exit)
        : func_{std::move(func)}, on_exit_{std::move(on_exit)} {
    }
    ~TaskWithExit() {
        if (on_exit_) {
            (*on_exit_)();
        }
    }
    TaskWithExit(const TaskWithExit&) = delete;
    TaskWithExit& operator=(const TaskWithExit&) = delete;
    TaskWithExit(TaskWithExit&& other)
        : func_{std::move(other.func_)}, on_exit_{std::move(other.on_exit_)} {
        other.on_exit_.reset();
    }
    
    template <class... Args>
    auto operator()(Args&&... args) {
        return func_(std::forward<Args>(args)...);
    }

private:
    Function func_;
    std::optional<OnExitFunction> on_exit_;
};

class TimeRunner {
public:
    explicit TimeRunner(std::chrono::steady_clock::duration time) : time_{time} {
        flag_.test_and_set(std::memory_order::relaxed);
        threads_.emplace_back([&, end = std::chrono::steady_clock::now() + time] {
            std::this_thread::sleep_until(end);
            flag_.clear(std::memory_order::relaxed);
        });
    }

    template <class Function, class... Args>
    void Do(Function&& func, Args&&... args) {
        DoWithInit([] {}, std::forward<Function>(func), std::forward<Args>(args)...);
    }

    template <class InitFunction, class Function, class... Args>
    void DoWithInit(InitFunction&& init_f, Function&& func, Args&&... args) {
        threads_.emplace_back([this,
                init_f = std::forward<InitFunction>(init_f),
                func = std::forward<Function>(func),
                ... args = std::forward<Args>(args)]() mutable {
            init_f();
            size_t num_iterations = 0;
            while (flag_.test(std::memory_order::relaxed)) {
                func(std::forward<Args>(args)...);
                ++num_iterations;
            }
            ref_.fetch_add(num_iterations, std::memory_order::relaxed);
        });
    }

    std::chrono::steady_clock::duration Wait() {
        threads_.clear();
        return time_ / std::max(1ul, num_iterations_);
    }

private:
    const std::chrono::steady_clock::duration time_;
    size_t num_iterations_ = 0;
    const std::atomic_ref<size_t> ref_{num_iterations_};
    std::atomic_flag flag_;
    std::vector<std::jthread> threads_;
};
