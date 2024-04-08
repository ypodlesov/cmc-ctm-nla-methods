#include <catch2/catch_session.hpp>

int main(int argc, char* argv[]) {
    Catch::Session session;
    session.configData().benchmarkSamples = 5;
    if (auto code = session.applyCommandLine(argc, argv)) {
        return code;
    }
    return session.run();
}
