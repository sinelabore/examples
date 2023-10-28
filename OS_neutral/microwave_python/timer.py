class timer:

    def __init__(self):
        self.tickval=0;
        self.presetval=0;
        self.timerStatus="OFF"
        
    def start(self):
        print("Timer::start")
            
        if(self.presetval!=0):
            self.tickval=0;
            self.timerStatus = "ON"
            return True;
        else:
            self.timerStatus = "OFF"
            return False;

    def clear(self):
        print("Timer::stop\n")
        self.presetval=0
        self.timerStatus="OFF"
    
    def preset(self):
        print("Timer::preset")
        return self.presetval
    
    def cont(self):
        self.timerStatus="ON"
        
    def pause(self):
        self.timerStatus="PAUSE"

    def inc(self):
        if(self.presetval<60):
            self.presetval=self.presetval+1
            print("Timer::inc -> preset={}\n".format(self.presetval))

    def dec(self):     
        if(self.presetval>0):
            self.presetval=self.presetval-1
            print("Timer::dec -> preset={}\n".format(self.presetval))
    
    #called every 100ms
    def tick(self):       
    
        if(self.timerStatus=="ON"):            
            if(self.tickval<self.presetval*10):
                self.tickval=self.tickval+1
                print("Timer {} left".format(self.presetval - (self.tickval/10)))
                                 
            elif(self.tickval==self.presetval*10):
                print("Timer::timeout")
                self.clear()
                self.timerStatus="OFF"
                #
                # signal timeout event
                #
                return True
        return False
