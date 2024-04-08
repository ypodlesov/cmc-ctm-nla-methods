#pragma once

#include <thread>
#include <atomic>
#include <ranges>
#include <vector>

#include <catch2/benchmark/catch_benchmark.hpp>

static constexpr auto kCount = 500'000;
static constexpr auto kSum = static_cast<int64_t>(kCount) * (kCount - 1) / 2;

void Send(auto* channel, uint32_t threads_count) {
    auto send_func = [&](int start) {
        for (auto i = start; i < kCount; i += threads_count) {
            channel->Send(i);
        }
    };
    std::vector<std::jthread> threads;
    for (auto i : std::views::iota(0u, threads_count)) {
        threads.emplace_back(send_func, i);
    }
    threads.clear();
    channel->Close();
}

int64_t CalcSum(auto* channel, uint32_t senders_count, uint32_t readers_count) {
    using ChannelType = std::remove_pointer_t<decltype(channel)>;
    std::atomic sum = 0ll;
    auto read_func = [&] {
        while (auto value = channel->Recv()) {
            sum.fetch_add(*value, std::memory_order::relaxed);
        }
    };
    std::vector<std::jthread> threads;
    threads.emplace_back(Send<ChannelType>, channel, senders_count);
    for (auto i = 0u; i < readers_count; ++i) {
        threads.emplace_back(read_func);
    }
    threads.clear();
    return sum;
}
