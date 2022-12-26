from PPlay.window import *
from PPlay.gameimage import *
from PPlay.sprite import *
from PPlay.keyboard import *
from PPlay.mouse import *

from cons import *
from game import *


def placar(janela, mouse, teclado):

	fundo = GameImage(FUNDO_JOGO)

	placar = open('placar.txt', 'r')
	placares = []

	for linha in placar:
		placares.append(linha)

	placar.close()

	descritor_tela_placar = Sprite("descritor_placar.png")
	descritor_tela_placar.set_position(janela.width/2 - descritor_tela_placar.width/2, 50)

	while(1):
		if(teclado.key_pressed("ESC")):
			return 0

		#Reinicia o fundo do jogo
		fundo.draw()

		#Desenha os sprites e escreve os placares na tela
		descritor_tela_placar.draw()

		identificador = 1
		margem_topo_1 = 175
		margem_topo_2 = 175
		for placar in placares:

			#QUEBRA O TEXTO
			placar = placar.replace('\n', '')
			nome, pontos = placar.split("#")

			#COLOCA OS PRIMEIROS CINCO A ESQUERDA
			if(identificador <= 5 ):
				janela.draw_text(str(identificador)+"  "+nome+"   "+pontos+" ptos", 100, margem_topo_1, size=40, color=(0,0,0), font_name='Times', bold=False, italic=False)
				identificador += 1
				margem_topo_1 += 55

			#COLOCA OS ÚLTIMOS CINCO A DIREITA
			else:
				janela.draw_text(str(identificador)+"  "+nome+"   "+pontos+" ptos", (janela.width/2) + 100, margem_topo_2, size=40, color=(0,0,0), font_name='Times', bold=False, italic=False)
				identificador += 1
				margem_topo_2 += 55

		#Atualiza a janela
		janela.update()