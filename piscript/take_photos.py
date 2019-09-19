import io
import time
import picamera
import subprocess

def clean_file():
    print("cleaning")
    delete_files = subprocess.check_output(["find", "/home/pi/minitruckee/images", "-type", "f",  "-mmin", "+120", "-delete"])



with picamera.PiCamera() as camera:
    # Set the camera's resolution to VGA @40fps and give it a couple
    # of seconds to measure exposure etc.
    camera.resolution = (640, 480)
    camera.framerate = 80
    time.sleep(.5)
    i = 0
    print("go")
    clean_file()
    time.sleep(.1)
    while True:
        timestr = time.strftime("%m-%d--%H:%M:%S")                                                                                                                                                            
        file_str = "/home/pi/minitruckee/images/"+timestr+"{}.jpg".format(i)
        camera.capture(file_str)
        i = i + 1
