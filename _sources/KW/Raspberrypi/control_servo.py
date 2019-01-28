import RPi.GPIO as GPIO
import time

servo_pin = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(servo_pin, GPIO.OUT)

p = GPIO.PWM(servo_pin, 50)
p.start(7.5)

try:
    while True:
        p.ChangeDutyCycle(7.5)
        time.sleep(1)
        p.ChangeDutyCycle(10.5)
        time.sleep(1)
        p.ChangeDutyCycle(2.5)
        time.sleep(3)
except KeyboardInterrupt:
    p.stop()
    GPIO.cleanup()