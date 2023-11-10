/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include <dk_buttons_and_leds.h>
#include "my_lbs.h"
#include "adc.h"

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
	(BT_LE_ADV_OPT_CONNECTABLE |
	 BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
	800, /* Min Advertising Interval 500ms (800*0.625ms) */
	801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
	NULL); /* Set to NULL for undirected advertising */

LOG_MODULE_REGISTER(Lesson4_Exercise2, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#define RUN_STATUS_LED DK_LED1
#define CON_STATUS_LED DK_LED2
#define USER_LED DK_LED3
#define USER_BUTTON DK_BTN1_MSK
#define USER_BUTTON DK_BTN2_MSK

#define DIRECTION_COUNT 6
#define DIRECTION_UP 1
#define DIRECTION_DOWN -1

#define STACKSIZE 1024
#define PRIORITY 7

#define RUN_LED_BLINK_INTERVAL 1000
/* STEP 17 - Define the interval at which you want to send data at */
#define NOTIFY_INTERVAL 500
static bool app_button_state;
/* STEP 15 - Define the data you want to stream over Bluetooth LE */
static uint32_t app_sensor_value = 100;
static int direction = DIRECTION_UP;
static int value = 0;
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_LBS_VAL),
};

/* STEP 16 - Define a function to simulate the data */
/*
static void simulate_data(void)
{
	app_sensor_value++;
	if (app_sensor_value == 200) {
		app_sensor_value = 100;
	}
}
*/
static void app_led_cb(bool led_state)
{
	dk_set_led(USER_LED, led_state);
}

static bool app_button_cb(void)
{
	return app_button_state;
}

/* STEP 18.1 - Define the thread function  */
void send_data_thread(void)
{
	while (1) {
		/* Simulate data */
		//simulate_data();
		/* Send notification, the function sends notifications only if a client is subscribed */
		my_lbs_send_sensor_notify(value);

		struct Measurement m = readADCValue();

		my_lbs_send_sensor_notify(m.x);
		my_lbs_send_sensor_notify(m.y);
		my_lbs_send_sensor_notify(m.z);

		k_sleep(K_MSEC(NOTIFY_INTERVAL));
		//k_sleep(K_MSEC(1000));
	}
}

static struct my_lbs_cb app_callbacks = {
	.led_cb = app_led_cb,
	.button_cb = app_button_cb,
};



static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	//printk("button state = %d\n", button_state);
	//printk("has changed = %d\n", has_changed);
	/*
	if ((has_changed & USER_BUTTON)&&(button_state & USER_BUTTON)) {
		uint32_t user_button_state = button_state & USER_BUTTON;
		my_lbs_send_button_state_indicate(user_button_state);
		app_button_state = user_button_state ? true : false;
		printk("nappi 1 painettu\n");
	}
	*/

	if ((has_changed & USER_BUTTON)&&(button_state & USER_BUTTON)) {
		uint32_t user_button_state = button_state & USER_BUTTON;
		my_lbs_send_button_state_indicate(user_button_state);
		app_button_state = user_button_state ? true : false;
		printk("nappi 2 painettu\n");
		
		value += direction;
		if (value >= DIRECTION_COUNT)
		{
			value = 0;
		}
		else if (value < 0)
		{
			value = DIRECTION_COUNT - 1;
		}
		printk("Arvo: %d\n", value);

		if(value == 0)
		{
			printk("0 = x-akseli maata kohti\n");
			struct Measurement m = readADCValue();
			printk("x = %d,  y = %d,  z = %d\n",m.x,m.y,m.z);
		
		k_sleep(K_MSEC(1000));
		}
		else if(value == 1)
		{
			printk("1 = x-akseli taivasta kohti\n");
			struct Measurement m = readADCValue();
			printk("x = %d,  y = %d,  z = %d\n",m.x,m.y,m.z);
		}
		else if(value == 2)
		{
			printk("2 = y-akseli maata kohti\n");
			struct Measurement m = readADCValue();
			printk("x = %d,  y = %d,  z = %d\n",m.x,m.y,m.z);
		}
		else if(value == 3)
		{
			printk("3 = y-akseli taivasta kohti\n");
			struct Measurement m = readADCValue();
			printk("x = %d,  y = %d,  z = %d\n",m.x,m.y,m.z);
		}
		else if(value == 4)
		{
			printk("4 = z-akseli maata kohti\n");
			struct Measurement m = readADCValue();
			printk("x = %d,  y = %d,  z = %d\n",m.x,m.y,m.z);
		}
		else if(value == 5)
		{
			printk("3 = z-akseli taivasta kohti\n");
			struct Measurement m = readADCValue();
			printk("x = %d,  y = %d,  z = %d\n",m.x,m.y,m.z);
		}
		}
}
static void on_connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
		return;
	}

	printk("Connected\n");

	dk_set_led_on(CON_STATUS_LED);
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);

	dk_set_led_off(CON_STATUS_LED);
}

struct bt_conn_cb connection_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected,
};

static int init_button(void)
{
	int err;

	err = dk_buttons_init(button_changed);
	if (err) {
		printk("Cannot init buttons (err: %d)\n", err);
	}

	return err;
}

void main(void)
{
	int blink_status = 0;
	int err;

	LOG_INF("Starting Lesson 4 - Exercise 2 \n");

	if(initializeADC() != 0)
	{
	printk("ADC initialization failed!");
	return;
	}

	err = dk_leds_init();
	if (err) {
		LOG_ERR("LEDs init failed (err %d)\n", err);
		return;
	}

	err = init_button();
	if (err) {
		printk("Button init failed (err %d)\n", err);
		return;
	}

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return;
	}
	bt_conn_cb_register(&connection_callbacks);

	err = my_lbs_init(&app_callbacks);
	if (err) {
		printk("Failed to init LBS (err:%d)\n", err);
		return;
	}
	LOG_INF("Bluetooth initialized\n");
	err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return;
	}

	LOG_INF("Advertising successfully started\n");
	for (;;) {
		dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
	}
	/*
	while (1) 
	{
		struct Measurement m = readADCValue();
		printk("x = %d,  y = %d,  z = %d\n",m.x,m.y,m.z);
		
		k_sleep(K_MSEC(1000));
	}
	*/
}
/* STEP 18.2 - Define and initialize a thread to send data periodically */
K_THREAD_DEFINE(send_data_thread_id, STACKSIZE, send_data_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
