#pragma once

#include <queue>
#include <mutex>
#include <optional>

template <class T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;

    void push(T next){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(next);
    }

    bool empty(){
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    std::optional<T> pop(){
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_queue.empty()){
            return std::nullopt;
        }else{
            auto val = m_queue.front();
            m_queue.pop();
            return std::optional<T>(val);
        }
    }


private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};

