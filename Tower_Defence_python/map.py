#### ====================================================================================================================== ####
#############                                           IMPORTS                                                    #############
#### ====================================================================================================================== ####

from helper_functions import *
import pygame

#### ====================================================================================================================== ####
#############                                          MAP_CLASS                                                   #############
#### ====================================================================================================================== ####

class Map:
    ''' Map Class - represents a single Map Object. '''
    # Represents common data for all Maps - only loaded once, not per new Map (Class Variable)

    def __init__(self, settings,m_number):

        legend_data = {}
        if m_number == 'map1':
           l_number = 'legend1'
        elif m_number == 'map2':
           l_number = 'legend2'
        for legend in csv_loader('data/' + l_number +'.csv'):
           legend_data[legend[0]] = { "type": legend[1], "sprite": legend[2] }

        ''' Initialization for Map.
        Input: Settings Oject
        Output: A Map Object
        '''
        self.map_data = {}
        row = 0; col = 0
        Mapp = []
        for cell_row in csv_loader('data/' + m_number + '.csv'):
            r = []
            for cell_col in cell_row:
                r.append(cell_col)
            Mapp.append(r)

        for r in range(len(Mapp)):
            for c in range(len(Mapp[r])):
                if Mapp[r][c] != 'R' and Mapp[r][c] != 'E':
                    self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.scale(pygame.image.load(legend_data[Mapp[r][c]]["sprite"]), settings.tile_size).convert_alpha() }
                elif Mapp[r][c] == 'E':
                     self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.scale(pygame.image.load(legend_data[Mapp[r][c]]["sprite"]), (settings.tile_size[0]*2,settings.tile_size[1]*2)).convert_alpha() }
                else:
                    n = s = e = w = False
                    if Mapp[r-1][c] in ['R','S','E']:
                       n = True
                    if Mapp[r+1][c] in ['R','S','E']:
                       s = True
                    if Mapp[r][c-1] in ['R','S','E']:
                       w = True
                    if Mapp[r][c+1] in ['R','S','E']:
                       e = True
                    if w and e:
                       self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.rotate(pygame.transform.scale(pygame.image.load(legend_data[Mapp[r][c]]["sprite"]), settings.tile_size).convert_alpha(),90) }
                    elif s and e:
                       self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.rotate(pygame.transform.scale(pygame.image.load('assets/' + m_number + '/corner.png'), settings.tile_size).convert_alpha(),0) }
                    elif n and e:
                       self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.rotate(pygame.transform.scale(pygame.image.load('assets/' + m_number + '/corner.png'), settings.tile_size).convert_alpha(),90) }
                    elif n and w:
                       self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.rotate(pygame.transform.scale(pygame.image.load('assets/' + m_number + '/corner.png'), settings.tile_size).convert_alpha(),180) }
                    elif s and w:
                       self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.rotate(pygame.transform.scale(pygame.image.load('assets/' + m_number + '/corner.png'), settings.tile_size).convert_alpha(),270) }
                    else:
                       self.map_data[(c, r)] = { "value": Mapp[r][c], "sprite": pygame.transform.scale(pygame.image.load(legend_data[Mapp[r][c]]["sprite"]), settings.tile_size).convert_alpha() }

#### ====================================================================================================================== ####
#############                                        MAP_FUNCTIONS                                                 #############
#### ====================================================================================================================== ####

def render_map(map, screen, settings):
    ''' Helper function that renders the Map.
    Input: Map Object, screen (pygame display), Settings Object
    Output: None
    '''
    for cell in map.map_data:
        if map.map_data[cell]['value'] != 'E':
           screen.blit(map.map_data[cell]["sprite"], [cell[0] * settings.tile_size[0], cell[1] * settings.tile_size[1]])
    for cell in map.map_data:
        if map.map_data[cell]['value'] == 'E':
           screen.blit(map.map_data[cell]["sprite"], [cell[0] * settings.tile_size[0] - settings.tile_size[0]//2 , (cell[1]-1) * settings.tile_size[1]])


def check_location(game_data,map, settings, location, placement=False, direction=None):
    ''' Helper function that checks a given location
    Input: Map Object, location (Tuple), Settings Object, placement (Boolean), direction (String)
    Output: Boolean
    '''
    # Handle Tower Placement checking
    if placement:
        cell_value_tl = (location[0] // settings.tile_size[0], location[1] // settings.tile_size[1])
        cell_value_tr = ((location[0] + settings.tile_size[0] * 2) // settings.tile_size[0], location[1] // settings.tile_size[1])
        cell_value_bl = (location[0] // settings.tile_size[0], (location[1] + settings.tile_size[1] * 2) // settings.tile_size[1])
        cell_value_br = ((location[0] + settings.tile_size[0] * 2) // settings.tile_size[0], (location[1] + settings.tile_size[1] * 2) // settings.tile_size[1])
        try:
            if map.map_data[cell_value_tl]["value"] == "B" and map.map_data[cell_value_tr]["value"] == "B" and map.map_data[cell_value_bl]["value"] == "B" and map.map_data[cell_value_br]["value"] == "B":
               return True
        except:
            return False

    # Handle Enemy Movement checking (path following)
    else:
        new_location = [0, 0]

        if direction == "E":
            new_location[0] += 1
        elif direction == "W":
            new_location[0] -= 1
        elif direction == "S":
            new_location[1] += 1
        else:
            new_location[1] -= 1

        xLoc = location[0] + settings.tile_size[0] - 3 if direction == "W" else location[0]
        yLoc = location[1] + settings.tile_size[1] - 3 if direction == "N" else location[1]

        new_location = [(xLoc // settings.tile_size[0]) + new_location[0], (yLoc // settings.tile_size[1]) + new_location[1]]

        try:
            if  map.map_data[(new_location[0], new_location[1])]["value"] in ["R", "S", "E"]:
                return True
        except:
            game_data['playing'] = False
        return False
        

def get_starting_tile(map, settings):
    ''' Helper function that retrieves starting tile.
    Input: Map Object, Settings Object
    Output: location of starting tile (tuple)
    '''
    for col_row in map.map_data:
        if map.map_data[col_row]["value"] == "S":
            return [col_row[0] * settings.tile_size[0], col_row[1] * settings.tile_size[1]]
