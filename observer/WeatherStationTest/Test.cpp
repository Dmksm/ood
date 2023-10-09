#define CATCH_CONFIG_MAIN
#include "../../catch2/catch.hpp"

#include "../WeatherStation/Observer.h"
#include "../WeatherStation/WeatherData.h"
#include <memory>
#include <sstream>
#include <fstream>

template <class T>
class CMockObservable : public IObservable<T>
{
public:
	typedef ObserverWithPriority<T> ObserverType;
	void RegisterObserver(ObserverType& observerWithPriority) override
	{
		m_observers.insert(&observerWithPriority);
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		std::set<ObserverType*, CmpPoint<T>> observersCopy = m_observers;
		for (auto& it : observersCopy)
		{
			it->m_observer.Update(data);
		}
	}

	void RemoveObserver(ObserverType& observerWithPriority) override
	{
		m_observers.erase(&observerWithPriority);
	}

	std::set<ObserverType*, CmpPoint<T>> GetObserverSequence()
	{
		return m_observers;
	};

protected:
	virtual T GetChangedData()const = 0;

private:
	std::set<ObserverType*, CmpPoint<T>> m_observers;
};

class MockDisplay : public IObserver<SWeatherInfo>
{
public:
	MockDisplay(std::shared_ptr<IObservable<SWeatherInfo>>&& subject, std::stringstream& output)
		: m_subject(std::move(subject))
		, m_output(output)
	{
	};

	bool IsUpdate()
	{
		return m_isUpdate;
	}

private:
	void Update(SWeatherInfo const& data) override
	{
		m_output << "Current Temp " << data.temperature << std::endl;
		m_output << "Current Hum " << data.humidity << std::endl;
		m_output << "Current Pressure " << data.pressure << std::endl;
		m_output << "----------------" << std::endl;
		
		m_isUpdate = true;
		ObserverWithPriority<SWeatherInfo> observerWithPriority(*this);
		m_subject->RemoveObserver(observerWithPriority);
	}

	std::shared_ptr<IObservable<SWeatherInfo>> m_subject;
	bool m_isUpdate = false;
	std::stringstream& m_output;
};

class CMockWeatherData : public CMockObservable<SWeatherInfo>
{
public:
	double GetTemperature()const
	{
		return m_temperature;
	}

	double GetHumidity()const
	{
		return m_humidity;
	}

	double GetPressure()const
	{
		return m_pressure;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;

		MeasurementsChanged();
	}
protected:
	SWeatherInfo GetChangedData()const override
	{
		SWeatherInfo info;
		info.subject = this;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
};

class MockStatsDisplay : public IObserver<SWeatherInfo>
{
public:
	struct Sensor
	{
		std::map<std::string, StatsData> m_statsList =
		{
			{
				std::string("Humidity"),
				StatsData()
			},
			{
				std::string("Temp"),
				StatsData()
			},
			{
				std::string("Pressure"),
				StatsData()
			},
		};
	};

	MockStatsDisplay(std::stringstream& output) : m_output(output)
	{
	};

	std::map<const IObservable<SWeatherInfo>*, Sensor> GetState()
	{
		return m_sensors;
	}
private:
	double GetComparableData(const std::string& name, SWeatherInfo info)
	{
		if (name == "Humidity")
		{
			return info.humidity;
		}
		if (name == "Temp")
		{
			return info.temperature;
		}
		if (name == "Pressure")
		{
			return info.pressure;
		}
		else
		{
			throw std::logic_error(name + " is incompatible with no one from SWeatherInfo");
		}
	}

	void Update(SWeatherInfo const& data) override
	{
		bool isNewStatsInfo = false;
		for (auto& it : m_sensors[data.subject].m_statsList)
		{
			if (isNewStatsInfo)
			{
				m_output << std::endl;
			}
			isNewStatsInfo = true;

			double comparableData = GetComparableData(it.first, data);
			if (it.second.min > comparableData)
			{
				it.second.min = comparableData;
			}
			if (it.second.max < comparableData)
			{
				it.second.max = comparableData;
			}
			it.second.acc += comparableData;
			++it.second.countAcc;

			m_output << "Max " << it.first << " " << it.second.max << std::endl;
			m_output << "Min " << it.first << " " << it.second.min << std::endl;
			m_output << "Average " << it.first << " " <<
				(it.second.acc / it.second.countAcc) << std::endl;
		}
		m_output << "----------------" << std::endl;
	}

