import time
import signal
import serial
import os
import subprocess
import xbox
proc1 = None
proc2 = None
#initialize joystick
joy = xbox.Joystick()
# initialize serial port on arduino
ser = serial.Serial('/dev/ttyACM0',115200, timeout = 1)
#time.sleep(2)
video_script = False;
photo_script = False;
cam_script = False;
print("were live")

def write(str):
    ser.write(str)
    print("write")

def vid_open(open_close_bool):
    global proc1
    if open_close_bool and not video_script:
        proc1 = subprocess.Popen(['sudo', 'python3.7', '/home/pi/minitruckee/piscript/cam_test.py'], preexec_fn=os.setsid)
        time.sleep(.8)
    if not open_close_bool and video_script:
        os.killpg(os.getpgid(proc1.pid), signal.SIGTERM)
        time.sleep(.8)

def photo_open(open_close_bool):
    global proc2
    if open_close_bool:
        proc2 = subprocess.Popen(['sudo', 'python3.7', '/home/pi/minitruckee/piscript/take_photos.py'], preexec_fn=os.setsid)
        time.sleep(.8)
    if not open_close_bool:
        os.killpg(os.getpgid(proc2.pid), signal.SIGTERM)
        time.sleep(.8)



 
def controller_test(old_dir_string, old_button_string):
    # A = 1, B = 2, X = 3, Y = 4
    button_string = ""
    dir_string = ""
    global video_script
    global photo_script
    position = joy.leftStick()
    x = position[0]
    y = position[1]
    # dir values:
    # 0 = no movement
    # 1 = forward left
    # 2 = forward straight
    # 3 = f right
    # 4 = b left
    # 5 = b str
    # 6 = b r

    if y == 0:
        dir_string = "d0>"
    elif y > 0:
        if x < 0:
            dir_string  = "d1>"
        elif x ==0:
            dir_string = "d2>"
        elif x > 0:
            dir_string = "d3>"
    elif y < 0:
        if x < 0:
            dir_string = "d4>"
        elif x == 0:
            dir_string = "d5>"
        elif x > 0:
            dir_string = "d6>"

    #set button input
    if joy.A() == True:
        if not video_script and not photo_script:
            print("starting video script")
            vid_open(True)
            video_script = True
        elif video_script:
            vid_open(False)
            video_script = False
        #button_string = "b1>"
    elif joy.B() == True: 
        button_string = "b2>"
    elif joy.X() == True:
        if photo_script:
            photo_open(False)
        if video_script:
            vid_open(False)
        quit()    
    elif joy.Y() == True: 
        if not photo_script and not video_script:
            print("starting camera script")
            photo_open(True)
            photo_script = True
        elif photo_script:
            photo_open(False)
            photo_script = False
        
    #compare current input to last input
    #only send if input is different and not null
    if button_string != old_button_string:
        if button_string:
            write(button_string.encode())
            print("new button string detected. sending "+button_string)
        old_button_string = button_string
    if dir_string != old_dir_string:
        print("new dir_string detected. sending "+dir_string)
        write(dir_string.encode())
        old_dir_string = dir_string
          
    # create tuple of old values to return 
    old_values = (old_dir_string, old_button_string)
    return old_values
    

str1 = ""
str2 = ""
while(True):
    str1, str2  = controller_test(str1, str2)
