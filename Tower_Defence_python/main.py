#### ====================================================================================================================== ####
#############                                           IMPORTS                                                    #############
#### ====================================================================================================================== ####

from helper_functions import *
from settings import *
from shop import *
from tower import *
from enemy import *
from map import *
import pygame
import sys
import random
import time


#### ====================================================================================================================== ####
#############                                         INITIALIZE                                                   #############
#### ====================================================================================================================== ####

def initialize(m_number = 'map1'):
    ''' Initialization function - initializes various aspects of the game including settings, shop, and more.
    Input: None
    Output: game_data dictionary
    '''
    # Initialize Pygame
    pygame.mixer.pre_init(44100, -16, 2, 2)
    pygame.mixer.init()
    pygame.init()
    pygame.display.set_caption("COMP 1501 - Tutorial Bonus: Tower Defense - Improved")

    # Initialize the Settings Object
    settings = Settings()

    # Initialize game_data and return it
    game_data = { "screen": pygame.display.set_mode(settings.window_size),
                  "current_currency": settings.starting_currency,
                  "current_wave": 0,
                  "stay_open": True,
                  "map_number" : m_number,
                  "selected_tower": None,
                  "clicked": False,
                  "levelup": False,
                  "settings": settings,
                  "towers": [],
                  "enemies": [],
                  "shop": Shop("Space", settings),
                  "map": Map(settings,m_number),
                  "enemy_timer": pygame.time.get_ticks(),
                  "enemy_spawn_interval": 2,
                  "tower_timer": pygame.time.get_ticks(),
                  "playing" : True,
                  "current_kill" : 0,
                  "note":'',
                  "placement_mus" : pygame.mixer.Sound("assets/music/placement.wav"),
                  "shoot_mus" : pygame.mixer.Sound("assets/music/biu.wav"),
                  "magnetic_mus": pygame.mixer.Sound("assets/music/magnetic.wav"),
                  "monster_mus": pygame.mixer.Sound("assets/music/monster.wav"),
                  "tower_tick_interval": 0.15 }

    return game_data

#### ====================================================================================================================== ####
#############                                           PROCESS                                                    #############
#### ====================================================================================================================== ####

def process(game_data):
    ''' Processing function - handles all form of user input. Raises flags to trigger certain actions in Update().
    Input: game_data dictionary
    Output: None
    '''
    for event in pygame.event.get():

        # Handle [X] press
        if event.type == pygame.QUIT:
            game_data["stay_open"] = False

        # Handle Mouse Button Down
        if event.type == pygame.MOUSEBUTTONDOWN:
            game_data["clicked"] = True
            game_data["selected_tower"] = False

        # Handle Mouse Button Up
        if event.type == pygame.MOUSEBUTTONUP:
            game_data["clicked"] = False
	
        if event.type == pygame.KEYDOWN:
            if pygame.key.get_pressed()[pygame.K_UP]:
               game_data['levelup'] = True
        if event.type == pygame.KEYUP:
            game_data['levelup'] = False

#### ====================================================================================================================== ####
#############                                            UPDATE                                                    #############
#### ====================================================================================================================== ####

