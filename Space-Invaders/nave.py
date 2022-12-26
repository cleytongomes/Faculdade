'''
	FUNÇÕES DE CONTROLE DA NAVE
'''

#IMPORTANDO AS LIBS
from PPlay.window import *
from PPlay.gameimage import *
from PPlay.sprite import *
from PPlay.keyboard import *
from PPlay.mouse import *
from cons import *

#FUNÇÃO DE CONTROLE DAS NAVES
def Nave(nave,janela, mouse, teclado):
	global VELOCIDADE_NAVE, VIDAS

	if(teclado.key_pressed("A") or teclado.key_pressed("LEFT")):
		delta = janela.delta_time()
		nave.x -= (VELOCIDADE_NAVE[0] * delta) + 0.01
	elif(teclado.key_pressed("D") or teclado.key_pressed("RIGHT")):
		delta = janela.delta_time()
		nave.x += (VELOCIDADE_NAVE[0] * delta) + 0.01

	if(teclado.key_pressed("T") or teclado.key_pressed("SPACE")):
		global LAST_TIRO

		time = janela.time_elapsed()
		if(time - LAST_TIRO > TEMPO_RECARGA[0]):
			tiro = Sprite(IMG_TIRO)
			tiro.set_position(nave.x + (nave.width / 2), nave.y - tiro.height)
			tiros.append(tiro)
			LAST_TIRO = time

	if(nave.x < 0):
		nave.x = MENU_WIDTH - nave.width
	elif(nave.x + nave.width > MENU_WIDTH):
		nave.x = 0 


#VERIFICA A COLISÃO DA NAVE COM TIROS DOS ALIENÍGENAS
def colisao_nave(nave):
	#IMPORTANDO VARIÁVEIS
	global VIDAS

	cont = 0
	for tiro in tiros_monstros:
		if(tiro.y >= MENU_HEIGHT):
			tiros_monstros.pop(cont)
		elif(tiro.y + tiro.height >= nave.y):
			if(tiro.collided(nave)):
				VIDAS[0] -= 1
				tiros_monstros.pop(cont)
		cont+=1