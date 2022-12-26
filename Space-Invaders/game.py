from PPlay.window import *
from PPlay.gameimage import *
from PPlay.sprite import *
from PPlay.keyboard import *
from PPlay.mouse import *

from cons import *
from game import *
from nave import *
from monsters import *
from pause import *
from random import randint
from operator import itemgetter


def game(janela, mouse, teclado):
	#IMPORTA AS VARIÁVEIS GLOBAIS
	global MONSTROS, tiros, F_JOGO, VIDAS
	
	#LIMPA A MATRIZ DE MONSTROS
	limpa_monstros();

	#REINICIA PONTUAÇÃO
	PONTOS[0] = 0
	F_JOGO[0] = 0
	VIDAS[0] = 3
	
	#INSERE UM FUNDO PRETO
	fundo = GameImage(FUNDO_BLACK)
	
	#SPRITE
	nave = Sprite(NAVE)
	nave.set_position((MENU_WIDTH/2) - (nave.width/2), (80/100)*MENU_HEIGHT)

	#INICIALIZA UMA NOVA MATRIZ DE MONSTROS
	inicializaMonstros(janela)

	#DESCOBRE O FPS DO JOGO
	time_frame = janela.time_elapsed()
	fps = str(round(1/janela.delta_time()))

	#VARIÁVEL DE CONTROLE DO PAUSE
	bloqueio_pause = 1

	#VARIÁVEL PARA QUEBRAR O LOOP DE JOGO
	estado_jogo = 1
	while(estado_jogo):

		#PAUSA O JOGO
		if(teclado.key_pressed("ESC")):
			if(bloqueio_pause == 0):				
				p = Pause(janela, teclado, mouse)
			if(p.exit):
				break

			bloqueio_pause = 1
		else:
			bloqueio_pause = 0

		#REINICIA O FUNDO DO JOGO
		fundo.draw()			

		#CONTROLE DA NAVE E DOS ALIENÍGENAS
		Nave(nave,janela, mouse, teclado)			
		Monsters(janela)
		
		#CONTROLE FIM DE JOGO
		[nave, estado_jogo] = fim_jogo(nave)
		
		#DESENHA A NAVE
		nave.draw()

		#DESENHA OS TIROS DA NAVE
		for tiro in tiros:
			delta = janela.delta_time()
			tiro.y -= (VELOCIDADE_TIRO[0] * delta) + 0.01
			tiro.draw();
		
		colisao_monstros()
		colisao_nave(nave)
		
		#APRESENTA AS INFORMAÇÕES
		time_frame, fps = printa_infos(janela, time_frame, fps)

		#ATUALIZA A JANELA
		janela.update()

	if(not estado_jogo):
		termino_jogo(janela, mouse, teclado)

#APRESENTA AS INFORMAÇÕES NA TELA DO JOGO
def printa_infos(janela, time_frame, fps):
	janela.draw_text("FPS : "+fps+" ", 10, 10, size=20, color=(255,0,0), font_name='Times', bold=False, italic=False)
	janela.draw_text("VIDAS : "+str(VIDAS[0])+" ", 10, janela.height-50, size=20, color=(255,0,0), font_name='Times', bold=False, italic=False)
	janela.draw_text("PONTOS : "+str(PONTOS[0])+" ", 10, janela.height-25, size=20, color=(255,0,0), font_name='Times', bold=False, italic=False)	

	if(time_frame + 1000 < janela.time_elapsed()):
		time_frame = janela.time_elapsed()
		if(janela.delta_time() != 0):
			fps = str(round(1/janela.delta_time()))

	return [time_frame, fps]

#TERMINA O JOGO E ADD AO BD 
def termino_jogo(janela, mouse, teclado):
	fim_de_jogo = Sprite(IMAGEM_FIM_JOGO)
	fim_de_jogo.set_position(janela.width/2 - fim_de_jogo.width/2,janela.height/2 - fim_de_jogo.height/2)
	fim_de_jogo.draw()
	janela.update()

	nome_jogador = input("DIGITE SEU NOMBRE : ")
	while nome_jogador == "":
		nome = input("NOME INVÁLIDO, DIGITE SEU NOMBRE NOVAMENTE : ")
		fim_de_jogo.draw()
		janela.update()
		nome_jogador = input("DIGITE SEU NOMBRE : ")

	arq_placar = open("placar.txt")
	placares = []
	for linha in arq_placar:
		nome, pontos = linha.split("#")
		placares.append((nome, int(pontos)))

	
	placares.append((nome_jogador, PONTOS[0]))

	arq_placar = open("placar.txt", 'w')
	
	placares.sort(key=itemgetter(1), reverse=True)

	contador = 1
	for placar in placares:
		arq_placar.write(str(placar[0]))
		arq_placar.write("#")
		arq_placar.write(str(placar[1]))
		arq_placar.write("\n")
		if contador == 10:
			break
		contador += 1

	arq_placar.close()

#FUNÇÃO QUE VERIFICA SE A NAVE AINDA TEM VIDAS
def fim_jogo(nave):
	global VIDAS, F_JOGO
	estado_jogo = 1

	#print(POS_MATRIZ_B[0])
	if(VIDAS[0] < 0 or F_JOGO[0]):
		#FIM DE JOGO	
		VIDAS[0] = 0
		x = nave.x
		y = nave.y
		nave = Sprite(NAVE_DESTRUIDA)
		nave.set_position(x, y)
		estado_jogo = 0

	return [nave, estado_jogo]