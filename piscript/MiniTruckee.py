import time
import serial
import os
import subprocess
import xbox
#initialize joystick
joy = xbox.Joystick()
# initialize serial port on arduino
ser = serial.Serial('/dev/ttyACM0',115200, timeout = 1)
time.sleep(2)
print("were live")

def write(str):
    ser.write(str)


def controller_test(old_x_string, old_y_string, old_button_string):
    # A = 1, B = 2, X = 3, Y = 4
    button_string = ""
    x_int = 50
    y_int = 50
    x_string = ""
    y_string = ""
    
    position = joy.leftStick()
    x = position[0]
    y = position[1]
    
    x_int = round(x*(50)+50)
    x_string = "x"+str(x_int)+">"    
    y_int = round(y*50+50)
    y_string = "y" + str(y_int)+">"
    

    #set button input
    if joy.A() == True: 
        button_string = "b1>"
    elif joy.B() == True: 
        button_string = "b2>"
    elif joy.X() == True: 
        button_string = "b3>"
    elif joy.Y() == True: 
        button_string = "b4>"


    #compare current input to last input
    #only send if input is different and not null
    if button_string != old_button_string:
        if button_string:
            write(button_string.encode())
            print("new button string detected. sending "+button_string)
        old_button_string = button_string
    if x_string != old_x_string:
        print("new x string detected. sending "+x_string)
        write(x_string.encode())
        old_x_string = x_string
    if y_string != old_y_string:
        print("new y string detected. sending "+y_string)
        write(y_string.encode())
        old_y_string = y_string
        
    # create tuple of old values to return 
    old_values = (old_x_string, old_y_string, old_button_string)
    return old_values
    

str1 = ""
str2 = ""
str3 = ""
while(True):
    str1, str2, str3 = controller_test(str1, str2, str3)
    #