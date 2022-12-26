from PPlay.window import *
from PPlay.gameimage import *
from PPlay.sprite import *
from PPlay.keyboard import *
from PPlay.mouse import *

from cons import *
from buton_click import *
from game import *


def config(janela, mouse, teclado):

	global DIFICULDADE
	global VELOCIDADE_NAVE, VELOCIDADE_TIRO, VELOCIDADE_MONSTRO
	global TEMPO_RECARGA

	fundo = GameImage(FUNDO_JOGO)
	b_facil = Sprite(B_FACIL_W)
	b_medio = Sprite(B_MEDIO_W)
	b_dificil = Sprite(B_DIFICIL_W)

	altura_botao = (MENU_HEIGHT)/3
	alt_min = b_facil.height/3

	b_facil.set_position((MENU_WIDTH/2) - (b_facil.width/2), alt_min )
	b_medio.set_position((MENU_WIDTH/2) - (b_facil.width/2), alt_min + altura_botao)
	b_dificil.set_position((MENU_WIDTH/2) - (b_facil.width/2), alt_min + altura_botao * 2)

	buttons = [b_facil, b_medio, b_dificil]

	estado_mouse = 1;

	while(1):
		if(teclado.key_pressed("ESC")):
			return 0

		bt = hover(mouse, buttons)	
		if(bt == 1):
			buttons = [Sprite(B_FACIL_R),b_medio, b_dificil]
			buttons[0].set_position((MENU_WIDTH/2) - (b_facil.width/2), alt_min )
		elif(bt == 2):
			buttons = [b_facil,Sprite(B_MEDIO_R), b_dificil]
			buttons[1].set_position((MENU_WIDTH/2) - (b_facil.width/2), alt_min + altura_botao)
		elif(bt == 3):
			buttons = [b_facil, b_medio, Sprite(B_DIFICIL_R)]
			buttons[2].set_position((MENU_WIDTH/2) - (b_facil.width/2), alt_min + altura_botao * 2)
		else:
			buttons = [b_facil, b_medio, b_dificil]	

		if(mouse.is_button_pressed(1) and (not estado_mouse)):
			bt = click(mouse, buttons)

			if(bt == 1):


				#FACIL
				DIFICULDADE[0] = 1
				VELOCIDADE_NAVE[0] = 300
				VELOCIDADE_TIRO[0] = 250	
				TEMPO_RECARGA[0] = 200
				VELOCIDADE_MONSTRO[0] = 30

				game(janela, mouse, teclado)

			elif(bt == 2):
				#MÉDIO
				DIFICULDADE[0] = 2
				VELOCIDADE_NAVE[0] = 130
				VELOCIDADE_TIRO[0] = 70	
				TEMPO_RECARGA[0] = 400
				VELOCIDADE_MONSTRO[0] = 50

				game(janela, mouse, teclado)

			elif(bt == 3):
				#DIFÍCIL
				print("T: ",TEMPO_RECARGA)
				DIFICULDADE[0] = 3
				VELOCIDADE_NAVE[0] = 100
				VELOCIDADE_TIRO[0] = 400	
				TEMPO_RECARGA[0] = 750
				VELOCIDADE_MONSTRO[0] = 75

				game(janela, mouse, teclado)

			estado_mouse = 1

		if((not mouse.is_button_pressed(1)) and estado_mouse):
			estado_mouse = False

		#Desenha os game_objects
		fundo.draw()

		for but in buttons:
			but.draw()

		#Atualiza a janela
		janela.update()