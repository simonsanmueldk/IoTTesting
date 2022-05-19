#include "gtest/gtest.h"
#include "../../fff/fff.h"
DEFINE_FFF_GLOBALS

extern "C" {
#include <stdio.h>

	// The simulated FreeRTOS header files belonging to the test project
#include "FreeRTOS.h" 
#include "task.h"

	// Header file from the production code project
#include <myTask.h>
}

// --- Create mocks for FreeRTOS functions ---
// void vTaskDelay( const TickType_t xTicksToDelay );
FAKE_VOID_FUNC(vTaskDelay,  TickType_t);

class VIAFreeRTOSTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(vTaskDelay);
		FFF_RESET_HISTORY();
	}

	void TearDown() override {}
};

TEST_F(VIAFreeRTOSTest, vTaskDelay_called) {
	// Arrange
	// Act
	myTask_run();
	// Assert
	EXPECT_EQ(1, vTaskDelay_fake.call_count);
	EXPECT_EQ(pdMS_TO_TICKS(200), vTaskDelay_fake.arg0_val);
}