def update(game_data):
    ''' Updating function - handles all the modifications to the game_data objects (other than boolean flags).
    Input: game_data
    Output: None
    '''
    # Handle Tower interaction (and Tower Placement)
    tower = update_shop(game_data["shop"], game_data["current_currency"], game_data["settings"], game_data["clicked"])
    if tower is not None:
        if check_location(game_data,game_data["map"], game_data["settings"], tower["Location"], placement=True):
            game_data["towers"].append(Tower(tower["Type"], tower["Location"], tower["Radius Sprite"]))
            game_data["current_currency"] -= tower["Cost"]
            game_data['placement_mus'].play()
            

    # Handle enemy Spawning
    game_data["enemy_spawn_interval"] = 100000000/pygame.time.get_ticks()
    if game_data["enemy_spawn_interval"] >= 3000:
       game_data["enemy_spawn_interval"] = 3000 
    elif game_data["enemy_spawn_interval"] <= 200:
       game_data["enemy_spawn_interval"] = 200
    if (pygame.time.get_ticks() - game_data["enemy_timer"]) > game_data["enemy_spawn_interval"]:
        game_data["enemy_timer"] = pygame.time.get_ticks()
        name_list = []
        game_data['note'] = ''
        for i in range(len(csv_loader("data/enemies.csv"))):
            name = csv_loader("data/enemies.csv")[i][0]
            if name != 'Name':
               weight = int(20*(len(csv_loader("data/enemies.csv"))-i) + (pygame.time.get_ticks()/10000)**(i-len(csv_loader("data/enemies.csv"))/2))
               for times in range(weight):
                   name_list.append(name)
        if  game_data["enemy_spawn_interval"] >= 3000:
            for i in range(name_list.count('Titan')):
                  name_list.remove('Titan')
            random_result = name_list[random.randint(0,len(name_list)-1)]
        elif 200 < game_data["enemy_spawn_interval"] < 3000:
            if 2500 <  game_data["enemy_spawn_interval"] < 3000:
               game_data['note'] = 'They start to increase the frequency!'
               for i in range(name_list.count('Titan')):
                  name_list.remove('Titan')
            if 200 < game_data["enemy_spawn_interval"] < 1000:
               game_data['note'] = 'They are about to make a general offensive!'
            random_result = name_list[random.randint(0,len(name_list)-1)]
        elif game_data["enemy_spawn_interval"] <= 200:
            for i in range(name_list.count('Lesser Alien')):
               name_list.remove('Lesser Alien')

            random_result = name_list[random.randint(0,len(name_list)-1)]
        if random_result == 'Titan':
           game_data['monster_mus'].play(0,0,100)
        game_data["enemies"].append(Enemy(random_result, get_starting_tile(game_data["map"], game_data["settings"]), game_data["settings"]))
    
    # Update each Enemy in Enemy list
    for i in range(len(game_data["enemies"])):
        if check_location(game_data,game_data["map"], game_data["settings"], location=game_data["enemies"][i].location, direction=game_data["enemies"][i].direction) and game_data["enemies"][i].reverse_direction != game_data["enemies"][i].direction:
           if update_enemy(game_data["enemies"][i], direction=game_data["enemies"][i].direction) == False:
              game_data["current_currency"] += game_data["enemies"][i].points
              game_data["enemies"].pop(i)
              game_data["current_kill"] += 1
              break


        else:
            if game_data["enemies"][i].direction == "S":
                game_data["enemies"][i].direction = "E"
            elif game_data["enemies"][i].direction == "E":
                game_data["enemies"][i].direction = "N"
            elif game_data["enemies"][i].direction == "N":
                game_data["enemies"][i].direction = "W"
            else:
                game_data["enemies"][i].direction = "S"
    
    # Handle Tower Clicking (selection)
    clicked_tower = False

    for tower in game_data["towers"]:
        if (pygame.time.get_ticks() - tower.timer) / 1000 > tower.rate_of_fire:
            tower.timer = pygame.time.get_ticks()
            clicked_tower = update_tower(tower, game_data["clicked"], game_data["enemies"], game_data["settings"], game_data, True)
        else:
            clicked_tower = update_tower(tower, game_data["clicked"], game_data["enemies"], game_data["settings"], game_data)
        if clicked_tower:
            game_data["selected_tower"] = tower
            if game_data['levelup']:
               tower = level_up(tower,game_data)
            break
    if not clicked_tower:
        game_data["selected_tower"] = None
        


#### ====================================================================================================================== ####
#############                                            RENDER                                                    #############
#### ====================================================================================================================== ####

