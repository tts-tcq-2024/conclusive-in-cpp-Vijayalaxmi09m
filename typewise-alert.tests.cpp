#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(inferBreach(20, 30, 40), TOO_LOW);
    EXPECT_EQ(inferBreach(50, 30, 40), TOO_HIGH);
    EXPECT_EQ(inferBreach(35, 30, 40), NORMAL);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreach) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 30), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 36), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 39), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToController) {
    BatteryCharacter battery = {PASSIVE_COOLING, "BrandA"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, battery, 25);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("feed : 0"), std::string::npos);  // NORMAL case

    checkAndAlert(TO_CONTROLLER, battery, 36);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("feed : 2"), std::string::npos);  // TOO_HIGH case
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmail) {
    BatteryCharacter battery = {HI_ACTIVE_COOLING, "BrandB"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, battery, 46);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("To: a.b@c.com"), std::string::npos);
    EXPECT_NE(output.find("Hi, the temperature is too high"), std::string::npos);

    checkAndAlert(TO_EMAIL, battery, 20);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("Hi, the temperature is too low"), std::string::npos);
}
