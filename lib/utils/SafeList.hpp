/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SafeList
*/

#pragma once

#include <functional>
#include <list>
#include <mutex>

namespace eng {

template <typename T>
class SafeList {
    public:
    SafeList() : _consumeFunc(nullptr) {}

    SafeList(std::function<void (T&)> func) : _consumeFunc(func) {}

    ~SafeList() = default;

    void pushBack(const T &data)
    {
        std::scoped_lock<std::mutex> lck(_mut);
        _datas.push_back(data);
    }

    void registerConsumeFunc(std::function<void(T)> func)
    {
        _consumeFunc = func;
    }

    void consumeList()
    {
        if (!_consumeFunc) {
            return;
        }
        std::scoped_lock<std::mutex> lck(_mut);

        while (!_datas.empty()) {
            _consumeFunc(_datas.front());
            _datas.pop_front();
        }
    }

    private:
    std::list<T> _datas;
    std::mutex _mut;
    std::function<void (T&)> _consumeFunc;
};

} // namespace eng
