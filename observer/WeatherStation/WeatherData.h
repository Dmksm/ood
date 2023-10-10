#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
#include <iterator>
#include "Observer.h"

struct SWeatherInfo
{
	const IObservable<SWeatherInfo>* subject = nullptr;
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
};

class CDisplay: public IObserver<SWeatherInfo>
{
private:
	void Update(SWeatherInfo const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

struct StatsData
{
	double min = std::numeric_limits<double>::infinity();
	double max = -std::numeric_limits<double>::infinity();
	double acc = 0;
	unsigned countAcc = 0;
};

class CStatsDisplay : public IObserver<SWeatherInfo>
{
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

	void Update(const SWeatherInfo& data) override
	{
		bool isNewStatsInfo = false;
		for (auto& it : m_sensors[data.subject].m_statsList)
		{
			if (isNewStatsInfo)
			{
				std::cout << std::endl;
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

			std::cout << "Max " << it.first << " " << it.second.max << std::endl;
			std::cout << "Min " << it.first << " " << it.second.min << std::endl;
			std::cout << "Average " << it.first << " " <<
				(it.second.acc / it.second.countAcc) << std::endl;
		}
		std::cout << "----------------" << std::endl;
	}

	struct Sensor
	{
		std::map<std::string, StatsData> m_statsList = //сделать так чтобы не дублировать код при использовании в других ( в статс дата, сделать классво умеющим обновлять свое состояние, выводом статс дисплей )
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

	std::map<const IObservable<SWeatherInfo>*, Sensor> m_sensors;
};

class CWeatherData : public CObservable<SWeatherInfo>
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
