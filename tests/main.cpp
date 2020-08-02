#include <gtest/gtest.h>
#include <chrono>
#include <speedometer/sources/quantities/quantities_processor.h>
#include <memory>
#include <random>

int main(int argc, char * argv [])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class TestTickProvider: public TickProviderInterface{
public:
    TestTickProvider() = default;
    virtual ~TestTickProvider() = default;

    TestTickProvider(TestTickProvider &) = delete;
    TestTickProvider(TestTickProvider &&) = delete;
    TestTickProvider& operator=(TestTickProvider&) = delete;
    TestTickProvider& operator=(TestTickProvider&&) = delete;

    virtual std::chrono::high_resolution_clock::duration getTick() const override
    {
        return m_tick;
    }

    void setTick(std::chrono::high_resolution_clock::duration tick)
    {
        m_tick=tick;
    }

   private:
    std::chrono::high_resolution_clock::duration m_tick;

};

TEST(test, testProcessorWithoutFilter)
{
    auto testTickProvider = std::make_shared<TestTickProvider>();
    QuantitiesProcessor processor(testTickProvider, FILTER_OFF);
    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(0)));

    processor.setExternalSpeed(100.0);
    processor.processStep();
    ASSERT_EQ(100, processor.getCurrentSpeed());

    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(16)));
    processor.processStep();
    ASSERT_EQ(100, processor.getCurrentSpeed());

    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(18)));
    processor.processStep();
    ASSERT_EQ(0.0, processor.getCurrentSpeed());

}



TEST(test, testProcessorWithFilter)
{
    auto testTickProvider = std::make_shared<TestTickProvider>();
    QuantitiesProcessor processor(testTickProvider);
    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(0)));

    processor.setExternalSpeed(100.0);
    processor.processStep();
    ASSERT_GE(processor.getCurrentSpeed(), 0);

    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(16)));
    processor.processStep();
    ASSERT_GE(processor.getCurrentSpeed(), 0);

    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(18)));
    processor.processStep();
    ASSERT_LE(processor.getCurrentSpeed(), 50);

}

TEST(test, testProcessorOdoWithoutFilter)
{
    auto testTickProvider = std::make_shared<TestTickProvider>();
    QuantitiesProcessor processor(testTickProvider, FILTER_OFF);
    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(0)));

    processor.setExternalSpeed(180.0);
    processor.processStep();
    ASSERT_EQ(180, processor.getCurrentSpeed());

    int i=1;
    while(i++!=20000)
    {
        processor.setExternalSpeed(180.0);
        testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(i)));
        processor.processStep();
    }

    ASSERT_NEAR(1.0, processor.getOdoKm(), 0.001); //180 km/h for 20 seconds = 1km
}

TEST(test, testProcessorOdoWithFilter)
{
    auto testTickProvider = std::make_shared<TestTickProvider>();
    QuantitiesProcessor processor(testTickProvider);
    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(0)));

    processor.setExternalSpeed(180.0);
    processor.processStep();

    int i=1;
    while(i++!=20000)
    {
        processor.setExternalSpeed(180.0);
        testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(i)));
        processor.processStep();
    }

    ASSERT_NEAR(1.0, processor.getOdoKm(), 0.001);

}

TEST(test, testProcessorOdoWithoutFilter_NoiseSpeed)
{
    auto testTickProvider = std::make_shared<TestTickProvider>();
    QuantitiesProcessor processor(testTickProvider, FILTER_OFF);
    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(0)));

    processor.setExternalSpeed(180.0);
    processor.processStep();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-5, 5);

    int i=1;
    while(i++!=20000)
    {
        int s = dis(gen);
        processor.setExternalSpeed(180.0 + s);
        testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(i)));
        processor.processStep();
    }

    ASSERT_NEAR(1.0, processor.getOdoKm(), 0.001);

}


TEST(test, testProcessorOdoWithFilter_NoiseSpeed)
{
    auto testTickProvider = std::make_shared<TestTickProvider>();
    QuantitiesProcessor processor(testTickProvider);
    testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(0)));

    processor.setExternalSpeed(180.0);
    processor.processStep();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-5, 5);
    int i=1;
    while(i++!=20000)
    {
        processor.setExternalSpeed(180.0 + dis(gen));
        testTickProvider->setTick(std::chrono::duration(std::chrono::milliseconds(i)));
        processor.processStep();
    }

    ASSERT_NEAR(1.0, processor.getOdoKm(), 0.001);
}





