'''
	INICIANDO O JOGO PELO MENU PRINCIPAL
'''

from PPlay.window import *
from PPlay.gameimage import *
from PPlay.sprite import *
from PPlay.keyboard import *
from PPlay.mouse import *

from cons import *
from buton_click import *
from game import *
from config import *
from placar import *

#Iniciando a Janela
janela = Window(MENU_WIDTH,MENU_HEIGHT)
janela.set_title("Jogo")

#Atribuições de Controladores
teclado = Window.get_keyboard()
mouse = Window.get_mouse()

#Atribuições dos game objects
fundo = GameImage(IMAGEM_MENU)
logo = GameImage(IMAGEM_LOGO)


#Butons do Menu
b_play = Sprite(BUTTON_PLAY_W)
b_nivel = Sprite(BUTTON_NIVEL_W)
b_placar = Sprite(BUTTON_PLACAR_W)  
b_sair = Sprite(BUTTON_SAIR_W)


#Setando a Posição dos Butons
altura_botao = (MENU_HEIGHT)/4
alt_min = b_play.height/4
logo.set_position((MENU_WIDTH/2) - (logo.width/2), (1/100)*MENU_HEIGHT)
b_play.set_position((MENU_WIDTH/4) - (b_play.width/2), MENU_HEIGHT / 2 - alt_min * 3)
b_nivel.set_position((MENU_WIDTH/4) - (b_play.width/2), MENU_HEIGHT / 2 + alt_min * 2)
b_placar.set_position( MENU_WIDTH/2 + ((MENU_WIDTH/4) - (b_play.width/2)), MENU_HEIGHT / 2 - alt_min * 3)
b_sair.set_position( MENU_WIDTH/2 + (MENU_WIDTH/4) - (b_play.width/2), MENU_HEIGHT / 2 + alt_min * 2)
butons = [b_play, b_nivel, b_placar, b_sair]

#Variável de estado do click
estado_mouse = 0;

while(1):

	#Recebe qual o botão está com o Mouse em cima
	bt = hover(mouse, butons)	
	if(bt == 1):
		butons = [Sprite(BUTTON_PLAY_R),b_nivel, b_placar, b_sair]
		butons[0].set_position((MENU_WIDTH/4) - (b_play.width/2), MENU_HEIGHT / 2 - alt_min * 3)
	elif(bt == 2):
		butons = [b_play,Sprite(BUTTON_NIVEL_R), b_placar, b_sair]
		butons[1].set_position((MENU_WIDTH/4) - (b_play.width/2), MENU_HEIGHT / 2 + alt_min * 2)
	elif(bt == 3):
		butons = [b_play, b_nivel, Sprite(BUTTON_PLACAR_R), b_sair]
		butons[2].set_position( MENU_WIDTH/2 + ((MENU_WIDTH/4) - (b_play.width/2)), MENU_HEIGHT / 2 - alt_min * 3)
	elif(bt == 4):
		butons = [b_play,b_nivel, b_placar, Sprite(BUTTON_SAIR_R)]
		butons[3].set_position( MENU_WIDTH/2 + (MENU_WIDTH/4) - (b_play.width/2), MENU_HEIGHT / 2 + alt_min * 2)
	else:
		butons = [b_play,b_nivel, b_placar, b_sair]

	#Pega o evento de Click
	if(mouse.is_button_pressed(1) and (not estado_mouse)):
		bt = click(mouse, butons)
		if(bt == 1):
			#VAI PARA O JOGO
			game(janela, mouse, teclado)

		elif(bt == 2):
			#VAI PARA AS CONFIGURAÇÕES
			config(janela, mouse, teclado)

		elif(bt == 3):
			#VAI PARA OS PLACARES
			placar(janela, mouse, teclado)

		elif(bt == 4):
			#SAI DO JOGO
			janela.close()

		estado_mouse = 1
	if((not mouse.is_button_pressed(1)) and estado_mouse):
		estado_mouse = False

	#Reseta o fundo
	fundo.draw()
	
	#Desenha os objetos
	logo.draw()
	for b in butons:
		b.draw()

	#Atualiza a janela
	janela.update()