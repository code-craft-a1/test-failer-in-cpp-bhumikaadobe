#include <string>
#include <iostream>
#include <assert.h>
#include <gtest/gtest.h>

using namespace std;

namespace WeatherSpace
{    
    class IWeatherSensor {
        public:
            virtual double TemperatureInC() const = 0;
            virtual int Precipitation() const = 0;
            virtual int Humidity() const = 0;
            virtual int WindSpeedKMPH() const = 0;
    };
    /// <summary>
    /// This is a stub for a weather sensor. For the sake of testing 
    /// we create a stub that generates weather data and allows us to
    /// test the other parts of this application in isolation
    /// without needing the actual Sensor during development
    /// </summary>
    class SensorStub : public IWeatherSensor {
        int Humidity() const override {
            return 72;
        }

        int Precipitation() const override {
            return 70;
        }

        double TemperatureInC() const override {
            return 26;
        }

        int WindSpeedKMPH() const override {
            return 52;
        }
    };

    class HighPrecipitationStub : public IWeatherSensor {
        int Humidity() const override {
            return 90;
        }

        int Precipitation() const override {
            return 80; //high precipitation
        }

        double TemperatureInC() const override {
            return 30; // hot
        }

        int WindSpeedKMPH() const override { 
            return 45; 
        }    // low wind speed <= 50
    };

    string Report(const IWeatherSensor& sensor)
    {
        int precipitation = sensor.Precipitation();
        // precipitation < 20 is a sunny day
        string report = "Sunny Day";

        if (sensor.TemperatureInC() > 25)
        {
            if (precipitation >= 20 && precipitation < 60)
                report = "Partly Cloudy";
            else if (sensor.WindSpeedKMPH() > 50)
                report = "Alert, Stormy with heavy rain";
        }
        return report;
    }
    
    void TestRainy()
    {
        SensorStub sensor;
        string report = Report(sensor);
        cout << report << endl;
        assert(report.find("rain") != string::npos);
    }

    void TestHighPrecipitation()
    {
        // This instance of stub needs to be different-
        // to give high precipitation (>60) and low wind-speed (<50)
        SensorStub sensor;

        // strengthen the assert to expose the bug
        // (function returns Sunny day, it should predict rain)
        string report = Report(sensor);
        assert(report.length() > 0);
    }

    void TestHighPrecipitationLowWind()
    {
        HighPrecipitationStub sensor;
        string report = Report(sensor);
        cout << "Report: High Precipitation and Low Wind: "<< report << endl;
        EXPECT_NE(report.find("rain") , string::npos) << "'rain' Expected in report,rather received " << report;
    }
}

void testWeatherReport() {
    cout << "\nWeather report test\n";
    WeatherSpace::TestRainy();
    WeatherSpace::TestHighPrecipitation();
    WeatherSpace::TestHighPrecipitationLowWind()
    cout << "All is well (maybe)\n";
}
