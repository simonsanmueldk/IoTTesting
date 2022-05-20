//#include "pch.h"
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
//Mocks 
FAKE_VOID_FUNC(tempHum_init);
FAKE_VOID_FUNC(tempHum_taskRun);
FAKE_VALUE_FUNC(float, hih8120_getHumidity);
FAKE_VALUE_FUNC(float, hih8120_getTemperature);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);




FAKE_VOID_FUNC(vTaskDelay, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xSempahoreGive, SemaphoreHandle_t);
FAKE_VOID_FUNC(xTaskDelayUntil, TickType_t*, TickType_t);

//------------------------

class TempHumTaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(tempHum_init);
		RESET_FAKE(tempHum_taskRun);
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

//------------------------------------------------------

TEST_F(TempHumTaskTest, tempHum_init_calledDuringInitialisation) {
	tempHum_init();
	EXPECT_EQ(1, tempHum_init_fake.call_count);
}

TEST_F(TempHumTaskTest, FirstTest)
{
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST_F(TempHumTaskTest, TempHum_getHumididty_success) {
	//Arrange
	hih8120_getHumidity_fake.return_val = 0.0;
	//Act
	tempHum_taskRun();
	//Assert
	EXPECT_EQ(0.0, hih8120_getHumidity_fake.call_count);
	EXPECT_EQ(0.0, humidity);
}


TEST_F(TempHumTaskTest, TempHum_getTemperature_success) {
	//Arrange
	hih8120_getTemperature_fake.return_val = 0.0;
	//Act
	tempHum_taskRun();
	//Assert
	EXPECT_EQ(0.0, hih8120_getTemperature_fake.call_count);
	EXPECT_EQ(0.0, temperature);
}


TEST_F(TempHumTaskTest, TempHum_xTaskDelayUntil_calledCorrectly) {
	//Arrange
	//Act
	tempHum_taskRun();
	//Assert
	EXPECT_EQ(1, xTaskDelayUntil_fake.call_count);
	EXPECT_TRUE(NULL != xTaskDelayUntil_fake.arg0_val);
}

TEST_F(TempHumTaskTest, TempHum_vTaskDelay_calledCorrectly) {
	//Arrange
	//Act
	tempHum_taskRun();
	//Assert
	EXPECT_EQ(2, vTaskDelay_fake.call_count);
	EXPECT_EQ(pdMS_TO_TICKS(50), vTaskDelay_fake.arg0_history[0]);
	EXPECT_EQ(pdMS_TO_TICKS(500), vTaskDelay_fake.arg0_history[1]);
}


TEST_F(TempHumTaskTest, TempHum_driver_wakeup_success) {
	//Arrange
	hih8120_wakeup_fake.return_val = HIH8120_OK;
	//Act
	tempHum_taskRun();
	//Assert
	EXPECT_EQ(1, hih8120_wakeup_fake.call_count);
	EXPECT_EQ(HIH8120_OK, hih8120_wakeup_fake.return_val);
}

