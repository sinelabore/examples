#!/usr/bin/env python3.9
import tkinter as tk
import sys     
from oven import *
from timer import *
from radiator import *
import socket

class myoven(oven):
    
    def __init__(self, tm, rad):
        self.radiator = rad
        self.tm = tm
        super().__init__(self)



class Application(tk.Tk):             
    def __init__(self, *args, **kwargs):
        
        self.tm = timer()
        self.aRadiator = radiator()
        self.myoven = myoven(self.tm, self.aRadiator)
        
        tk.Tk.__init__(self, *args, **kwargs)
 
        self.root = tk.Frame(self)
        self.root.pack(side="top", fill="both", expand=True)
 
 
        self.createWidgets()
        self.bind_all('<Any-KeyPress>', self.handlerName)
        
        # Create a UDP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_address = ('localhost', 4445)
        
 
    def createWidgets(self):
       
        self.buttonFrame = tk.Frame(self.root)
        self.buttonFrame.pack(side=tk.RIGHT)
        
        self.quit = tk.Button(self.buttonFrame, text='Quit', command=self.quit).pack(side=tk.TOP, expand=True, fill="both")
        self.close_btn = tk.Button(self.buttonFrame, text='Close', command=self.close).pack(side=tk.TOP, expand=True, fill="both")
        #self.close_btn_wguard = tk.Button(self.buttonFrame, text='Close with guard', command=self.close_wg).pack(side=tk.TOP, expand=True, fill="both")
        self.open_btn = tk.Button(self.buttonFrame, text='Open', command=self.open).pack(side=tk.TOP, expand=True, fill="both")
        self.inc_btn = tk.Button(self.buttonFrame, text='Inc', command=self.inc).pack(side=tk.TOP, expand=True, fill="both")
        self.dec_btn = tk.Button(self.buttonFrame, text='Dec', command=self.dec).pack(side=tk.TOP, expand=True, fill="both")
        #self.timeout = tk.Button(self.buttonFrame, text='Timeout', command=self.timeout).pack(side=tk.TOP, expand=True, fill="both")
        
        self.v = tk.IntVar()
        self.pwr_high = tk.Radiobutton(self.buttonFrame, text='PwrHigh', command=self.high, variable=self.v, value=1).pack(anchor=tk.W)
        self.pwr_low = tk.Radiobutton(self.buttonFrame, text='PwrLow', command=self.low, variable=self.v, value=2).pack(anchor=tk.W)
        
        self.door_open_img = tk.PhotoImage(file="oven_open.gif")
        self.door_closed_img = tk.PhotoImage(file="oven_closed.gif")
        self.door_label = tk.Label(self.root, image=self.door_open_img)
        self.door_label.pack(side=tk.LEFT)
        
        # initial time display
        self.onUpdate()
   
    def inc(self):
        self.sock.sendto("evInc".encode('utf-8'), self.server_address)
        self.myoven.processEvent("evInc")
   
    def high(self):
        self.sock.sendto("evPwrHigh".encode('utf-8'), self.server_address)
        self.myoven.processEvent("evPwrHigh")
   
    def low(self):
        self.sock.sendto("evPwrLow".encode('utf-8'), self.server_address)
        self.myoven.processEvent("evPwrLow")
   
    def dec(self):
        self.sock.sendto("evDec".encode('utf-8'), self.server_address)
        self.myoven.processEvent("evDec")
       
    def open(self):
        self.sock.sendto("evDoorOpen".encode('utf-8'), self.server_address)
        self.door_label.configure(image = self.door_open_img)
        self.myoven.processEvent("evDoorOpen")
 
    def close(self):
        self.sock.sendto("evDoorClosed".encode('utf-8'), self.server_address)
        self.door_label.configure(image = self.door_closed_img)
        self.myoven.processEvent("evDoorClosed")

    def close_wg(self):
        self.sock.sendto("evDoorClosed[self.tm.preset()>0]".encode('utf-8'), self.server_address)
        self.door_label.configure(image = self.door_closed_img)
        self.myoven.processEvent("evDoorClosed")
    
    def timeout(self):
        self.sock.sendto("evTimeout".encode('utf-8'), self.server_address)
     
    #100ms tick
    def onUpdate(self):
        # update displayed time
        #print(time.strftime("%Y%m%dT%H%M%SZ", time.gmtime()))
        retVal = self.tm.tick()
        if(retVal == True):
            # timeout
            self.myoven.processEvent("evTimeout")
            self.sock.sendto("evTimeout".encode('utf-8'), self.server_address)
        # schedule timer to call myself after 1 second
        self.after(100, self.onUpdate)          
               
    def handlerName(self, event):
            print("get event {}".format(event.char ))


print("running python version {}".format(sys.version_info)) 
app = Application()                      
app.title('Microwave Oven')   
app.mainloop()
