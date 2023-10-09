#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
#include <iterator>
#include "Observer.h"
#include <optional>
#include <numeric>
#include <numbers>
#include <cmath>

struct Wind
{
	double windSpeed = 0;
	double windDirection = 0;
};

struct SWeatherInfo
{
	const IObservable<SWeatherInfo>* subject = nullptr;
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
	std::optional<Wind> wind;
};

class CDisplay: public IObserver<SWeatherInfo>
{
private:
	void Update(SWeatherInfo const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		if (data.wind.has_value())
		{
			std::cout << "Wind direction " << data.wind.value().windDirection << std::endl;
			std::cout << "Wind speed " << data.wind.value().windSpeed << std::endl;
		}
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

struct StatsWindData
{
	double x = 0;
	double y = 0;
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
		for (auto& it : m_sensors[data.subject].basicStatsList)
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
			
			std::cout << "Max " << it.first << " " << it.second.max << std::endl;
			std::cout << "Min " << it.first << " " << it.second.min << std::endl;

			it.second.acc += comparableData;
			++it.second.countAcc;
			std::cout << "Average " << it.first << " " <<
				(it.second.acc / it.second.countAcc) << std::endl;
		}
		if (data.wind.has_value())
		{
			std::cout << std::endl;
			Wind wind = data.wind.value();

			double x, y, newX, newY;
			x = m_sensors[data.subject].windStatsList.x;
			y = m_sensors[data.subject].windStatsList.y;

			newX = (wind.windDirection == 270 || wind.windDirection == 90)
				? x
				: cos(wind.windDirection * std::numbers::pi / 180) * wind.windSpeed + x;
			newY = (wind.windDirection == 180)
				? y
				: sin(wind.windDirection * std::numbers::pi / 180) * wind.windSpeed + y;

			double windSpeed = pow(pow(newX, 2) + pow(newY, 2), 0.5);
			double windDirection = (acos(newX / windSpeed) / std::numbers::pi * 180);

			m_sensors[data.subject].windStatsList.x = newX;
			m_sensors[data.subject].windStatsList.y = newY;
			std::cout << "Average wind direction " << windDirection << std::endl;
			std::cout << "Average wind speed " << windSpeed << std::endl;
		}
		std::cout << "----------------" << std::endl;
	}

	struct Sensor
	{
		std::map<std::string, StatsData> basicStatsList =
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
		StatsWindData windStatsList;
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

	std::optional<Wind> GetWind()const
	{
		return m_wind;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(
		double temp,
		double humidity,
		double pressure,
		std::optional<Wind> wind = std::nullopt
	)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;
		m_wind = wind;

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
		info.wind = GetWind();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;	
	double m_pressure = 760.0;	
	std::optional<Wind> m_wind = std::nullopt;
};
