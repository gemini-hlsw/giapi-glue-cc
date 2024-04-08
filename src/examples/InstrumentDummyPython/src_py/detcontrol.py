#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 20:41:42 2024

@author: framos
"""

import os
import time
import math
import threading
from datetime import datetime
from multiprocessing import Queue, Process
from enum import Enum

from typing import Final
from astropy.io import fits
import numpy as np

from actresponse import ActResponse
from cmdorder import RESPONSE
from excptdefinition import WrongValueParameter, DeviceBusy, WrongNumberOfParams, InitializationError

TMINFOWLER   : Final[float] = 0.68  # seconds
TFRAME       : Final[float] = 1.45 # seconds
MAXFS        : Final[float] = 16   # MAximum fowler sampling
MINEXPTIME   : Final[float] = 1.68
RESETTIME    : Final[float] = 1.45 # seconds
READOUTTIME  : Final[float] = 1    # second
EPXTIMECAL   : Final[str]   = 'exptimecalc'
DIRIMGPATH   : Final[str]   = './images'
    
class DetectCmd(Enum):
    '''
       Define the all commands implemented in the FilterWheel. 
    '''
    TEST    = 0, 'TEST'
    INIT    = 1, 'INIT'
    EXPOSE  = 2, 'EXPOSE'
    CONFIG  = 3, 'CONFIG'
    STOP    = 4, 'STOP'
    
    # pylint: disable=E0213
    def getCmdFromNumber(n):
        for f in DetectCmd:
           if f.value[0] == n:
               return f
        return None
    
    
class DetectMode(Enum):
    '''
       Define the three different detector mode implemented 
    '''
    CDS       = 0, 'CDS'
    FOWLER    = 1, 'FOWLER'
    RAMP      = 2, 'RAMP'
    
    def __new__(cls, value, strName):
       obj = object.__new__(cls)
        # pylint: disable=C0103
       obj._value_ = value
       obj.strName = strName
       return obj

# pylint: disable=R0902
class DetectControl(Process):
    '''
       Main class which emulates the control of a Detector Controller. 
       There are some message getting from the MACIE library to control the H2RG.
    '''
    idName       : str
    qCmd         : Queue          
    qResp        : Queue
    detectMode   : DetectMode = DetectMode.FOWLER
    
    def __init__(self, idName, qCmd : Queue, qResp, logger):
        Process.__init__(self)
        self.qCmd         = qCmd
        self.qResp        = qResp
        self.idName       = idName
        self.detectorBusy = False
        self.isInit       = False
        self.expTimeCalc = MINEXPTIME
        self.stop         = False
        self.logger       = logger
    
    ##############################################################
    
    def testCmd(self, cmdOrder):
        
        if not self.isInit:
            self.initCmd(cmdOrder)
        else:
           self.__sendResponse(cmdOrder, RESPONSE.COMPLETED, "")
    
    ##############################################################
    
    def initCmd(self, cmdOrder):
                   
        self.__stopAction ()
        self.detectorBusy = True
        self.logger.info(f'{self.idName}-Sending to Detector Controller\
                         the initialization. For example MACIE_Init()')
        time.sleep(0.1)
     
        self.logger.info(f'{self.idName}-Sending the GetAvailableMACIEs to the DC.\
                         To know how many card is ready ')
        time.sleep(0.1)
     
        self.logger.info(f'{self.idName}-Loading the MACIE Firmware (MACIE store \
                         the firmware in the EEPROM). Sending MACIE_loadMACIEFirmware')
        time.sleep(2)
        
        self.logger.info(f'{self.idName}-Sending the MACIE_DownloadMACIEFile. Used \
                         to initialize the MACIE card and power up the ASIC')
        time.sleep(1)
        
        self.isInit = True
        self.detectorBusy = False 
        self.__sendResponse(cmdOrder, RESPONSE.COMPLETED, "")
    
    ##############################################################
    
    # pylint: disable=C0103  
    def __checkExpTimeParam(self, exptime_sci):
        if exptime_sci < MINEXPTIME:
            raise WrongValueParameter(f'The exposition time provided is less than the minimun allowed\
                            ({MINEXPTIME}). Please provide an equal or greate value. ')
       
    ##############################################################
    
    # pylint: disable=C0103  
    def __cdsModeConfig(self, expTime):
        self.logger.info(f'{self.idName}-Sending to the detector the expTime: {expTime}, \
                         nreset:1 nreads:2')
        time.sleep(0.1)
        self.expTimeCalc = RESETTIME + expTime + TFRAME*2
        
    ##############################################################
    
    # pylint: disable=C0103  
    def __rampModeConfig(self, expTime):
        ngroups = 4
        nreads = math.ceil((expTime / TFRAME) / ngroups)
        ndrop = 3
        self.logger.info(f'{self.idName}-Sending to the detector the nreset:1 ngroups: \
                         {ngroups} nreads:{nreads} ndrop:{ndrop} ')
        time.sleep(0.1)
        self.expTimeCalc = RESETTIME + (nreads + ndrop) * TFRAME 
        
    ##############################################################
    
    # pylint: disable=C0103  
    def __fowlerModeConfig(self, exptime_sci): 
        
        numFowlerNumber = int(exptime_sci - TMINFOWLER) / TFRAME
        fsNumberCal = MAXFS
        while fsNumberCal > numFowlerNumber:
            fsNumberCal//=2
            
        fowlerTime = exptime_sci - (TFRAME * fsNumberCal)
        self.logger.info(f'{self.idName}-Sending to the detector the nreset: 1, \
                         numFowler: {fsNumberCal}, groups: 1,  fowlerTime: {fowlerTime}.\
                             With H2RG would be using the MACIE_WriteASICReg')
        time.sleep(0.1)
        self.expTimeCalc = RESETTIME + fsNumberCal*2 + fowlerTime + READOUTTIME
    
    ##############################################################
    
    # pylint: disable=C0103  
    def __checkDectBusy(self):
        if self.detectorBusy:
           raise DeviceBusy("The Detector Controller is busy") 
           
    ##############################################################

    # pylint: disable=C0103  
    def __sendResponse(self, cmdOrder, response, msg):
        
        d = None
        if response == RESPONSE.COMPLETED and cmdOrder.reqData:
            for key in cmdOrder.reqData:
                match key:
                    case 'exptimecalc':
                        d = {EPXTIMECAL : self.expTimeCalc}
                    case _:
                        response = RESPONSE.ERROR
                        msg = f'InstSeq requested data but the {key} was not analyzed.'
                        break
                
        self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, response, msg, d))   

    ##############################################################
    
    def configCmd(self, cmdOrder):
        msg=""
        response = RESPONSE.COMPLETED
        try:
            if len(cmdOrder.params) == 0:
                raise WrongNumberOfParams('You should provide at least the exposure time in seconds')
                
            if not self.isInit:
                raise InitializationError('Detector not initialized first. Please, initialize \
                                the detector before')
            self.__checkDectBusy()
            #cmdOrder.param[i], i = 0 -> exptime, i = 1 ->detmode [optional]
            detectMode = DetectMode.FOWLER.strName if len(cmdOrder.params) == 1 else cmdOrder.params[1]
            expTime = float(cmdOrder.params[0])
            self.__checkExpTimeParam(expTime)
            self.detectorBusy = True
            match detectMode:
                case DetectMode.CDS.strName:
                    self.__cdsModeConfig(expTime)
                case DetectMode.FOWLER.strName:
                    self.__fowlerModeConfig(expTime)
                case DetectMode.RAMP.strName:    
                    self.__rampModeConfig(expTime)
                case _:
                    self.detectorBusy = False
                    raise WrongValueParameter('Bad configMode specified.')   
            self.detectorBusy = False
        except (WrongNumberOfParams, WrongValueParameter, InitializationError, DeviceBusy) as e:
            msg = str(e)
            self.logger.error(msg)
            response = RESPONSE.ERROR
        # pylint: disable=W0718
        except Exception as e:
            msg = f'Unexpected Exception. Msg= {str(e)}'
            self.logger.error(msg)
            response = RESPONSE.ERROR
        self.__sendResponse(cmdOrder, response, msg)
            
    
    ##############################################################
            
    def createFitsFile(self, datalabel): 
        n = np.arange(100.0)
        hdr = fits.Header()
        hdr['OBSERVER']    = 'Charle Messier'
        hdr['COMMENT']     = "This is and example of the InstumentDummy python"
        hdr['DC_MODE']     = self.detectMode.strName
        hdr['ACQTIME']     = str(datetime.now())
        
        hdu = fits.PrimaryHDU(n,header=hdr)
        hdul = fits.HDUList([hdu])
        
        if not os.path.exists(DIRIMGPATH):
            os.mkdir(DIRIMGPATH)
        
        hdul.writeto(f'{DIRIMGPATH}/{datalabel}', overwrite=True)
        self.logger.info(f'New fits file created in {DIRIMGPATH}/{datalabel}')
        
    ##############################################################
            
    def expCmd(self, cmdOrder):
        try:
            if not self.isInit:
                raise InitializationError('Detector not initialized first. Please, initialize the \
                                detector before')
            self.__checkDectBusy() 
            self.detectorBusy = True
            fitsName = cmdOrder.params[0]
            i = 0
            self.logger.info('Integrating ')
            while  i < self.expTimeCalc and not self.stop:
                self.logger.info('.')
                i+=1
                time.sleep(1)
            self.createFitsFile(fitsName)  
            msg=""
            response=RESPONSE.COMPLETED
            if self.stop:
                msg=""
                response=RESPONSE.COMPLETED
            
            self.detectorBusy = False
            self.__sendResponse(cmdOrder, response, msg)
        except (WrongNumberOfParams, WrongValueParameter, InitializationError, DeviceBusy) as e:
            self.logger.error(str(e))
            self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.ERROR, str(e)))
        # pylint: disable=W0718
        except Exception as e:
            msg = f'Unexpected Exception. Msg= {str(e)}'
            self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.ERROR, msg))
    
    ##############################################################
    
    def __stopAction(self):
        if self.detectorBusy:
            self.stop = True
            self.logger.debug('All the process stopped') 
        self.stop = False
           
    
    ###############################################################
    
    def stopCmd(self, cmdOrder):
        self.__stopAction()
        time.sleep(1)
        self.__sendResponse(cmdOrder, RESPONSE.COMPLETED, "")
            
    ##############################################################
    
    def run(self):
        while True:
            cmdOrder = self.qCmd.get()
            self.logger.info(f'{self.idName}-Executing the {DetectCmd.getCmdFromNumber(cmdOrder.cmd)}.\
                             actionID: {cmdOrder.actionID}')
            try:
                t1 = None
                match cmdOrder.cmd:
                    case DetectCmd.TEST:
                        t1 = threading.Thread(target=self.testCmd, args=(cmdOrder,))
                    case DetectCmd.INIT:
                        t1 = threading.Thread(target=self.initCmd, args=(cmdOrder,))
                    case DetectCmd.EXPOSE:
                        t1 = threading.Thread(target=self.expCmd, args=(cmdOrder,))
                    case DetectCmd.CONFIG:
                        t1 = threading.Thread(target=self.configCmd, args=(cmdOrder,))
                    case DetectCmd.STOP:
                        t1 = threading.Thread(target=self.stopCmd, args=(cmdOrder,))
                                       
                    case _:
                        self.logger.error(f'The {cmdOrder.cmd} command no implemented ')
                        self.qResp.put(ActResponse(cmdOrder.actionID,
                                                   self.idName,
                                                   RESPONSE.ERROR,
                                                   f'The {self.idName} has not implemented the {cmdOrder.cmd} command '))
                if t1:
                    t1.start()
            # pylint: disable=W0718
            except Exception as e:
                self.logger.error(f'Excepiton happend. {e}')
                self.qResp.put(ActResponse(cmdOrder.actionID, self.idName, RESPONSE.ERROR, str(e)))
                