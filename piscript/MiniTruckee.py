import time
import signal
import serial
import os
import subprocess
import xbox
# Set subprocess variables for vid and photo script
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

#function that writes to arduino over serial port given a string
def write(str):
    ser.write(str)
    print("write")

#function that starts and stops the live video script depending on what bool value is passed in
def vid_open(open_close_bool):
    global proc1
    #opens vid script
    if open_close_bool and not video_script:
        proc1 = subprocess.Popen(['sudo', 'python3.7', '/home/pi/minitruckee/piscript/cam_test.py'], preexec_fn=os.setsid)
        time.sleep(.8)
    #closes vid script
    if not open_close_bool and video_script:
        os.killpg(os.getpgid(proc1.pid), signal.SIGTERM)
        time.sleep(.8)

#function that starts and stops the photo taking script depending on what bool is passed in
def photo_open(open_close_bool):
    global proc2
    # if true, start script
    if open_close_bool:
        proc2 = subprocess.Popen(['sudo', 'python3.7', '/home/pi/minitruckee/piscript/take_photos.py'], preexec_fn=os.setsid)
        time.sleep(.8)
    # if false, stop script
    if not open_close_bool:
        os.killpg(os.getpgid(proc2.pid), signal.SIGTERM)
        time.sleep(.8)



#controller input function, takes in arg of most recent direction and most recent button pressed
def controller_test(old_dir_string, old_button_string):
    # A = 1, B = 2, X = 3, Y = 4
    button_string = ""
    dir_string = ""
    global video_script
    global photo_script
    # get position from joystick in x and y value
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
    # string starts with 'd' or 'b' for direction or button, has an int value for the direction or button id, 
    # and ends with > so arduino knows easily when string ends

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
    # If button A is pressed
        # start video script if its currently off and photo script not running
        if not video_script and not photo_script:
            print("starting video script")
            vid_open(True)
            video_script = True
        # if video script already running, stop it
        elif video_script:
            vid_open(False)
            video_script = False
        
    elif joy.B() == True: 
        # if button is B, send this string to arduino
        button_string = "b2>"
    elif joy.X() == True:
        #if X button is pressed, script will end
        # quit camera subprocesses if running
        if photo_script:
            photo_open(False)
        if video_script:
            vid_open(False)
        quit()    
    elif joy.Y() == True: 
        # if Y is pressed
        # if photo and video scripts arent running, start photo script
        if not photo_script and not video_script:
            print("starting photo script")
            photo_open(True)
            photo_script = True
        # if photo script already running, stop it
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
#constantly call controller_test to always be ready to get input from controller
# receive most recent values and then pass them back in
while(True):
    str1, str2  = controller_test(str1, str2)
