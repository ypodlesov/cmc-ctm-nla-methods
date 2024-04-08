#pragma once

#include <vector>
#include <atomic>
#include <ranges>
#include <mutex>
#include <thread>
#include <algorithm>

#include <catch2/catch_test_macros.hpp>

namespace {

void TestDataRace(auto* channel) {
    int* p = nullptr;
    std::jthread t1{[&] {
        p = new int{25};
        channel->Send(5);
    }};
    std::jthread t2{[&] {
        auto v = channel->Recv();
        REQUIRE((v && (*v == 5)));
        REQUIRE((p && (*p == 25)));
        delete p;
    }};
}

void TestCopy(auto* channel) {
    std::vector v = {1, 2, 3};
    std::thread t1{[&] { channel->Send(v); }};
    std::thread t2{[&] {
        auto opt = channel->Recv();
        REQUIRE(opt);
        REQUIRE(*opt == std::vector{1, 2, 3});
    }};
    t1.join();
    t2.join();
    REQUIRE(v == std::vector{1, 2, 3});
}

void TestClose(auto* channel) {
    channel->Close();
    std::thread thread{[&] {
        CHECK_THROWS_AS(channel->Send(2), std::runtime_error);
    }};
    thread.join();
    CHECK(!channel->Recv());
}

void TestMoveOnly(auto* channel) {
    auto u_p = std::make_unique<std::string>("abacaba");
    auto* p = u_p.get();
    std::jthread t1{[&] {
        channel->Send(std::move(u_p));
    }};
    std::jthread t2{[&] {
        auto opt = channel->Recv();
        REQUIRE(opt);
        const auto& u_p = *opt;
        REQUIRE(u_p);
        REQUIRE(u_p.get() == p);
        REQUIRE(*u_p == "abacaba");
    }};
}

std::vector<int> Join(const std::vector<std::vector<int>>& values) {
    auto joined = std::views::join(values);
    return {joined.begin(), joined.end()};
}

void CheckValues(const std::vector<std::vector<int>>& send_values,
                 const std::vector<std::vector<int>>& recv_values) {
    auto all_send = Join(send_values);
    auto all_recv = Join(recv_values);
    std::ranges::sort(all_send);
    std::ranges::sort(all_recv);
    REQUIRE(all_send == all_recv);
    REQUIRE(all_send.size() > 5'000);
}

}  // namespace

namespace CheckMTImpl {
    inline std::mutex mutex;
}

#define CHECK_MT( ... ) do { \
    std::lock_guard guard{CheckMTImpl::mutex}; \
    CHECK( __VA_ARGS__ ); \
} while (false);
