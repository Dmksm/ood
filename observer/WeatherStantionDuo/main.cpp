#include "WeatherData.h"

int main()
{
	CWeatherData wdIn, wdOut;

	CDisplay display;
	ObserverWithPriority<SWeatherInfo> displayWithPriority(display, 1u);
	wdIn.RegisterObserver(displayWithPriority);
	wdOut.RegisterObserver(displayWithPriority);

	CStatsDisplay statsDisplay;
	ObserverWithPriority<SWeatherInfo> statsDisplayWithPriority(statsDisplay, 3u);
	wdIn.RegisterObserver(statsDisplayWithPriority);
	wdOut.RegisterObserver(statsDisplayWithPriority);

	wdOut.SetMeasurements(1, 4.5, 740, Wind{ 10, 0 });
	wdIn.SetMeasurements(2, 0.7, 760);
	wdOut.SetMeasurements(3, 0.8, 761, Wind{ 30, 180 });
	wdOut.SetMeasurements(3, 0.8, 761, Wind{ 20, 90 });
	wdIn.SetMeasurements(4, 0.8, 761);

	return 0;
}