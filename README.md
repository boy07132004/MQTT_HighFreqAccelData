# DualESP32_with_MPU9250

Stably get the 1KHz accelerometer data by MQTT.

## Wiring 

<table>
    <tr>
        <td width="50%">
            <table>
                <tr>
                    <td align="center">ESP32_sensor</td>
                    <td align="center">Connect Pin</td>
                </tr>
                <tr>
                    <td align="center">3V3</td>
                    <td align="center">MPU9250 VCC</td>
                </tr>
                <tr>
                    <td align="center">GND</td>
                    <td align="center">MPU9250 GND<br> ESP32_MQTT GND</td>
                </tr>
                <tr>
                    <td align="center">GPIO 19</td>
                    <td align="center">MPU9250 SDO</td>
                </tr>
                <tr>
                    <td align="center">GPIO 18</td>
                    <td align="center">MPU9250 CL</td>
                </tr>
                <tr>
                    <td align="center">GPIO 05</td>
                    <td align="center">MPU9250 NCS</td>
                </tr>
                <tr>
                    <td align="center">GPIO 17<br> (Serial2 TX)</td>
                    <td align="center">ESP32_MQTT RX2<br> (Serial2 RX)</td>
                </tr>
            </table>
        </td>
        <td width="50%" height="100%">
            <img src="https://github.com/boy07132004/DualESP32_with_MPU9250/blob/main/Wiring.png" height="40%">
        </td>
    </tr>
</table>

