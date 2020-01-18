#### ====================================================================================================================== ####
#############                                           IMPORTS                                                    #############
#### ====================================================================================================================== ####

from helper_functions import *
import math
import pygame

#### ====================================================================================================================== ####
#############                                         TOWER_CLASS                                                  #############
#### ====================================================================================================================== ####

class Tower:
    ''' Tower Class - represents a single Tower Object. '''
    # Represents common data for all towers - only loaded once, not per new Tower (Class Variable)
    tower_data = {}
    for tower in csv_loader("data/towers.csv"):
        tower_data[tower[0]] = { "sprite": tower[1], "damage": int(tower[2]), "rate_of_fire": int(tower[3]), "radius": int(tower[4]) ,"level": int(tower[5]),"basic":tower[6]}
    def __init__(self, tower_type, location, radius_sprite):
        ''' Initialization for Tower.
        Input: tower_type (string), location (tuple), radius_sprite (pygame.Surface)
        Output: A Tower Object
        '''
        self.name = tower_type
        self.sprite = pygame.image.load(Tower.tower_data[tower_type]["sprite"]).convert_alpha()
        self.radius_sprite = radius_sprite
        self.radius = Tower.tower_data[tower_type]["radius"]
        self.damage = Tower.tower_data[tower_type]["damage"]
        self.rate_of_fire = Tower.tower_data[tower_type]["rate_of_fire"]
        self.location = location
        self.isClicked = False
        self.angle = 0
        self.LockTarget = False
        self.Fire = False
        self.arrange = 0
        self.level = Tower.tower_data[tower_type]["level"]
        self.basic = Tower.tower_data[tower_type]["basic"]
        self.timer = 0


#### ====================================================================================================================== ####
#############                                       TOWER_FUNCTIONS                                                #############
#### ====================================================================================================================== ####