	std::stringstream& m_output;
	std::map<const IObservable<SWeatherInfo>*, Sensor> m_sensors;
};

SCENARIO("Delete in update method with check priority")
{
	GIVEN("MockDisplay")
	{
		CMockWeatherData wd;
		std::stringstream ss;
		MockDisplay display(std::make_shared<CMockWeatherData>(wd), ss);
		ObserverWithPriority<SWeatherInfo> displayWithPriority(display, 5u);
		wd.RegisterObserver(displayWithPriority);

		MockDisplay display2(std::make_shared<CMockWeatherData>(wd), ss);
		ObserverWithPriority<SWeatherInfo> displayWithPriority2(display2, 10u);
		wd.RegisterObserver(displayWithPriority2);

		MockDisplay display3(std::make_shared<CMockWeatherData>(wd), ss);
		ObserverWithPriority<SWeatherInfo> displayWithPriority3(display3, 1u);
		wd.RegisterObserver(displayWithPriority3);

		MockDisplay display4(std::make_shared<CMockWeatherData>(wd), ss);
		ObserverWithPriority<SWeatherInfo> displayWithPriority4(display4, 7u);
		wd.RegisterObserver(displayWithPriority4);

		for (auto& it : wd.GetObserverSequence())
		{
			ss << it->m_priority << " ";
		}

		REQUIRE(ss.str() == "1 5 7 10 ");

		WHEN("attempt to delete from observers list")
		{
			THEN("Succesed deleted without errors")
			{
				REQUIRE_NOTHROW(wd.SetMeasurements(4, 0.8, 761));
				REQUIRE(display.IsUpdate() == true);
				REQUIRE(display2.IsUpdate() == true);
				REQUIRE(display3.IsUpdate() == true);
			}
		}
	}
}

SCENARIO("Check displaying information about several stations")
{
	GIVEN("MockDisplay")
	{
		CMockWeatherData input, output;
		std::stringstream ss, comparable;

		MockStatsDisplay statsDisplay(ss);
		ObserverWithPriority<SWeatherInfo> statsDisplayWithPriority(statsDisplay);

		input.RegisterObserver(statsDisplayWithPriority);
		output.RegisterObserver(statsDisplayWithPriority);

		WHEN("Input and output measurements was changed")
		{
			input.SetMeasurements(2, 10, 740);
			output.SetMeasurements(4, 11, 741);
			THEN("Input measurements are not equale output measurements")
			{
				comparable << "Max Humidity " << 10 << std::endl;
				comparable << "Min Humidity " << 10 << std::endl;
				comparable << "Average Humidity " << 10 << std::endl;
				comparable << std::endl;

				comparable << "Max Pressure " << 740 << std::endl;
				comparable << "Min Pressure " << 740 << std::endl;
				comparable << "Average Pressure " << 740 << std::endl;
				comparable << std::endl;

				comparable << "Max Temp " << 2 << std::endl;
				comparable << "Min Temp " << 2 << std::endl;
				comparable << "Average Temp " << 2 << std::endl;
				comparable << "----------------" << std::endl;

				comparable << "Max Humidity " << 11 << std::endl;
				comparable << "Min Humidity " << 11 << std::endl;
				comparable << "Average Humidity " << 11 << std::endl;
				comparable << std::endl;

				comparable << "Max Pressure " << 741 << std::endl;
				comparable << "Min Pressure " << 741 << std::endl;
				comparable << "Average Pressure " << 741 << std::endl;
				comparable << std::endl;

				comparable << "Max Temp " << 4 << std::endl;
				comparable << "Min Temp " << 4 << std::endl;
				comparable << "Average Temp " << 4 << std::endl;
				comparable << "----------------" << std::endl;
				REQUIRE(ss.str() == comparable.str());
			}
		}
	}
}
