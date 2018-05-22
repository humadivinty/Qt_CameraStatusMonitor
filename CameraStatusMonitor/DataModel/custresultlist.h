#ifndef CUSTRESULTLIST_H
#define CUSTRESULTLIST_H
#include<Camera/cameraresult.h>
#include<list>
#include <memory>
#include <mutex>
#include<condition_variable>
#include <chrono>


template <typename T>
class ThreadSafeList
{
public:
    void pushBack(T& value);
    void pushBack(T&& value);
    void pop(T&value);
    int TimePop(T&value, uint32_t milliseconds);
    bool IsEmpty();
    size_t GetSize();

private:
    std::list<T> _list;

    std::mutex _mutex;
    std::condition_variable _condition;
};

template <typename T>
void ThreadSafeList<T>::pushBack(T& value)
{
    std::lock_guard<std::mutex> lck(_mutex);
    _list.push_back(value);
    _condition.notify_one();
}

template <typename T>
void ThreadSafeList<T>::pushBack(T&& value)
{
    std::lock_guard<std::mutex> lck(_mutex);
    _list.push_back(std::move(value));
    _condition.notify_one();
}

template <typename T>
void ThreadSafeList<T>::pop(T& value)
{
    std::unique_lock<std::mutex> lck(_mutex);
    _condition.wait(lck, [this]{return !_list.empty(); });
    value = _list.front();
    _list.pop_front();
}

template<typename T>
int ThreadSafeList<T>::TimePop(T &value, uint32_t milliseconds)
{
    std::unique_lock<std::mutex> lck(_mutex);
    if(false == _condition.wait_for(lck, std::chrono::milliseconds(milliseconds), [this]{ return !_list.empty(); }))
        return -1;

    value = _list.front();
    _list.pop_front();
    return 0;
}

template <typename T>
bool ThreadSafeList<T>::IsEmpty()
{
    std::lock_guard<std::mutex> lck(_mutex);
    return _list.empty();
}

template <typename T>
size_t ThreadSafeList<T>::GetSize()
{
    std::lock_guard<std::mutex> lck(_mutex);
    return _list.size();
}

#endif // CUSTRESULTLIST_H
