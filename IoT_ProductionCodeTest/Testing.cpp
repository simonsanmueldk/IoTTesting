/*
 * Testing.cpp
 *
 * Created: 18/05/2022 12.49.21
 *  Author: Simon Samuel & Chen
 */

#include "gtest/gtest.h"
#include "C:\Users\simon\New SEP\Testing\fff-master\fff.h"
//#include "C:\Users\simon\New SEP\Testing\VIAFreeRTOSTest\FreeRTOS_FFF_MocksDeclaration.h"

DEFINE_FFF_GLOBALS

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <TempHumSensor.h>
#include "event_groups.h"


float temperature;
float humidity;

//Drivers included
#include "C:\Users\simon\New SEP\IOT\4.Semester\drivers\hih8120.h"

}
//IoT mocks 
FAKE_VOID_FUNC(tempHum_init);
FAKE_VOID_FUNC(tempHum_getDataFromTempHumSensorTask);
FAKE_VALUE_FUNC(float, hih8120_getHumidity);
FAKE_VALUE_FUNC(float, hih8120_getTemperature);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);

//FreeRTOS mocks
FAKE_VOID_FUNC(vTaskDelay, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xSempahoreGive, SemaphoreHandle_t);
FAKE_VOID_FUNC(xTaskDelayUntil, TickType_t*, TickType_t);

//Co2 Mocks

//DonwlinkHandler Mocks

//UplinkHandler Mocks


//IOT Setup
class TempHumTaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(tempHum_init);
		RESET_FAKE(tempHum_getDataFromTempHumSensorTask);
		RESET_FAKE(vTaskDelay);
		RESET_FAKE(xTaskDelayUntil);
		RESET_FAKE(hih8120_getHumidity);
		RESET_FAKE(hih8120_getTemperature);
		RESET_FAKE(hih8120_wakeup);
		//RESET_FAKE(xEventGroupSetBits);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};

//Co2 Setup

//DonwlinkHandler Setup

//UplinkHandler Setup



//IoT Testing
// 
//First test
TEST_F(TempHumTaskTest, FirstTest)
{
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

//Initalisation correct test
TEST_F(TempHumTaskTest, tempHum_init_calledDuringInitialisation) {
	tempHum_init();
	EXPECT_EQ(1, tempHum_init_fake.call_count);
}

//Get humidity test
TEST_F(TempHumTaskTest, TempHum_getHumididty_success) {
	//Arrange
	hih8120_getHumidity_fake.return_val = 0.0;
	//Act
	hih8120_getHumidity();
	//Assert
	EXPECT_EQ(0.0, humidity);
}

//Get temperature test
TEST_F(TempHumTaskTest, TempHum_getTemperature_success) {
	//Arrange
	hih8120_getTemperature_fake.return_val = 0.0;
	//Act
	hih8120_getTemperature();
	//Assert
	EXPECT_EQ(0.0, temperature);
}

//Get data from sensor test
TEST_F(TempHumTaskTest, TempHum_GetDataFromSensor_Succes) {
	//Arrange
	hih8120_getHumidity_fake.return_val = 0.0;
	hih8120_getTemperature_fake.return_val = 0.0;
	//Act
	tempHum_getDataFromTempHumSensorTask();
	//Assert
	EXPECT_EQ(humidity, hih8120_getHumidity_fake.call_count);
	EXPECT_EQ(temperature, hih8120_getTemperature_fake.call_count);
}


//Create task test
TEST_F(TempHumTaskTest, TempHUm_taskCreated_succes) {
	//Arrange
	
	//Act
	
	//Assert

}



