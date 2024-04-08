#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 20:41:42 2024

@author: framos
"""
import time as ti
import os
import sys
from multiprocessing import Queue, Manager
from queue import Empty
from typing import Final
from enum import Enum
import logging
import logging.config
import yaml
import cppyy
# Loading the GIAPI library
sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))
giapi_root=os.environ.get("GIAPI_ROOT")
cppyy.add_include_path(f"{giapi_root}/install/include")
cppyy.add_library_path(f"{giapi_root}/install/lib")
cppyy.include("giapi/StatusUtil.h")
cppyy.include("giapi/SequenceCommandHandler.h")
cppyy.include("giapi/CommandUtil.h")
cppyy.include("giapi/HandlerResponse.h")
cppyy.include("giapi/DataUtil.h")
cppyy.load_library("libgiapi-glue-cc")
cppyy.add_include_path(f"{giapi_root}/src/examples/InstrumentDummyPython")
cppyy.include("DataResponse.h")
cppyy.include("InstCmdHandler.h")
#pylint: disable=C0413 E0401
from cppyy.gbl import giapi
from cppyy.gbl import instDummy

from filterwheel import FilterWheel, FilterCmd
from detcontrol import DetectControl, DetectCmd, MINEXPTIME, EPXTIMECAL
from cmdorder import CmdOrder, RESPONSE
from excptdefinition import BadConfigApplyParameter, ApplyFormatError

# COMPONENTS AND DC NAMES
FW1 : Final[str] = "FW1"
DC1 : Final[str] = "DC1"

# LIST OF CC AND DC
LCC : Final[list] = [FW1]
LDC : Final[list] = [DC1]

# CONSTANTE DEFINITION TO RESPONSE BETWEEN INSTSEQ AND CC
LISTCOMP     : Final[str] = 'listComp'
TIMEINIT     : Final[str] = 'timeInit'
TIMEOUT      : Final[str] = 'timeOut'
ONESEC       : Final[int] = 1000000000

# CONFIG KEY DEFINITION (APPLY COMMAND)
INSTSEQNAME  : Final[str] = 'instdummy'
CC_STR       : Final[str] = 'cc'
DC_STR       : Final[str] = 'dc'

#CONSTANT DICTIONARY ATTRIBUTES
CMD_ORDER  : Final[str] = 'cmdOrder'
QUEUE      : Final[str] = 'q'
TOUT       : Final[str] = 'out'

# pylint: disable=R0902
class InstSeq():
    '''
       Main class of this python Instrument Dummy. The purpose of this class is 
       to subscribe to GIAPI commands (TEST, INIT, PARK, ...) and synchronize 
       the execution of lower level components such as a filter wheel and a detector. 
       
    '''
    
    def __init__(self, qFilt, qDetCont, mngGIAPIresp, log):
        self.callbackGiapi = self.callback_giapi
        self.actRequested    = {}
        self.qFilter         = qFilt
        self.qDC             = qDetCont
        self.mngGIAPIres     = mngGIAPIresp
        self.expTimeDC       = MINEXPTIME
        self.logger          = log
        
        
        self._handler = instDummy.InstCmdHandler.create(self.callbackGiapi)
        #Subscribing to all giapi sequence commands
        seqAttr = dir( giapi.command.SequenceCommand)
        for attr in seqAttr:
            if not attr.startswith('__') and attr.isupper():
               self.logger.info(f'InstSeq is sbuscribed to {attr} command')
               giapi.CommandUtil.subscribeSequenceCommand(getattr(giapi.command.SequenceCommand, attr), 
                                                          giapi.command.ActivitySet.SET_PRESET_START,
                                                          self._handler)
        
        #Subscribing to apply command because this is a special cmd 
        giapi.CommandUtil.subscribeApply(INSTSEQNAME, giapi.command.ActivitySet.SET_PRESET_START,self._handler)

    

    def testCMD(self, actID, activity, t):
        if activity == giapi.command.Activity.PRESET:
            return giapi.HandlerResponse.ACCEPTED
        self.mngGIAPIres.insertNewActionID(actID, FW1, t, 2*ONESEC)
        self.qFilter.put(CmdOrder(actID, FilterCmd.TEST, [], FW1, None))
        return giapi.HandlerResponse.STARTED

    def initCMD(self, actID, activity, t):
        if activity == giapi.command.Activity.PRESET:
            return giapi.HandlerResponse.ACCEPTED
        self.mngGIAPIres.insertNewActionID(actID, FW1, t, 40*ONESEC)
        self.qFilter.put(CmdOrder(actID, FilterCmd.INIT, [], FW1, None))
        self.mngGIAPIres.insertNewActionID(actID, DC1, t, 10*ONESEC)
        self.qDC.put(CmdOrder(actID, DetectCmd.INIT, [], DC1))
        
        return giapi.HandlerResponse.STARTED
    
    def stopCMD(self, actID, activity, t):
        if activity == giapi.command.Activity.PRESET:
            return giapi.HandlerResponse.ACCEPTED
        self.mngGIAPIres.insertNewActionID(actID, FW1, t, 5*ONESEC)
        self.qFilter.put(CmdOrder(actID, FilterCmd.STOP, [], FW1, None))
        self.mngGIAPIres.insertNewActionID(actID, DC1, t, 5*ONESEC)
        self.qDC.put(CmdOrder(actID, DetectCmd.STOP, [], DC1, None))
        
        return giapi.HandlerResponse.STARTED
    
    
    def parkCMD(self, actID, activity, t):
        # TODO. Improve the activity management
        if activity == giapi.command.Activity.PRESET:
            return giapi.HandlerResponse.ACCEPTED
        self.mngGIAPIres.insertNewActionID(actID, FW1, t, 40*ONESEC)
        self.qFilter.put(CmdOrder(actID, FilterCmd.PARK, [], FW1, None))
        return giapi.HandlerResponse.STARTED
    
    def datumCMD(self, actID, activity, t):
        # TODO. Improve the activity management
        if activity == giapi.command.Activity.PRESET:
            return giapi.HandlerResponse.ACCEPTED
        self.mngGIAPIres.insertNewActionID(actID, FW1, t, 40*ONESEC)
        self.qFilter.put(CmdOrder(actID, FilterCmd.DATUM, [], FW1), None)
        return giapi.HandlerResponse.STARTED
    
    # pylint: disable=C0103 R0913
    def __insertCmdOrder(self, queue, tout, dCmdOrder, actID, compName, cmd, param, reqData):
        cmdObj = dCmdOrder[compName].get(cmd)
        if cmdObj:
            cmdObj[CMD_ORDER].params.append(param)
            if reqData:
                cmdObj[CMD_ORDER].reqData.append(param)
            if tout > cmdObj[TOUT]:
                cmdObj[TOUT] = tout
        else:
            dCmdOrder[compName][cmd] = {CMD_ORDER : CmdOrder(actID, cmd, [param], compName, reqData),\
                                        QUEUE     : queue,\
                                        TOUT      : tout }
    
    # pylint: disable=C0103                                        
    def __fromApplyParamsGetCmdOrders(self, config, dCmdOrder, actID):
        for key in config.getKeys():
            valk = config.getValue(key).c_str()
            lkey = key.split(':')
            # Basically it is checked the follwoing expressiong
            # <instName>:[dc|cc]:<componentName>
            if len(lkey) > 1 and lkey[0] != INSTSEQNAME and \
                lkey[1] != CC_STR  and lkey[1] != DC_STR:
               raise ApplyFormatError('Check the number of parameter (at least exptime) or the component name of the command')
            
            lComp = lkey[2].split('.')
   
            match lComp[1]:
                case APPLYPARAMS.SETFILTER.strName: 
                    self.__insertCmdOrder(self.qFilter, 40*ONESEC, 
                                          dCmdOrder, actID, 
                                          lComp[0], FilterCmd.MOVE, 
                                          valk, None)
                case APPLYPARAMS.EXPTIME.strName:
                    self.__insertCmdOrder(self.qDC, 5*ONESEC, 
                                          dCmdOrder, actID, 
                                          lComp[0], DetectCmd.CONFIG, 
                                          float(valk), [EPXTIMECAL])
                case APPLYPARAMS.DETMODE.strName:
                    self.__insertCmdOrder(self.qDC, 5*ONESEC, 
                                          dCmdOrder, actID, 
                                          lComp[0], DetectCmd.CONFIG, 
                                          valk, None)
                case _:
                    raise BadConfigApplyParameter('Configuration parameter not analyzed')
    
    def applyCMD(self, actID, activity, t, config):
        # TODO. Improve the activity management
        if activity == giapi.command.Activity.PRESET:
            return giapi.HandlerResponse.ACCEPTED
        
        dCmdOrder = { DC1 : {}, FW1 : {} }
        
        self.__fromApplyParamsGetCmdOrders(config, dCmdOrder, actID) 
        # pylint: disable=C0206
        for key in dCmdOrder:
            for k2 in dCmdOrder[key]:
                q = dCmdOrder[key][k2][QUEUE]
                self.mngGIAPIres.insertNewActionID(actID, key, t, dCmdOrder[key][k2][TOUT])
                q.put(dCmdOrder[key][k2][CMD_ORDER])
       
        return giapi.HandlerResponse.STARTED
    
    
    ##############################################################
    
    def observeCmd(self, actID, activity, t, config):
        # TODO. Improve the activity management
        if activity == giapi.command.Activity.PRESET:
            return giapi.HandlerResponse.ACCEPTED
        
        self.logger.debug(f'observeCmd {actID} {activity}')
        fitsName = config.getValue('DATA_LABEL') 
        self.mngGIAPIres.insertNewActionID(actID, DC1, t, self.expTimeDC*ONESEC + 5)
        self.qDC.put(CmdOrder(actID, DetectCmd.EXPOSE, [fitsName.c_str()], DC1))
        
        return giapi.HandlerResponse.STARTED
    
    def callback_giapi(self, action_id, seq_cmd, activity, config):
        '''
        This function will receive all the commands from the GIAPI
        '''
        t = ti.time_ns()
        res = giapi.HandlerResponse.STARTED
        try:   
            self.logger.info(f'{seq_cmd} command received {action_id} {activity}')
            match seq_cmd:
                case giapi.command.SequenceCommand.APPLY:
                   res = self.applyCMD(action_id, activity, t, config)
                case giapi.command.SequenceCommand.TEST:
                   res = self.testCMD(action_id, activity, t)
                case giapi.command.SequenceCommand.INIT:
                   self.initCMD(action_id, activity, t)
                case giapi.command.SequenceCommand.DATUM:
                   self.datumCMD(action_id, activity, t)
                case giapi.command.SequenceCommand.PARK:
                   res = self.parkCMD(action_id, activity, t)
                case giapi.command.SequenceCommand.OBSERVE:
                   res = self.observeCmd(action_id, activity, t, config)
                case giapi.command.SequenceCommand.STOP:
                   res = self.stopCMD(action_id, activity, t)
                case _:
                   self.logger.error('Command not implemented yet')
                   return instDummy.DataResponse(giapi.HandlerResponse.ERROR, "Command not implemented yet")
        # pylint: disable=W0718
        except Exception as e:
            self.logger.error(str(e))
            return instDummy.DataResponse(giapi.HandlerResponse.ERROR, str(e))    

        return instDummy.DataResponse(res, "Processed")
    
    
class APPLYPARAMS(Enum):
    '''
       Define the APPLY sequence command name parameters. 
       For example instdummy:dc:DC1.exptime=5.0
    '''
    SETFILTER  = 0, 'setfilter'
    EXPTIME    = 1, 'exptime'
    DETMODE    = 2, 'detmode'
    
   
    def __new__(cls, value, strName):
       obj = object.__new__(cls)
       # pylint: disable=C0103
       obj._value_ = value
       obj.strName = strName
       return obj
    
class ManageGIAPIres:
    '''
       This class is the responsible to manage the response to client.
    '''
    def __init__(self, actList, managerP, log):
        self.actionList = actList
        self.mng = managerP
        self.logger = log
    
    # pylint: disable=C0103 
    def __checkAndSetResponse(self, actRes):
        
        if actRes.resData:
            for k in actRes.resData:
                match k:
                    case 'exptimecalc': 
                        InstSeq.expTimeDC = actRes.resData[k]
                    case _:
                        self.logger.error ("Error getting the data response")
        
       
    def sendResponse(self, actRes):
        if self.actionList.get(actRes.actionID):
            self.actionList[actRes.actionID]['listComp'].remove(actRes.compName) 
            if not self.actionList[actRes.actionID]['listComp']:
                if RESPONSE.ERROR == actRes.response:
                    self.logger.info('fThe actionID {actRes.actionID} was completed with ERROR ')
                    giapi.CommandUtil.postCompletionInfo(actRes.actionID, 
                                                         giapi.HandlerResponse.createError(actRes.errorMsg))
                    
                else: 
                    self.logger.info(f'The actionID {actRes.actionID} was completed successfully')
                    giapi.CommandUtil.postCompletionInfo(actRes.actionID, 
                                                         giapi.HandlerResponse.create(giapi.HandlerResponse.COMPLETED))
                    
                self.actionList.pop(actRes.actionID)
                
        else:
            self.logger.warm('The actionId is not in the list')
            
        self.__checkAndSetResponse(actRes)
    
    def insertNewActionID(self, actID : int , compName : str, tInit : int, tOut : int):
        
        if not self.actionList.get(actID):
            self.actionList[actID] = { LISTCOMP : mng.list([compName]), 
                                      TIMEINIT : tInit, 
                                      TIMEOUT : tOut} 
        else:
            self.actionList[actID][LISTCOMP].append(compName)
       
    
    def checkTimeouts(self):
        
        for key in self.actionList.keys():
            tout = ti.time_ns() - self.actionList[key][TIMEINIT]
            if tout > self.actionList[key][TIMEOUT]:
                self.logger.error(f'The {key} actionId has spent more than the limit thought ({tout} seconds). \
                                  Responding ERROR ')
                
                giapi.CommandUtil.postCompletionInfo(key, 
                                                     giapi.HandlerResponse.createError(f'The {key} action can not be \
                                                                                       completed in the time out specified \
                                                                                       {self.actionList[key][TIMEOUT]} \
                                                                                       Please check the following components: \
                                                                                       {self.actionList[key][LISTCOMP]}'))
                self.actionList.pop(key)
    

def createLoggerInstance(logFileName):
    # Initialize the logger once as the application starts up.
    with open("logConfig.yaml", 'rt', encoding="utf-8") as f:
        config = yaml.safe_load(f.read())
        config['handlers']['file_handler']['filename']=logFileName
        logging.config.dictConfig(config)
        return logging.getLogger(__name__)

if __name__ == "__main__":
    mng = Manager()
    qFilter = Queue()
    qDC = Queue()
    actionList = mng.dict()
    #actIdG = Value('i', 0)
    qGiapi = Queue()
    logger = createLoggerInstance('InstDummy.log')
    mngGIAPIres = ManageGIAPIres(actionList, mng, logger)
    InstSeq = InstSeq(qFilter, qDC, mngGIAPIres, logger)
    filter1 = FilterWheel(FW1, qFilter, qGiapi, logger)
    filter1.start()
    
    detectorCont = DetectControl(DC1,qDC, qGiapi, logger)
    detectorCont.start()
    
    while True:
        try:
           resp = qGiapi.get(timeout=2)
           mngGIAPIres.sendResponse(resp)
        except Empty:
            mngGIAPIres.checkTimeouts()
            