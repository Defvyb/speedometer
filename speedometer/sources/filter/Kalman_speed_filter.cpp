#include "filter/Kalman_speed_filter.h"

KalmanSpeedFilter::KalmanSpeedFilter(double noise, double measureError): m_noise(noise), m_measureError(measureError){
}


double KalmanSpeedFilter::process(double speed){
    //time update - prediction
    m_X0 = m_speed;
    m_P0 = m_covariance + m_noise;

    //measurement update - correction
    auto K = m_P0/(m_P0 + m_measureError);
    m_speed = m_X0 + K*(speed - m_X0);
    m_covariance = (1 - K)*m_P0;

    return m_speed;
}


