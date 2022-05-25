/*
 * Testing.cpp
 *
 * Created: 18/05/2022 12.49.21
 *  Author: Simon Samuel & Chen
 */

#include "gtest/gtest.h"
#include "C:\Users\simon\New SEP\Testing\fff-master\fff.h"

DEFINE_FFF_GLOBALS

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "message_buffer.h"
#include <TempHumSensor.h>
#include <event_groups.h>
//#include <serial.h>

uint16_t co2_data;

uint16_t fakeTemperature = 30;
uint16_t fakeHumidity = 25;


//Drivers included
#include "hih8120.h"
#include "mh_z19.h"
}
//Temperature humidity mocks 
FAKE_VOID_FUNC(tempHum_init);
FAKE_VOID_FUNC(tempHum_getDataFromTempHumSensorTask);
FAKE_VALUE_FUNC(uint16_t, get_humidity_data);
FAKE_VALUE_FUNC(uint16_t, get_temperature_data);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);

//FreeRTOS mocks
FAKE_VOID_FUNC(vTaskDelay, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xSempahoreGive, SemaphoreHandle_t);
FAKE_VOID_FUNC(xTaskDelayUntil, TickType_t*, TickType_t);
FAKE_VOID_FUNC(xMessageBufferReceive);
FAKE_VOID_FUNC(xEventGroupWaitBits);


//Co2 Mocks
FAKE_VOID_FUNC(CO2_sensor_create);
FAKE_VOID_FUNC(myCo2CallBack);
FAKE_VALUE_FUNC(float, mh_z19_get_CO2_data);
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



//IOT Setup
class TempHumTaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(tempHum_init);
		RESET_FAKE(tempHum_getDataFromTempHumSensorTask);
		RESET_FAKE(get_humidity_data);
		RESET_FAKE(get_temperature_data);
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
		RESET_FAKE(vTaskDelay);
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
		RESET_FAKE(vTaskDelay);
		RESET_FAKE(xMessageBufferReceive);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};


//--Temperature humidity Testing

//Initalisation correct test
TEST_F(TempHumTaskTest, tempHum_init_calledDuringInitialisation) {
	tempHum_init();
	ASSERT_EQ(1, tempHum_init_fake.call_count);
}

//Get humidity test
TEST_F(TempHumTaskTest, TempHum_getHumididty_success) {
	//Arrange
	get_humidity_data_fake.return_val = 25.0;
	//Act
	get_humidity_data();
	//Assert
	ASSERT_EQ(25.0, fakeHumidity);
}

//Get temperature test
TEST_F(TempHumTaskTest, TempHum_getTemperature_success) {
	//Arrange
	get_temperature_data_fake.return_val = 30.0;
	//Act
	get_temperature_data();
	//Assert
	ASSERT_EQ(30.0, fakeTemperature);
}


//Get data from sensor test
TEST_F(TempHumTaskTest, TempHum_GetDataFromSensor_Succes) {
	//Arrange
	get_humidity_data_fake.return_val = 25.0;
	get_temperature_data_fake.return_val = 30.0;
	//Act
	tempHum_getDataFromTempHumSensorTask();
	//Assert
	ASSERT_EQ(xTaskDelayUntil_fake.call_count, 4);
	ASSERT_EQ(xEventGroupWaitBits_fake.call_count, 1);

	EXPECT_EQ(fakeHumidity, get_humidity_data_fake.return_val);
	EXPECT_EQ(fakeTemperature, get_temperature_data_fake.return_val);
}
//---------------------------------------------------------Uplink Testing---------------------------------------------------
TEST_F(UplinkHandlerTest, UpLinkHandler_create_IsCalled_test)
{
	//Arrange
	//Act
	upLink_create();
	//Assert
	EXPECT_EQ(1, upLink_create_fake.call_count);
}

TEST_F(UplinkHandlerTest, Lora_handler_task_IsCalled_test)
{
	lora_handler_task();
	
	EXPECT_EQ(1, lora_handler_task_fake.call_count); //Method runs?
	//ASSERT_EQ(1, vTaskDelay_fake.call_count); //Task delay called 1 time?
	//ASSERT_EQ(1, xMessageBufferReceive_fake.call_count); //Message buffer recieved
}

TEST_F(UplinkHandlerTest, more_than_zeroBytes_sent)
{
	size_t xBytesSent;
	xBytesSent = 5;
	lora_handler_task();
	ASSERT_TRUE(xBytesSent, xMessageBufferReceive);
}

TEST_F(UplinkHandlerTest, zeroBytes_sent)
{
	size_t xBytesSent;
	xBytesSent = 0;
	lora_handler_task();
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
	mh_z19_get_CO2_data_fake.return_val = 0.0;
	mh_z19_get_CO2_data();
	EXPECT_EQ(0.0, co2_data);
}

TEST_F(CO2TaskTest, getCO2DataFromSensor_test)
{
	mh_z19_get_CO2_data_fake.return_val = 0.0;
	CO2_Sensor_Task();

	//EXPECT_EQ(co2_data, mh_z19_get_CO2_data_fake.call_count);
}
//------------------------------------------------------Downlink test-------------------------------------------------------
TEST_F(DownLinkHandlerTest, lora_DownLinkHandler_create_test)
{
	lora_DownLinkHandler_create();
	EXPECT_EQ(1, lora_DownLinkHandler_create_fake.call_count);
}

TEST_F(DownLinkHandlerTest, lora_DownLinkHandler_task_test)
{
	lora_DownLinkHandler_task();
	EXPECT_EQ(1, lora_DownLinkHandler_task_fake.call_count);
}

TEST_F(DownLinkHandlerTest, lora_DownLinkHandler_startTask_test)
{
	lora_DownLinkHandler_startTask();
	EXPECT_EQ(1, lora_DownLinkHandler_startTask_fake.call_count);
}
