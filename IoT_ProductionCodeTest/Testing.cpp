/*
 * Testing.cpp
 *
 * Created: 19/05/2022 12.49.21
 * Authors: Simon Samuel & Junjie Chen
 * Simon: Responisble for UpLinkHandler tests and TempHumSensorImpl tests
 * Chen: Responisble for DownLinkHandler tests and CO2_SensorImpl tests
 */

#include "gtest/gtest.h"
#include "C:\Users\simon\New SEP\Testing\fff-master\fff.h"

DEFINE_FFF_GLOBALS
extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "message_buffer.h"
#include <event_groups.h>

uint16_t co2_data;

//Drivers included
#include "hih8120.h"
#include "mh_z19.h"

}
//Temperature humidity mocks 
FAKE_VOID_FUNC(tempHum_init);
FAKE_VOID_FUNC(tempHum_Task);
FAKE_VALUE_FUNC(uint16_t, get_humidity_data);
FAKE_VALUE_FUNC(uint16_t, get_temperature_data);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_measure);

//FreeRTOS mocks
FAKE_VOID_FUNC(vTaskDelay, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xSempahoreGive, SemaphoreHandle_t);
FAKE_VOID_FUNC(xTaskDelayUntil, TickType_t*, TickType_t);
FAKE_VOID_FUNC(xMessageBufferReceive);
FAKE_VOID_FUNC(xEventGroupWaitBits);

//Co2 Mocks
FAKE_VOID_FUNC(CO2_sensor_create);
FAKE_VOID_FUNC(myCo2CallBack);
FAKE_VALUE_FUNC(uint16_t, get_CO2_data);
FAKE_VOID_FUNC(CO2_taskRun);
FAKE_VOID_FUNC(CO2_Sensor_Task);

//DonwlinkHandler Mocks
FAKE_VOID_FUNC(lora_DownLinkHandler_create);
FAKE_VOID_FUNC(lora_DownLinkHandler_task);
FAKE_VOID_FUNC(lora_DownLinkHandler_startTask);

//UplinkHandler Mocks
FAKE_VOID_FUNC(upLinkHandler_StartTask);
FAKE_VOID_FUNC(upLink_create);
FAKE_VOID_FUNC(lora_handler_task);
FAKE_VOID_FUNC(send);

//IOT Setup
class TempHumTaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(tempHum_init);
		RESET_FAKE(tempHum_Task);
		RESET_FAKE(get_humidity_data);
		RESET_FAKE(get_temperature_data);
		RESET_FAKE(hih8120_wakeup);
		RESET_FAKE(hih8120_measure);
		RESET_FAKE(xEventGroupWaitBits);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};

//Co2 setup
class CO2TaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(CO2_sensor_create);
		RESET_FAKE(myCo2CallBack);
		RESET_FAKE(CO2_taskRun);
		RESET_FAKE(CO2_Sensor_Task);
		RESET_FAKE(get_CO2_data);
		RESET_FAKE(vTaskDelay);
		FFF_RESET_HISTORY();

	}
	void TearDown() override {}
};


//DonwlinkHandler Setup
class DownLinkHandlerTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(lora_DownLinkHandler_create);
		RESET_FAKE(lora_DownLinkHandler_task);
		RESET_FAKE(lora_DownLinkHandler_startTask);
		RESET_FAKE(xMessageBufferReceive);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};

//UplinkHandler Setup
class UplinkHandlerTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(upLinkHandler_StartTask);
		RESET_FAKE(upLink_create);
		RESET_FAKE(lora_handler_task);
		RESET_FAKE(send);
		RESET_FAKE(xMessageBufferReceive);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};

//----------------------------------------------------Temperature humidity Testing------------------------------------------


//Get humidity test
TEST_F(TempHumTaskTest, TempHum_getHumididty_success) {
	get_humidity_data_fake.return_val = 25.0;
	get_humidity_data();
	ASSERT_EQ(25.0, get_humidity_data_fake.return_val);
}

//Get temperature test
TEST_F(TempHumTaskTest, TempHum_getTemperature_success) {
	get_temperature_data_fake.return_val = 30.0;
	get_temperature_data();
	ASSERT_EQ(30.0, get_temperature_data_fake.return_val);
}

TEST_F(TempHumTaskTest, driver_wakeup_called_when_HIH8120_OK) {
	int driverState = HIH8120_OK;
	tempHum_Task();
	ASSERT_EQ(driverState, hih8120_wakeup());

}

TEST_F(TempHumTaskTest, EventGroupSet_called_correctly_when_HIH8120_OK) {
	//Arrange
	int driverState = HIH8120_OK;
	//Act
	tempHum_Task();
	ASSERT_EQ(driverState, hih8120_measure());
}

//---------------------------------------------------------Uplink Testing---------------------------------------------------
TEST_F(UplinkHandlerTest, UpLinkHandler_create_IsCalled_test)
{
	upLink_create();
	EXPECT_EQ(1, upLink_create_fake.call_count);
}

TEST_F(UplinkHandlerTest, Lora_handler_task_IsCalled_test)
{
	lora_handler_task();
	EXPECT_EQ(1, lora_handler_task_fake.call_count); 

}

TEST_F(UplinkHandlerTest, more_than_zeroBytes_sent)
{
	size_t xBytesSent;
	xBytesSent = 5;
	send();
	ASSERT_TRUE(xBytesSent, xMessageBufferReceive);
}

TEST_F(UplinkHandlerTest, zeroBytes_sent)
{
	size_t xBytesSent;
	xBytesSent = 0;
	send();
	ASSERT_FALSE(xBytesSent, xMessageBufferReceive);	
}

TEST_F(UplinkHandlerTest, UpLinkHandler_startTask_IsCalled_test)
{
	upLinkHandler_StartTask();
	EXPECT_EQ(1, upLinkHandler_StartTask_fake.call_count);
}

//---------------------------------------------------------CO2 Test--------------------------------------------------------
//CO2Test

TEST_F(CO2TaskTest, CO2_sensor_create_test)
{
	CO2_sensor_create();
	EXPECT_TRUE(1, CO2_sensor_create_fake.call_count);
}

TEST_F(CO2TaskTest, getCO2Data_test)
{
	get_CO2_data_fake.return_val = 5.0;
	get_CO2_data();
	EXPECT_EQ(get_CO2_data_fake.return_val, 5.0);	
}

TEST_F(CO2TaskTest, getCO2DataFromSensor_test)
{
	get_CO2_data_fake.return_val = 1.0;
	CO2_Sensor_Task();

	EXPECT_EQ(co2_data, get_CO2_data_fake.call_count);
}
//------------------------------------------------------Downlink test-------------------------------------------------------
TEST_F(DownLinkHandlerTest, lora_DownLinkHandler_create_test)
{
	lora_DownLinkHandler_create();
	EXPECT_EQ(1, lora_DownLinkHandler_create_fake.call_count);
}

TEST_F(DownLinkHandlerTest, lora_DownLinkHandler_task_test)
{
	size_t xBytesRecieved;
	xBytesRecieved = 5;
	lora_DownLinkHandler_startTask();
	ASSERT_TRUE(xBytesRecieved, xMessageBufferReceive);;
}

TEST_F(DownLinkHandlerTest, lora_DownLinkHandler_startTask_test)
{
	size_t xBytesRecieved;
	xBytesRecieved = 0;
	lora_DownLinkHandler_startTask();
	ASSERT_FALSE(xBytesRecieved, xMessageBufferReceive);;
}
