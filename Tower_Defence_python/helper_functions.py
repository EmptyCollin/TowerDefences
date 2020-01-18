#### ====================================================================================================================== ####
#############                                           IMPORTS                                                    #############
#### ====================================================================================================================== ####

import csv
import colorama
from colorama import Fore as text_color, Style

#### ====================================================================================================================== ####
#############                                          CSV_LOADER                                                  #############
#### ====================================================================================================================== ####

def csv_loader(filename, readall=False):
    ''' Helper function that reads in a CSV file. Optional flag for including header row.
    Input: filename (string), bool_flag (optional)
    Output: List of Rows (comma separated)
    '''
    returnList = []
    with open(filename) as csvfile:
        for row in csv.reader(csvfile):
            returnList.append(row)
    if readall:
        return returnList
    else:
        return returnList[1:]

#### ====================================================================================================================== ####
#############                                            LOGGER                                                    #############
#### ====================================================================================================================== ####

def logger(string, end_value="\n", DELIMITER="||", ERROR=False, SUCCESS=False, INFO=False):
    ''' Helper function for clear logging. Includes support for ERROR, SUCCESS, and INFO.
    Input: string (string), end_of_line_value (optional, default='\n'), bool_flags_for (ERROR, SUCCESS, INFO)
    Output: None (Or text to console)
    '''
    colorama.init()
    inputString = string.split(DELIMITER)
    if len(inputString) > 1:
        print(inputString[0], end="")
        if ERROR:
            print(text_color.RED, end="")
        elif SUCCESS:
            print(text_color.GREEN, end="")
        elif INFO:
            print(text_color.CYAN, end="")
        print(inputString[1], end="")
        print(Style.RESET_ALL, end="")
        print(inputString[2], end=end_value)
    else:
        print(string, end=end_value)
    
