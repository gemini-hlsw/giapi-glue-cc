#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 20:41:42 2024

@author: framos
"""

from dataclasses import dataclass


@dataclass
class ActResponse:
    '''
       This class is used to send a reponse from the software Components to InstSeq.      
    '''
    actionID  : int
    compName  : str
    response  : int
    errorMsg  : str
    resData   : dict = None
    
   

