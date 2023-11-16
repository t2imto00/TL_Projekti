import asyncio
import bleak
import struct
import mysql.connector

pos = 0
x = 0
y = 0
z = 0
counter = 0

mysql_config = {
    "host": "172.20.241.9",
    "user": "dbaccess_rw",
    "password": "fasdjkf2389vw2c3k234vk2f3",
    "database": "measurements",
}

mydb = mysql.connector.connect(**mysql_config)
mycursor = mydb.cursor()

nrf_address = "FD:08:D6:78:0D:5B"
characteristic_uuid = "00001526-1212-efde-1523-785feabcd123"

async def notification_handler(sender: int, data: bytearray):
    global pos
    global x
    global y
    global z
    global counter

    #print(f"Notification received from handle {sender}: {data}")
    value = struct.unpack('<I', data)[0]

    if value < 10:
        pos = value
        print(f"suunta: {pos}")
        counter += 1
    elif counter == 1:
        x = value
        print(f"x: {x}")
        counter += 1
    elif counter == 2:
        y = value
        print(f"y: {y}")
        counter += 1
    elif counter == 3:
        z = value
        print(f"z: {z}")
        sql = "INSERT INTO rawdata (timestamp, groupid, from_mac, to_mac, sensorvalue_a, sensorvalue_b, sensorvalue_c, sensorvalue_d) VALUES (NOW(), '04', %s, %s, %s, %s, %s, %s)"
        values = ("nrf5340", "raspi", pos, x, y, z)
        mycursor.execute(sql,values)
        mydb.commit()
        counter = 0    

    #sql = "DELETE FROM rawdata WHERE groupid=04"
    #sql = "INSERT INTO rawdata (timestamp, groupid, from_mac, to_mac, sensorvalue_a, sensorvalue_b, sensorvalue_c, sensorvalue_d) VALUES (NOW(), '04', %s, %s, %s, %s, %s, %s)"
    #values = ("nrf5340", "raspi", pos, x, y, z)

async def run():
    async with bleak.BleakClient(nrf_address) as client:
        # Check if the device is connected
        if not client.is_connected:
            print(f"Connecting to {nrf_address}...")
            await client.connect()
        '''
        # Print discovered services and characteristics
        for service in client.services:
            print(f"Service: {service.uuid}")
            for char in service.characteristics:
                print(f"  Characteristic: {char.uuid} - Properties: {char.properties}")
        '''
        # Subscribe to notifications for the characteristic
        await client.start_notify(characteristic_uuid, notification_handler)

        # Wait for notifications for a certain period (e.g., 10 seconds)
        await asyncio.sleep(5)

        # Stop notifications
        await client.stop_notify(characteristic_uuid)

        # Disconnect from the device
        await client.disconnect()
        print(f"Disconnected from {nrf_address}")

if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run())

    mydb.close()