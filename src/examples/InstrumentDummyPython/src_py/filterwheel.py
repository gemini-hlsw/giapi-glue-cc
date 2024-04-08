#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 20:41:42 2024

@author: framos
"""

from multiprocessing import  Process
import os
import time
from enum import Enum
import random
import threading
from typing import Final
# CPPYY  is used to import and use the giapigluc+++
import cppyy
giapi_root=os.environ.get("GIAPI_ROOT")
cppyy.add_include_path(f"{giapi_root}/install/include")
cppyy.add_library_path(f"{giapi_root}/install/lib")
cppyy.include("giapi/EpicsStatusHandler.h")
cppyy.include("giapi/GeminiUtil.h")
cppyy.include("giapi/GiapiUtil.h")
cppyy.include("giapi/EpicsStatusHandler.h")
cppyy.include("giapi/GiapiUtil.h")
cppyy.load_library("libgiapi-glue-cc")
cppyy.add_include_path(f"{giapi_root}/src/examples/InstrumentDummyPython")
#pylint: disable=C0413 E0401
from cppyy.gbl import giapi

from actresponse import ActResponse
from cmdorder import RESPONSE
from excptdefinition import InitializationError, NotReferenced, NotEnable, IncorrectControlMode


#### CONSTANT DEFINITION
# Filter Wheel constant
ENABLE        : Final[str] = 'enable'
ISREFERENCED  : Final[str] = 'isReferenced'
POSITION      : Final[str] = 'position'
MODE          : Final[str] = 'mode'
CURRENTACTION : Final[str] = 'currentAction'
STOPEVENT     : Final[str] = 'stopEvent'
STEPANGLE     : Final[float] = 0.75  # This means 225 steps by revolution. 
PARKPOS       : Final[int] = 0

# Status to send to Telescope
FILTERPOS_GIAPI : Final[str] = 'gmp:instdummy:sad:FW1.filterpos'

 
# pylint: disable=R0902
class FilterWheel(Process):
    '''
        This class emulates the control of a FilterWheel hardware. The Filter Wheel
        is controller by a stepper motor  with a step resolution of 0.75 degrees. 
    '''    
    def __init__(self, idName, qCmd, qResp, logger):
        Process.__init__(self)
        self.idName       = idName
        self.qCmd         = qCmd          
        self.qResp        = qResp
        self.logger       = logger
        # pylint: disable=C0103
        self.HOM_POS      = 10.5                    # degrees
        
        self.fSelected    = FilterPosition.UNDEFINED
        self.status       = {ENABLE       : False, 
                             ISREFERENCED : False , 
                             POSITION     : random.randint(0,360),
                             MODE         : FilterMode.JOG}
        self.mngCmdExec   = MngCmdExecution()
    

    ##############################################################

    def testCmd(self, cmdOrder):
        self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.COMPLETED, ""))
    
    
    ##############################################################
    
    def checkStopCmdAndSendRes(self, cmdOrder, stop):
        if not stop.is_set():
             self.__removeMoveAct()
             self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.COMPLETED, ""))
        else:
             # The stop command will call the removeMoveAct. 
             # The GIAPI when recieves the STOP command as completed
             self.qResp.put(ActResponse(cmdOrder.actionID,
                                        self.idName,
                                        RESPONSE.ERROR, "Action Stopped by the user"))

    ##############################################################

    def initCmd(self, cmdOrder):
       if not self.__setMovAction(cmdOrder.actionID):
          self.qResp.put(ActResponse(cmdOrder.actionID,
                                     self.idName, 
                                     RESPONSE.ERROR, 
                                     f'The {self.idName} Fiter Wheel is busy'))       
       else:   
          self.status[ENABLE] = True
          stopDatum = self.mngCmdExec.getStopEvent(FilterCmd.DATUM)
          self.__datumAction(stopDatum)
          self.checkStopCmdAndSendRes(cmdOrder, stopDatum)
    
    ##############################################################

    def datumCmd(self, cmdOrder):
       if not self.__setMovAction(cmdOrder.actionID):
           self.qResp.put(ActResponse(cmdOrder.actionID,
                                      self.idName,
                                      RESPONSE.ERROR,
                                      f'The {self.idName} Fiter Wheel is busy'))       
       else:    
           self.status[ENABLE] = True
           stopDatum = self.mngCmdExec.getStopEvent(FilterCmd.DATUM)
           self.__datumAction(stopDatum, True)
           self.checkStopCmdAndSendRes(cmdOrder, stopDatum)
    
    ##############################################################

    def parkCmd(self, cmdOrder):
       if not self.__setMovAction(cmdOrder.actionID):
           self.qResp.put(ActResponse(cmdOrder.actionID,
                                      self.idName,
                                      RESPONSE.ERROR,
                                      f'The {self.idName} Fiter Wheel is busy'))       
       else:
           #stop = treading.Event()
           stop = self.mngCmdExec.getStopEvent(FilterCmd.PARK)
           try:
               self.__movePos(stop, 0.0)
               self.checkStopCmdAndSendRes(cmdOrder, stop)
           except (InitializationError, NotReferenced, NotEnable, IncorrectControlMode) as e:
                  self.__removeMoveAct()
                  self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.ERROR, str(e)))    
           # pylint: disable=W0718
           except Exception as e:
               self.__removeMoveAct()
               self.qResp.put(ActResponse(cmdOrder.actionID, 
                                          self.idName, RESPONSE.ERROR, 
                                          f'Unexpected Exception. Msg= {str(e)}'))

           self.status[ENABLE] = False
           
    ##############################################################   

    def moveCmd(self, cmdOrder):
       if not self.__setMovAction(cmdOrder.actionID):
           self.qResp.put(ActResponse(cmdOrder.actionID,
                                      self.idName,
                                      RESPONSE.ERROR,
                                      f'The {self.idName} Fiter Wheel is busy'))       
       else:
           self.status[MODE] = FilterMode.POSITION
           if len(cmdOrder.params) > 0 \
               and (filterPos := FilterPosition.getFilterFromName(cmdOrder.params[0])) is not None:
               self.logger.info(f'Moving to {filterPos.name} filter position. {filterPos.value} degrees')
               # The stop command here could be used if you want to add an emergency stop
               stop = self.mngCmdExec.getStopEvent(FilterCmd.MOVE)
               try:
                  self.__movePos(stop, filterPos.value[0])
                  self.checkStopCmdAndSendRes(cmdOrder, stop)
               except (InitializationError, NotReferenced, NotEnable, IncorrectControlMode) as e:
                  self.__removeMoveAct()
                  self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.ERROR, str(e)))    
               # pylint: disable=W0718
               except Exception as e2:
                   self.__removeMoveAct()
                   self.qResp.put(ActResponse(cmdOrder.actionID, 
                                              self.idName, RESPONSE.ERROR, 
                                              f'Unexpected Exception. Msg= {str(e2)}'))
           else:
               self.qResp.put(ActResponse(cmdOrder.actionID,
                                          self.idName,
                                          RESPONSE.ERROR,
                                          "Not destination position provedided"))
    
    ##############################################################   

    def stopCmd(self, cmdOrder):
       if self.__isMovving():
           self.mngCmdExec.getStopEvent(FilterCmd.DATUM).set()
           self.mngCmdExec.getStopEvent(FilterCmd.INIT).set()
           self.mngCmdExec.getStopEvent(FilterCmd.MOVE).set() 
           self.mngCmdExec.getStopEvent(FilterCmd.PARK).set()
       
       self.__removeMoveAct()
       self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.COMPLETED, ""))
    
    ##############################################################   
    
    # pylint: disable=C0103
    def __increasePos(self, increase=True):
        angle = STEPANGLE if increase else -STEPANGLE
        self.status['position'] = self.status['position'] + angle
        if self.status['position'] > 360.0 or self.status[POSITION] < -360.0:
            self.status[POSITION] = self.status[POSITION] % 360.0

    ##############################################################
    
    # pylint: disable=C0103
    def __movePos (self, stop, destPos):
        if not self.status["isReferenced"]:
            raise NotReferenced("FilterWheel is not Referenced. Please, execute the datum command")
        if self.status[MODE] != FilterMode.POSITION:
            raise IncorrectControlMode("You have to select the POSITIOM Mode before executing the movePos")
        if not self.status[ENABLE]:
            raise NotEnable("The motor controller is disabled, please enable executing the init or datum command")
            
        sense = destPos > self.status[POSITION]
        while (not stop.is_set() and 
               not (self.status[POSITION] > destPos - STEPANGLE
                    and  self.status[POSITION] < destPos + STEPANGLE)):
            self.__increasePos(sense)
            self.logger.info(f'moving to {destPos} and currentPosition: {self.status[POSITION]} degrees')
            time.sleep(0.1)        
    
    ##############################################################
    
    # pylint: disable=C0103
    def __jogMove (self, stop, sense=True):
        if  self.status[MODE] != FilterMode.JOG:
            raise IncorrectControlMode("You have to select the Jog Mode before executing the jogMode")
        while not stop.is_set():
            self.__increasePos(sense)
            self.logger.info(f'Searching limit switch. currentPos: {self.status[POSITION]}')
            time.sleep(0.2) # Five step by seconds. 
            
           
    ##############################################################
    
    # pylint: disable=C0103
    def __datumAction(self, stopDatum, forceReferenced=False):
        if not forceReferenced and self.status[ISREFERENCED]:
            self.logger.info(f'The {self.idName} FilterWheel is currently referenced')
            return True
     
        self.status[MODE] = FilterMode.JOG
        datumSwitch = threading.Event()
        t1 = threading.Thread(target=self.__jogMove, args=(datumSwitch,))
        t1.start()
        # This emulates the randomness in which a stepper may find itself 
        # after a power On. We don't know the distance to reach the homming switch. 
        tSleepHomingSwitch = random.randint(10,20)
        
        i = 0
        self.logger.info(f'Time to find the homming switch {tSleepHomingSwitch}')
        while i < tSleepHomingSwitch and not stopDatum.is_set():
            i+=1
            time.sleep(1)
            self.logger.info(f'tSleepHomingSwitch: {tSleepHomingSwitch} - {i}')
        # Active the limitSwith and finish the jog movement. 
        datumSwitch.set()
        self.fSelected = FilterPosition.UNDEFINED
        if not stopDatum.is_set():
            self.status[POSITION] = self.HOM_POS # This is the limit swithc position
            self.logger.info(f'The {self.idName} is datumned, currentPosition: {self.status["position"]}')
            self.status[ISREFERENCED] = True
            return True
        
        self.status[ISREFERENCED] = False
        return False
    
    ##############################################################
    
    # pylint: disable=C0103
    def __isMovving(self):
        
        if  not self.mngCmdExec.isActive(FilterCmd.DATUM) and \
            not self.mngCmdExec.isActive(FilterCmd.MOVE) and \
            not self.mngCmdExec.isActive(FilterCmd.INIT) and \
            not self.mngCmdExec.isActive(FilterCmd.PARK):
                return False
        return True
    
    ##############################################################
    
    # pylint: disable=C0103
    def __setMovAction(self, actionID):
        if not self.__isMovving():
            self.mngCmdExec.setAction(FilterCmd.DATUM, actionID)
            self.mngCmdExec.setAction(FilterCmd.INIT, actionID) 
            self.mngCmdExec.setAction(FilterCmd.MOVE, actionID)  
            self.mngCmdExec.setAction(FilterCmd.PARK, actionID)
            return True
        return False
    
    # pylint: disable=C0103
    def __removeMoveAct(self):
        self.mngCmdExec.removeAction(FilterCmd.DATUM)
        self.mngCmdExec.removeAction(FilterCmd.INIT)
        self.mngCmdExec.removeAction(FilterCmd.MOVE)
        self.mngCmdExec.removeAction(FilterCmd.PARK)
        
    
    def sendTelescopeStatus(self):
        # Create the status Item firs as it was described in the GIAPIc++ICD50, section 4.2
        giapi.StatusUtil.createStatusItem("gmp:instdummy:sad:FW1.filterpos", giapi.type.Type.FLOAT)
        while True:
            giapi.StatusUtil.setValueAsFloat(FILTERPOS_GIAPI, self.status[POSITION])
            self.logger.debug(f'Sending the position: {self.status[POSITION]}')
            # This is when all the Status Item values will be updated
            giapi.StatusUtil.postStatus()
            time.sleep(2)

    def run(self):
        
        t2 = threading.Thread(target=self.sendTelescopeStatus)
        t2.start()
        
        while True:
            cmdOrder = self.qCmd.get()
            try:
                t1 = None
                self.logger.info(f'{self.idName}-Executing the {FilterCmd.getCmdFromNumber(cmdOrder.cmd)} \
                                 actionID: {cmdOrder.actionID}')
                match cmdOrder.cmd:
                    case FilterCmd.TEST:
                        t1 = threading.Thread(target=self.testCmd, args=(cmdOrder,))
                    case FilterCmd.INIT:
                        t1 = threading.Thread(target=self.initCmd, args=(cmdOrder,))
                    case FilterCmd.DATUM:
                        t1 = threading.Thread(target=self.datumCmd, args=(cmdOrder,))
                    case FilterCmd.PARK:
                        t1 = threading.Thread(target=self.parkCmd, args=(cmdOrder,))
                    case FilterCmd.MOVE:
                        t1 = threading.Thread(target=self.moveCmd, args=(cmdOrder,))
                    case FilterCmd.STOP:
                        t1 = threading.Thread(target=self.stopCmd, args=(cmdOrder,))
                    case _:
                        self.logger.error(f'The {cmdOrder.cmd} command no implemented ')
                        self.qResp.put(ActResponse(cmdOrder.actionID, 
                                                   self.idName, RESPONSE.ERROR, 
                                                   f'The {self.idName} has not implemented \
                                                       the {cmdOrder.cmd} command'))
                # pylint: disable=R0801
                if t1:
                    t1.start()
            # pylint: disable=W0718
            except Exception as e:
                self.logger.error(f'Excepiton happend. {e}')
                self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.ERROR, str(e)))
    
class FilterCmd(Enum):
    '''
        This Enum class lists all the Filter Command implemented
    '''
    TEST    = 0, 'TEST'
    INIT    = 1, 'INIT'
    DATUM   = 2, 'DATUM'
    PARK    = 3, 'PARK'
    MOVE    = 4, 'MOVE'
    STOP    = 5, 'STOP'
    
    # pylint: disable=E0213
    def getCmdFromNumber(n):
        for f in FilterCmd:
            if f.value[0] == n:
                return f
        return None
    
class FilterMode(Enum):
    '''
        This Enum class lists all loop control implemented
    '''
    JOG       = 0, 'JOG'
    POSITION  = 1, 'POSITION'
    
class FilterPosition(Enum):
    '''
       This Enum class indicates the filter name and its position on the frame. 
    '''
    UNDEFINED  = 999.0, 'UNDEFINED'
    U          = 15.0,  'U'
    G          = 105.0, 'G'
    I          = 195.0, 'I'
    Z          = 285.0, 'Z'
    
    # pylint: disable=E0213
    def getFilterFromName(nameFilter):
        for f in FilterPosition:
           if f.name == nameFilter:
               return f
        return None

    
class MngCmdExecution:
    '''
        This class is used to manage the Event signal which are used on the stop command
    '''
    actionExecution : dict = {}
    def __init__(self):
        for cmd in FilterCmd:
            self.actionExecution[cmd] = { CURRENTACTION : -1, 
                                          STOPEVENT     : None}
    
    def isActive(self, cmd):
        if self.actionExecution[cmd][CURRENTACTION] == -1:
            return False
        return True
    
    def setAction(self, cmd, actionID):
        if self.actionExecution[cmd][CURRENTACTION] == -1:
            self.actionExecution[cmd][CURRENTACTION] = actionID
            self.actionExecution[cmd][STOPEVENT] = threading.Event()
            return True
        return False
    
    def removeAction(self, cmd):
        self.actionExecution[cmd][CURRENTACTION] = -1
        self.actionExecution[cmd][STOPEVENT] = None
    
    def getStopEvent(self, cmd):
        return self.actionExecution[cmd][STOPEVENT]
