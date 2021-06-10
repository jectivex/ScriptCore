
#include <array>
#include <atomic>
#include "test.h"

namespace script::utils::test {

static void handleMessage(Message& msg) {
  auto* i = static_cast<std::atomic_int64_t*>(msg.ptr0);
  (*i)++;
}

TEST(ThreadPool, Run) {
  ThreadPool tp(2, std::make_unique<MessageQueue>(1000));
  EXPECT_EQ(2, tp.workerCount());

  auto i = std::make_unique<std::atomic_int64_t>();

  constexpr auto max = 1000;
  for (int j = 0; j < max; ++j) {
    Message msg(handleMessage, nullptr);
    msg.ptr0 = i.get();
    tp.postMessage(msg);
  }

  tp.shutdown(false);
  tp.awaitTermination();

  EXPECT_EQ(max, i->load());
}

TEST(ThreadPool, MultiThreadRun) {
  constexpr auto kWorkerCount = 2;
  constexpr auto kProducerCount = 4;
  constexpr auto max = 1000;

  ThreadPool tp(kWorkerCount);
  EXPECT_EQ(kWorkerCount, tp.workerCount());

  auto i = std::make_unique<std::atomic_int64_t>();

  std::array<std::unique_ptr<std::thread>, kProducerCount> p;
  for (auto& t : p) {
    t = std::make_unique<std::thread>([&]() {
      for (int j = 0; j < max; ++j) {
        Message msg(handleMessage, nullptr);
        msg.ptr0 = i.get();
        tp.postMessage(msg);
      }
    });
  }

  for (auto& t : p) {
    t->join();
  }
  tp.shutdown(true);

  EXPECT_EQ(max * kProducerCount, i->load());
}

TEST(ThreadPool, Benchmark) {
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::steady_clock;
  using std::chrono_literals::operator""ms;

  constexpr auto kEnable = false;
  constexpr auto kRunTime = 5000ms;
  constexpr auto kWorkerThreads = 4;
  constexpr auto kProducerThreads = 4;

  // simple benchmark
  if (!kEnable) return;

  auto start = steady_clock::now();

  ThreadPool tp(kWorkerThreads, std::make_unique<MessageQueue>(1000));
  auto i = std::make_unique<std::atomic_int64_t>();

  Message stopMsg([](auto& msg) { static_cast<ThreadPool*>(msg.ptr0)->shutdownNow(false); },
                  nullptr);
  stopMsg.ptr0 = &tp;

  tp.postMessage(stopMsg, kRunTime);

  for (int j = 0; j < kProducerThreads; ++j) {
    std::thread([&]() {
      while (true) {
        Message msg(handleMessage, nullptr);
        msg.ptr0 = i.get();
        try {
          tp.postMessage(msg);
        } catch (std::runtime_error&) {
          break;
        }
      }
    }).detach();
  }

  tp.awaitTermination();

  // run time should be close to kRunTime
  auto runTimeMillis = duration_cast<milliseconds>((steady_clock::now() - start)).count();

  std::cout << "run time:" << runTimeMillis << "ms, " << i->load() << "ops"
            << " [" << i->load() * 1000 / static_cast<double>(runTimeMillis) << " ops/S]"
            << std::endl;
}

}  // namespace script::utils::test
