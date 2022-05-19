//#include "pch.h"
#include "gtest/gtest.h"
#include "C:\Users\simon\New SEP\Testing\fff-master\fff.h"

DEFINE_FFF_GLOBALS

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <TempHumSensor.h>

}
//Mocks 
FAKE_VOID_FUNC(tempHum_init);
FAKE_VOID_FUNC(tempHum_taskRun);


//Create mocks FreeRTOS
FAKE_VOID_FUNC(vTaskDelay, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xSempahoreGive, SemaphoreHandle_t);
//------------------------

class TempHumTaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(tempHum_init);
		RESET_FAKE(tempHum_taskRun);
		RESET_FAKE(vTaskDelay);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};

//------------------------------------------------------

TEST_F(TempHumTaskTest, tempHum_init_calledDuringInitialisation) {
	tempHum_init();
	EXPECT_EQ(1, tempHum_init_fake.call_count);
}


TEST_F(TempHumTaskTest, vTaskDelay_calledCorrectFromTask) {
	tempHum_taskRun();
	EXPECT_EQ(1, vTaskDelay_fake.call_count);
	EXPECT_EQ(pdMS_TO_TICKS(200), vTaskDelay_fake.arg0_val);
}
