#pragma once
#include <stdint.h>
#include <chrono>
#include <atomic>
#include <filter/Kalman_speed_filter.h>
#include <memory>
class TickProviderInterface{
public:
    TickProviderInterface() = default;
    virtual ~TickProviderInterface() = default;

    TickProviderInterface(TickProviderInterface &) = delete;
    TickProviderInterface(TickProviderInterface &&) = delete;
    TickProviderInterface& operator=(TickProviderInterface&) = delete;
    TickProviderInterface& operator=(TickProviderInterface&&) = delete;

    virtual std::chrono::high_resolution_clock::duration getTick() const;
};

enum FilteringOption
{
  FILTER_OFF,
  FILTER_ON

};

class QuantitiesProcessor final{

public:
    QuantitiesProcessor(std::shared_ptr<TickProviderInterface> tickProvider = std::make_shared<TickProviderInterface>(),
                        FilteringOption isFilterOn = FILTER_ON);
    ~QuantitiesProcessor() = default;

    QuantitiesProcessor(QuantitiesProcessor &) = delete;
    QuantitiesProcessor(QuantitiesProcessor &&) = delete;
    QuantitiesProcessor& operator=(QuantitiesProcessor&) = delete;
    QuantitiesProcessor& operator=(QuantitiesProcessor&&) = delete;

    void processStep();
    void setExternalSpeed(double speed);

    double getCurrentSpeed() const;
    double getOdoKm() const;

private:
    KalmanSpeedFilter m_filter;
    std::shared_ptr<TickProviderInterface> m_tickProvider;
    std::atomic<double> m_externalSpeed = 0;
    std::atomic<std::chrono::high_resolution_clock::duration> m_lastExternalTickPoint;
    std::atomic<std::chrono::high_resolution_clock::duration> m_lastTickPoint;
    std::atomic<double> m_odoKm = 0;
    FilteringOption m_isFilterOn;
};
