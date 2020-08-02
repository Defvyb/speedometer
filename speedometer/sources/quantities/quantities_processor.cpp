#include "quantities_processor.h"
std::chrono::high_resolution_clock::duration TickProviderInterface::getTick() const{
    return std::chrono::high_resolution_clock::now().time_since_epoch();
}

QuantitiesProcessor::QuantitiesProcessor(std::shared_ptr<TickProviderInterface> tickProvider,
                                         FilteringOption isFilterOn):
    m_tickProvider(tickProvider),
    m_lastTickPoint(m_tickProvider->getTick()),
    m_isFilterOn(isFilterOn){
}

void QuantitiesProcessor::processStep(){
    using namespace std::chrono;
    auto newTick = m_tickProvider->getTick();

    if((newTick - m_lastExternalTickPoint.load()) > 17ms){ //~60 fps
        m_externalSpeed = 0;
    }

    if(FILTER_ON == m_isFilterOn) m_externalSpeed =  m_filter.process(m_externalSpeed);

    constexpr double nanosecInOur = 60.0 * 60.0 * 1000.0 * 1000.0 * 1000.0;
    double hours = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(newTick - m_lastTickPoint.load()).count()) / nanosecInOur;

    m_odoKm = m_odoKm + (hours * m_externalSpeed);
    m_lastTickPoint = newTick;
}

void QuantitiesProcessor::setExternalSpeed(double speed){
    m_externalSpeed = speed;
    m_lastExternalTickPoint = m_tickProvider->getTick();
}

double QuantitiesProcessor::getCurrentSpeed() const{
    return m_externalSpeed;
}

double QuantitiesProcessor::getOdoKm() const{
    return m_odoKm;
}

