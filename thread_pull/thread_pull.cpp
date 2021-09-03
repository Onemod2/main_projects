#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <functional>
#include <mutex>
#include <chrono>
#include <condition_variable>

// Unbounded no block Many Producer Many Concumer Queue

using namespace std::chrono_literals;

template <typename T>
class UnboundedNoBlockMPMCQueue {
    public:
        T take() {
            std::unique_lock<std::mutex> ul(m_);
            while (subjects_.empty()) {
                not_empty_.wait(ul);
            }
            return take_last();
        }

        T take_last() {
            auto last_subject = std::move(subjects_.back());
            subjects_.pop_back();
            
            return last_subject;
        }

        void put(T&& sub) {
            std::lock_guard<std::mutex> ul(m_);
            subjects_.push_front(std::move(sub));
            not_empty_.notify_one();
        }

    private:
        std::mutex m_;
        std::deque<T> subjects_;
        std::condition_variable not_empty_;
};

class StaticThreadPull {
    private:
        using SubjectType = std::function<void()>;
    
    public:
        StaticThreadPull(std::size_t n) {
            for (std::size_t i = 0; i < n; ++i) {
                workers_.emplace_back([this, i]() {
                    performSubject(i);
                });
            }
        }

        void Subject(SubjectType&& sub) {
            subjects_queue_.put(std::move(sub));
        }

        void Join() {
            for (auto& worker : workers_) {
                subjects_queue_.put({});
            }
            for (auto& worker : workers_) {
                worker.join();
            }
        }

    private:
        void performSubject(std::size_t i) {
           while (true) {
                auto subject = subjects_queue_.take();
                if (!subject) {
                    break;
                }
                subject();
           }
        }

        UnboundedNoBlockMPMCQueue<SubjectType> subjects_queue_;
        std::vector<std::thread> workers_;
};

int main() {
    StaticThreadPull thread_pull(4);
    std::mutex m;
    int val = 0; 

    for (std::size_t i = 0; i < 1'000'000; ++i) {
        thread_pull.Subject([&]() {
            std::lock_guard<std::mutex> m_(m);
            val++; 
        });
    }

    thread_pull.Join();

    std::cout << val << std::endl;

		// end of program    
    return 0;
}