def render(game_data):
    ''' Rendering function - displays all objects to screen.
    Input: game_data
    Output: None
    '''

    if game_data['map_number'] == 'map1':
       game_data['screen'].fill((39,174,96))
    else:
       game_data['screen'].fill((236,219,182))
    render_map(game_data["map"], game_data["screen"], game_data["settings"])
    render_shop(game_data["shop"], game_data["screen"], game_data["settings"], game_data["current_currency"],game_data['current_kill'])
    for enemy in game_data["enemies"]:
        render_enemy(enemy, game_data["screen"], game_data["settings"])
    for tower in game_data["towers"]:
        render_tower(tower, game_data["screen"], game_data["settings"])
    if game_data['note'] != '':
        t = pygame.font.SysFont("monospace", 24,True)
        t_ima = t.render(game_data['note'],1,(0, 0, 0))
        game_data['screen'].blit(t_ima,(game_data["settings"].window_size[0]//2-t_ima.get_size()[0]//2,400))
    pygame.display.update()



def render_result(game_data):
    game_data['screen'].fill((0,0,0))
    text = game_data['settings'].font.render('You killed   '+ str(game_data['current_kill']) + '   Aliens', True, (254, 207, 0))
    game_data['screen'].blit(text,((game_data['settings'].window_size[0]-text.get_size()[0])//2,(game_data['settings'].window_size[1] -text.get_size()[1])//2))
    pygame.display.update()
    
#### ====================================================================================================================== ####
#############                                             MAIN                                                     #############
#### ====================================================================================================================== ####

def main():
    ''' Main function - initializes everything and then enters the primary game loop.
    Input: None
    Output: None
    '''
    flag = True
    game_data = initialize()
    lines =[]
    gold_color = (254, 207, 0)
    for i in range(random.randint(20,25)):
        x1 = random.randint(-400,game_data["settings"].window_size[0]-200)
        y1 = game_data["settings"].window_size[1] + random.randint(0,450)
        d = random.randint(150,450)
        x2 = x1 - d
        y2 = y1 + d
        speed = random.randint(3,8)
        color = [random.randint(0,255),random.randint(0,255),random.randint(0,255)]
        coor = [x1,y1,x2,y2,speed,color]
        lines.append(coor)
    while flag:
        game_data['screen'].fill((0,0,0))
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                  return
            if event.type == pygame.KEYDOWN:
               if pygame.key.get_pressed()[pygame.K_F1]:
                  flag = False
                  m_number = 'map1'
               elif pygame.key.get_pressed()[pygame.K_F2]:
                  flag = False
                  m_number = 'map2'
               if pygame.key.get_pressed()[pygame.K_ESCAPE]:
                  return
        for i in lines:
            if i[2] > game_data["settings"].window_size[0] or i[3] < 0:

               i[0] = random.randint(-400,game_data["settings"].window_size[0]-200)
               i[1] = random.randint(game_data["settings"].window_size[1] - 250,game_data["settings"].window_size[1] + 250)
               d = random.randint(150,450)
               i[2] = i[0] - d
               i[3] = i[1] + d
               i[4] = random.randint(3,8)
               i[5] = [random.randint(0,255),random.randint(0,255),random.randint(0,255)]
            i[0] += i[4]
            i[1] -= i[4]
            i[2] += i[4]
            i[3] -= i[4]
            i[5][0] += 1
            i[5][1] += 1
            i[5][2] += 1
            if i[5][0] >= 255:
               i[5][0] = 255
            if i[5][1] >= 255:
               i[5][1] = 255
            if i[5][2] >= 255:
               i[5][2] = 255
            pygame.draw.line(game_data['screen'],(i[5][0],i[5][1],i[5][2]),(i[0],i[1]),(i[2],i[3]),5)
        
        text1 = 'Humans VS Aliens' 
        t1 = pygame.font.SysFont("monospace", 36,True)
        t = pygame.font.SysFont("monospace", 20,True)
        t1_ima = t1.render(text1,1,gold_color)
        game_data['screen'].blit(t1_ima,(game_data["settings"].window_size[0]//2-t1_ima.get_size()[0]//2,160))


        text2 = ['Bulid tower to defend your city.',
                 'Different towers have different attributes.',
                 'Click a built tower can check it\'s attack radius.',
                 'When checking a tower, you can press UP key to update it if it can be updated.']


        for i in range(len(text2)):
            t_ima = t.render(text2[i],1,gold_color)
            game_data['screen'].blit(t_ima,(game_data["settings"].window_size[0]//2-t_ima.get_size()[0]//2,180+45*(i+1)))

        text3 = 'Press F1 key /          \ Press F2 key'
        text4 =    'to play map1   ------------   to play map2'
        text5 = 'Press ESCAPE key'
        text6 = 'to exit'

        t3_ima = t1.render(text3,1,gold_color)
        game_data['screen'].blit(t3_ima,(game_data["settings"].window_size[0]//2-t3_ima.get_size()[0]//2,410))

        t4_ima = t.render(text4,1,gold_color)
        game_data['screen'].blit(t4_ima,(game_data["settings"].window_size[0]//2-t4_ima.get_size()[0]//2,460))

        t5_ima = t.render(text5,1,gold_color)
        game_data['screen'].blit(t5_ima,(game_data["settings"].window_size[0]//2-t5_ima.get_size()[0]//2,490))

        t6_ima = t.render(text6,1,gold_color)
        game_data['screen'].blit(t6_ima,(game_data["settings"].window_size[0]//2-t6_ima.get_size()[0]//2,530))

        pygame.display.flip()
        time.sleep(0.04)

    game_data = initialize(m_number)

    while game_data["stay_open"]:
        if not game_data["playing"]:
           render_result(game_data)
           process(game_data)
        else:
           process(game_data)
           update(game_data)
           render(game_data)
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