def update_tower(tower, clicked, enemies, settings,game_data,check_for_enemies=False):
    ''' Helper function that updates a single provided Tower.
    Input: Tower Object, clicked (boolean), enemies (Enemy Object list), Settings Object, check_for_enemies (boolean)
    Output: Boolean or None
    '''
    pygame.mixer.pre_init(44100, -16, 2, 2)
    pygame.mixer.init()


    if tower.LockTarget == False:
       for enemy in enemies:
           distance = ((tower.location[0] + tower.sprite.get_size()[0]//2 - enemy.location[0] - settings.tile_size[0] // 2)**2 + (tower.location[1] + tower.sprite.get_size()[1]//2 - enemy.location[1] - settings.tile_size[1] // 2)**2)**0.5
           if distance <= tower.radius and enemy.health >0 :
              tower.LockTarget = enemy
              break
    if tower.LockTarget != False:
       distance = ((tower.location[0] + tower.sprite.get_size()[0]//2 - tower.LockTarget.location[0] - settings.tile_size[0] // 2)**2 + (tower.location[1] + tower.sprite.get_size()[1]//2 - tower.LockTarget.location[1] - settings.tile_size[1] // 2)**2)**0.5
       if distance > tower.radius or tower.LockTarget.health <=0 :
          tower.LockTarget = False
       if tower.LockTarget != False:
          if check_for_enemies and tower.basic != 'magnetic':
             tower.Fire = True
             game_data['shoot_mus'].play(0,0,100)
             tower.LockTarget.health -= tower.damage
             if tower.basic == "frozen":
                tower.LockTarget.speed = 0
          if check_for_enemies == False:
             tower.Fire = False
          relative_x = (tower.LockTarget.location[0] + settings.tile_size[0] // 2) - (tower.location[0] + tower.sprite.get_size()[0]//2)
          relative_y = (tower.LockTarget.location[1] + settings.tile_size[1] // 2) - (tower.location[1] + tower.sprite.get_size()[1]//2)
          if relative_x > 0:
             if relative_y > 0:
                angle = math.degrees(math.atan(relative_y/relative_x)) 
             elif relative_y < 0:
                angle = math.degrees(math.atan(relative_y/relative_x))+360
             elif relative_y == 0:
                angle = 0
          elif relative_x < 0:
             if relative_y > 0:
                angle = math.degrees(math.atan(relative_y/relative_x)) +180
             elif relative_y < 0:
                angle = math.degrees(math.atan(relative_y/relative_x)) + 180
             elif relative_y == 0:
                angle = 180
          elif relative_x == 0:
             if relative_y > 0:
                angle = 90
             elif relative_y < 0:
                angle = 270
             elif relative_y == 0:
                angle = 'error' 
          tower.angle = angle*-1 - 90


    if tower.basic == 'magnetic':
       flag =False
       for enemy in enemies:
          distance = ((tower.location[0] + tower.sprite.get_size()[0]//2 - enemy.location[0] - settings.tile_size[0] // 2)**2 + (tower.location[1] + tower.sprite.get_size()[1]//2 - enemy.location[1] - settings.tile_size[1] // 2)**2)**0.5
          if distance <= tower.radius and pygame.time.get_ticks() - tower.timer >= tower.rate_of_fire*500:
             tower.timer = pygame.time.get_ticks()
             enemy.health -= tower.damage
             game_data['magnetic_mus'].play(0,0,200)
             flag = True
             if tower.arrange == 0:
                tower.arrange = 50

       if not flag:
          if tower.arrange >= tower.radius:
             tower.arrange = 0

    if tower.basic == 'magnetic' and tower.arrange > 0:
       tower.arrange += 10
       if tower.arrange >= tower.radius:
          tower.arrange = 0

    # If tower is checking for enemies

    # Handle Tower Clickable
    (mX, mY) = pygame.mouse.get_pos()
    if clicked and (mX > tower.location[0] and mX < tower.location[0] + tower.sprite.get_rect().size[0] and mY > tower.location[1] and mY < tower.location[1] + tower.sprite.get_rect().size[1]):
        tower.isClicked = True
        return True
    elif clicked:
        tower.isClicked = False
    return tower.isClicked


def level_up(tower,game_data):
    if game_data['current_currency'] < 50:
       return tower
    if tower.level <= 2:
       tower.level += 1
       for tower_mode in csv_loader("data/towers.csv"):
           if int(tower_mode[5]) == tower.level and tower_mode[6] == tower.basic:
              tower.name = tower_mode[0]
              tower.sprite = pygame.image.load(tower_mode[1]).convert_alpha()
              tower.damage = int(tower_mode[2])
              tower.rate_of_fire = int(tower_mode[3])
              tower.radius = int(tower_mode[4])
              game_data['current_currency'] -= 50
    game_data['levelup'] = False
    return tower



def render_tower(tower, screen, settings):
    ''' Helper function that renders a single provided Tower.
    Input: Tower Object, screen (pygame display), Settings Object
    Output: None
    '''
    if tower.isClicked:
        screen.blit(pygame.transform.scale(tower.radius_sprite, (tower.radius * 2, tower.radius * 2)), [tower.location[0] + tower.sprite.get_size()[0]//2 - tower.radius, tower.location[1] + tower.sprite.get_size()[1]//2 - tower.radius])

    if tower.Fire and tower.LockTarget and tower.basic != 'magnetic':
        pygame.draw.line(screen,(255,0,0),(tower.location[0]+tower.sprite.get_size()[0]//2,tower.location[1]+tower.sprite.get_size()[1]//2),(tower.LockTarget.location[0] + settings.tile_size[0] // 2,tower.LockTarget.location[1] + settings.tile_size[1] // 2),3)

    if tower.basic == 'magnetic' and tower.arrange != 0:
        if tower.level  == 1:
           color = (0,255,0)
        elif tower.level == 2:
           color = (0,0,255)
        elif tower.level == 3:
           color = (255,0,0)
        pygame.draw.circle(screen,color,(tower.location[0]+tower.sprite.get_size()[0]//2,tower.location[1]+tower.sprite.get_size()[1]//2),tower.arrange,5)

    sprite = pygame.transform.rotate(tower.sprite,tower.angle)
    pos = (tower.location[0] + tower.sprite.get_size()[0]//2 - sprite.get_size()[0]//2, tower.location[1]+tower.sprite.get_size()[1]//2 - sprite.get_size()[1]//2)

    screen.blit(sprite, pos)
