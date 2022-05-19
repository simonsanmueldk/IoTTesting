#include "gtest/gtest.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

extern "C" {
#include <stdio.h>
	// Header file from the production code project
#include <MySecondTask.h>
}

// --- Create Mocks ---

// ---------------------------------------------------------------------
class MySecondTaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(xSemaphoreCreateBinary);
		RESET_FAKE(vTaskDelay);
		RESET_FAKE(xSemaphoreGive);
		RESET_FAKE(xQueueCreate);
		FFF_RESET_HISTORY();
	}

	void TearDown() override {}
};

// ---------------------------------------------------------------------
TEST_F(MySecondTaskTest, InitialisationIsCorrect) {
	// Arrange
	// Act
	mySecondTask_initialise();
	// Assert
	EXPECT_EQ(1, xSemaphoreCreateBinary_fake.call_count);
	EXPECT_EQ(1, xQueueCreate_fake.call_count);
	EXPECT_EQ(2, xQueueCreate_fake.arg0_val);
	EXPECT_EQ(10, xQueueCreate_fake.arg1_val);
}


// ---------------------------------------------------------------------
TEST_F(MySecondTaskTest, InitIsCorrectFromTask) {
	// Arrange
	// Act
	mySecondTask_init();
	// Assert
	EXPECT_EQ(1, xTaskGetTickCount_fake.call_count);  // Called once?
}


// ---------------------------------------------------------------------
TEST_F(MySecondTaskTest, runIsCorrectFromTask) {
	// Arrange
	// Act
	mySecondTask_run();
	// Assert
	EXPECT_EQ(1, vTaskDelay_fake.call_count);
	// Argument correct?
	EXPECT_EQ(pdMS_TO_TICKS(200), vTaskDelay_fake.arg0_val);

	EXPECT_EQ(1, xSemaphoreGive_fake.call_count);

	EXPECT_EQ(1, xQueueSend_fake.call_count);
	// Arguments correct?
	EXPECT_EQ(portMAX_DELAY, xQueueSend_fake.arg2_val);
}
