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
/**
 *  @class SafeList
 *  @brief Thread Safe wrapper of std::list.
 *
 *  @tparam Type of the elements inside std::list container.
 */
template <typename T>
class SafeList {
    public:
    /**
     *  @brief Default constructor of SafeList with no consumeFunc.
     */
    SafeList() : _consumeFunc(nullptr) {}

    /**
     *  @brief Constructor of SafeList with a consumeFunc parameter.
     *
     *  @param func Function that will be used on each parameter to consume the `std::list`.
     */
    SafeList(std::function<void(T &)> func) : _consumeFunc(func) {}

    /**
     *  @brief Destructor of the SafeList class.
     */
    ~SafeList() = default;

    /**
     *  @brief Push at the end of the `std::list` the data specified in @param data.
     *
     *  @param data Data to push at the end of the list.
     */
    void pushBack(const T &data)
    {
        std::scoped_lock<std::mutex> lck(_mut);
        _datas.push_back(data);
    }

    /**
     *  @brief Register a consume function that will be used on each data when consuming the list.
     *
     *  @param func Function that will will be used as the consumeFunction.
     */
    void registerConsumeFunc(std::function<void(T)> func)
    {
        _consumeFunc = func;
    }

    /**
     *  @brief Consume all the elements of the `std::list` (poping them).
     */
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
    std::function<void(T &)> _consumeFunc;
};

} // namespace eng
