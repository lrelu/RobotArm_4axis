import RPi.GPIO as GPIO
import time
import mqtt_handler


class servo_handler:
    mqtt = mqtt_handler.mqtt_handler()
    robot_run = 0

    @classmethod
    def __init__(cls):
        cls.mqtt.subscribe()
        time.sleep(1)
        cls.mqtt.publish("connect robot arm")
        return

    @classmethod
    def dump(cls):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(2, GPIO.OUT)
        GPIO.setup(3, GPIO.OUT)
        GPIO.setup(4, GPIO.OUT)


        sv2 = GPIO.PWM(2, 50)
        sv3 = GPIO.PWM(3, 50)
        sv4 = GPIO.PWM(4, 50)

        sv2.start(7.5)
        sv3.start(7.5)
        sv4.start(7.5)

        sv2_motion = [7, 10, 10, 6, 7]
        sv3_motion = [7, 7, 10, 6, 11, 7]
        sv4_motion = [3, 8, 5, 4, 7, 3]

        try:
            while True:
                for i in range(0, len(sv2_motion)):
                    sv2.ChangeDutyCycle(sv2_motion[i])
                    sv3.ChangeDutyCycle(sv3_motion[i])
                    sv4.ChangeDutyCycle(sv4_motion[i])

                    time.sleep(1)

                cls.robot_run = cls.robot_run + 1
                cls.mqtt.publish("robot_run count: {0}".format(cls.robot_run))
                time.sleep(1)
        except KeyboardInterrupt:
            p.stop()
            GPIO.cleanup()


servo = servo_handler()
servo.dump()