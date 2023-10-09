#include "WeatherData.h"

int main()
{
	CWeatherData wd, wd2;

	CDisplay display;
	ObserverWithPriority<SWeatherInfo> displayWithPriority(display, 1u);
	wd.RegisterObserver(displayWithPriority);
	wd2.RegisterObserver(displayWithPriority);

	CStatsDisplay statsDisplaySecond;
	ObserverWithPriority<SWeatherInfo> statsDisplayWithPrioritySecond(statsDisplaySecond, 5u);
	wd.RegisterObserver(statsDisplayWithPrioritySecond);
	wd2.RegisterObserver(statsDisplayWithPrioritySecond);

	CStatsDisplay statsDisplay;
	ObserverWithPriority<SWeatherInfo> statsDisplayWithPriority(statsDisplay, 3u);
	wd.RegisterObserver(statsDisplayWithPriority);
	wd2.RegisterObserver(statsDisplayWithPriority);

	wd2.SetMeasurements(1, 4.5, 740);
	wd.SetMeasurements(2, 0.7, 760);
	wd2.SetMeasurements(3, 0.8, 761);
	wd.SetMeasurements(4, 0.8, 761);

	wd.RemoveObserver(statsDisplayWithPriority);

	wd.SetMeasurements(10, 0.8, 761);
	wd.SetMeasurements(-10, 0.8, 761);
	return 0;
}