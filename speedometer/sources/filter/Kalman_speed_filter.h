#pragma once

class KalmanSpeedFilter final{
public:
    explicit KalmanSpeedFilter(double noise = 2.0, double measureError = 15.0);
    ~KalmanSpeedFilter() = default;

    KalmanSpeedFilter(KalmanSpeedFilter &) = delete;
    KalmanSpeedFilter(KalmanSpeedFilter &&) = delete;
    KalmanSpeedFilter& operator=(KalmanSpeedFilter&) = delete;
    KalmanSpeedFilter& operator=(KalmanSpeedFilter&&) = delete;

    double process(double speed);


private:
    double m_X0 = 0.0;
    double m_P0 = 0.0;

    double m_noise;
    double m_measureError;

    double m_speed = 0.0;
    double m_covariance = 0.1;

};
