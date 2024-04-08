#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 20:41:42 2024

@author: framos
"""

from enum import Enum


class RESPONSE(Enum):
    '''
        This class defines the state in which the command execution can be found. 
    '''
    ACCEPTED   = 0, 'ACCEPTED'
    STARTED    = 1, 'STARTED'
    COMPLETED  = 2, 'COMPLETED'
    ERROR      = 3, 'ERROR'
    
# pylint: disable=R0913 R0903
class CmdOrder:
    '''
         This class is used between the InstSeq and the low level software components to order
         a command. 
    '''
    def __init__(self,actionID : int, cmd : int, params : list, compName : str, reqData : list = None ):
        self.actionID = actionID
        self.cmd      = cmd
        self.params   = params
        self.compName = compName
        self.reqData  = reqData
    

