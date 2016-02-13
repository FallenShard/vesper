#pragma once

#include <set>

#include "Delegate.hpp"

template<typename ReturnType, typename... ParamTypes>
class EventSource
{
public:
    EventSource()
    {
    }

    template<typename T, ReturnType(T::*callbackMethod)(ParamTypes...)>
    void subscribe(T* obj)
    {
        auto del = Delegate<ReturnType, ParamTypes...>::fromFunction<T, callbackMethod>(obj);
        m_delegates.insert(del);
    }

    template<typename T>
    void subscribe(Delegate<ReturnType, ParamTypes...> del)
    {
        m_delegates.insert(del);
    }

    template<typename T, ReturnType(T::*callbackMethod)(ParamTypes...)>
    void unsubscribe(T* obj)
    {
        auto del = Delegate<ReturnType, ParamTypes...>::fromFunction<T, callbackMethod>(obj);
        m_delegates.erase(del);
    }

    void operator()(ParamTypes... args)
    {
        for (auto& delegate : m_delegates)
            delegate(args...);
    }

private:
    EventSource<ReturnType, ParamTypes...>& operator=(const EventSource<ReturnType, ParamTypes...> other) = delete;
    EventSource(const EventSource<ReturnType, ParamTypes...>& other) = delete;

    std::set<Delegate<ReturnType, ParamTypes...>> m_delegates;
};
