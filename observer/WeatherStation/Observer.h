#pragma once

#include <set>
#include <functional>

template <typename T>
class IObserver
{
public:
	virtual void Update(T const& data) = 0;
	virtual ~IObserver() = default;
};

template <typename T>
class ObserverWithPriority
{
public:
	ObserverWithPriority(IObserver<T>& observer, unsigned priority = 0)
		: m_observer(observer)
		, m_priority(priority)
	{
	}

	bool operator <(const ObserverWithPriority& obesrver) const
	{
		return m_priority < obesrver.m_priority;
	}

	IObserver<T>& m_observer;
	unsigned m_priority;
};

template <typename T>
struct CmpPoint
{
	bool operator()(
		const ObserverWithPriority<T>* lObesrver,
		const ObserverWithPriority<T>* rObesrver
		) const
	{
		return lObesrver->m_priority < rObesrver->m_priority;
	}
};

template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(ObserverWithPriority<T>& observerWithPriority) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(ObserverWithPriority<T>& observerWithPriority) = 0;
};

template <class T>
class CObservable : public IObservable<T>
{
public:
	typedef ObserverWithPriority<T> ObserverType;
	void RegisterObserver(ObserverType& observerWithPriority) override // отдельно наблюдатель и приоритет
	{
		m_observers.insert(&observerWithPriority);
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		std::set<ObserverType*, CmpPoint<T>> observersCopy = m_observers;
		for (auto & it : observersCopy)
		{
			it->m_observer.Update(data);
		}
	}

	void RemoveObserver(ObserverType& observerWithPriority) override // разобарться с передачей аргументов по ссылке
	{
		m_observers.erase(&observerWithPriority);
	}

protected:
	virtual T GetChangedData()const = 0;

private:
	std::set<ObserverType*, CmpPoint<T>> m_observers; // не получится с одним приоритетом получать, сделать при
};
