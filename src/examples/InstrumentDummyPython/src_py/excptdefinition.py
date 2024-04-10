#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr  5 16:17:03 2024

@author: framos
"""

class InitializationError(Exception):
    '''
        Exception raised when the device is not initialized. 
    '''
    
    
class NotReferenced(Exception):
    '''
        Exception raised when the device is not referenced and a movement is requested. 
    '''
    
class NotEnable(Exception):
    '''
        Exception raised when the device is disable and a movement has been requested. 
    '''
    
class IncorrectControlMode(Exception):
    '''
        Exception raised when the device is commanded to do a movement with the incorrect mode set
    '''
    
class BadConfigApplyParameter(Exception):
    '''
        Exception raised when the array command is received with a parameter which is not analyzed yet
    '''
    

class ApplyFormatError(Exception):
    '''
        Check the number of parameter (at least exptime) or the component name of the command
    '''
    
class WrongValueParameter(Exception):
    '''
        Wrong value parameter provided. 
    '''
    
class DeviceBusy(Exception):
    '''
      Exception raised when the Device is busy and a new command is ordered to be executed. 
    '''
class WrongNumberOfParams(Exception):
    '''
      Exception raised when a command is received without the enough parameters specified. 
    '''
