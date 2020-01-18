#### ====================================================================================================================== ####
#############                                           IMPORTS                                                    #############
#### ====================================================================================================================== ####

from helper_functions import *
import pygame

#### ====================================================================================================================== ####
#############                                         ENEMY_CLASS                                                  #############
#### ====================================================================================================================== ####

class Enemy:
    ''' Enemy Class - represents a single Enemy Object. '''
    # Represents common data for all enemies - only loaded once, not per new Enemy (Class Variable)
    enemy_data = {}
    for enemy in csv_loader("data/enemies.csv"):
        enemy_data[enemy[0]] = { "sprite": enemy[1], "health": int(enemy[2]), "speed": int(enemy[3]),"points":int(enemy[4]) }
    # Inverse direction dictionary for enemy movement
    inverse_direction = { "S": "N", "E": "W", "N": "S", "W": "E" }
    def __init__(self, enemy_type, location, settings):
        ''' Initialization for Enemy.
        Input: enemy type (string), location (tuple of ints)
        Output: An Enemy Object
        '''
        self.name = enemy_type
        self.sprite = pygame.transform.scale(pygame.image.load(Enemy.enemy_data[enemy_type]["sprite"]).convert_alpha(), settings.tile_size)
        self.health = Enemy.enemy_data[enemy_type]["health"]
        self.speed = Enemy.enemy_data[enemy_type]["speed"]
        self.location = location
        self.direction = "S"
        self.reverse_direction = "N"
        self.points = Enemy.enemy_data[enemy_type]["points"]

#### ====================================================================================================================== ####
#############                                       ENEMY_FUNCTIONS                                                #############
#### ====================================================================================================================== ####

def update_enemy(enemy, direction, damage=0):
    ''' Helper function that updates a single provided Enemy.
    Input: Enemy Object, direction (string), damage (int)
    Output: Boolean (True if Enemy Alive; False otherwise)
    '''
    # Cause enemy to take damage
    enemy.health -= damage
    # If enemy health <= 0, return False (to kill enemy)
    if enemy.health <= 0:
        return False
    
    # Update directional variables
    enemy.direction = direction
    enemy.reverse_direction = Enemy.inverse_direction[direction]

    # Update location based on direction
    if enemy.direction == "N":
        enemy.location[1] -= enemy.speed
    elif enemy.direction == "S":
        enemy.location[1] += enemy.speed
    elif enemy.direction == "E":
        enemy.location[0] += enemy.speed
    else:
        enemy.location[0] -= enemy.speed
    return True

def render_enemy(enemy, screen, settings):
    ''' Helper function that renders a single provided Enemy.
    Input: Enemy Object, screen (pygame display), Settings Object
    Output: None
    '''
    ice = pygame.transform.scale(pygame.image.load('assets/enemies/ice.png').convert_alpha(), settings.tile_size)
    screen.blit(enemy.sprite, enemy.location)
    if enemy.speed == 0:
       location = (enemy.location[0],enemy.location[1]+settings.tile_size[1]//2)
       screen.blit(ice,location)
