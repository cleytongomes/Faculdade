'''
	FUNÇÕES DE CONTROLE DOS MONSTROS
'''

#IMPORTANDO AS LIBS
from PPlay.window import *
from PPlay.gameimage import *
from PPlay.sprite import *
from PPlay.keyboard import *
from PPlay.mouse import *
from cons import *
from random import randint

#CONTROLE DOS MONSTROS
def Monsters(janela):
	#IMPORTA AS VARIÁVEIS GLOBAIS
	global LAST_TIRO_MONSTRO, RECARGA_MONSTRO, MONSTROS, tiros
	
	#ATIRANDO COM OS MONSTROS 
	time = janela.time_elapsed()
	if(time - LAST_TIRO_MONSTRO > RECARGA_MONSTRO):
			
		possiveis = []
		for vet in MONSTROS:
			for monstro in vet:
				if(monstro != 0 and monstro != 1):
					possiveis.append(monstro)
		
		if(len(possiveis)!=0):
			p = possiveis[randint(0, len(possiveis)-1)]
			tiro = Sprite(IMG_TIRO_MONSTRO)
			tiro.set_position(p.x + (p.width / 2), p.y + p.height)
			tiros_monstros.append(tiro)
			LAST_TIRO_MONSTRO = time
			RECARGA_MONSTRO = randint(TEMP_MIN_TIRO_MOSTRO[0],TEMP_MAX_TIRO_MOSTRO[0])

	#MOVIMENTANDO OS TIROS DOS MONSTROS
	for tiro in tiros_monstros:
		delta = janela.delta_time()
		tiro.y += (VELOCIDADE_TIRO[0] * delta) + 0.01
		tiro.draw();

	retorno = moveMatrizMonstros(janela)
	if(retorno):
		limpa_monstros()
		inicializaMonstros(janela)
		VELOCIDADE_MONSTRO[0] += 50

#CRIA A MATRIZ DE MONSTROS
def inicializaMonstros(janela):
	#IMPORTA AS VARIÁVEIS GLOBAIS
	global SENTIDO_MOV_MONSTRO, ALTURA, LARGURA
	global POS_ESPECIAL_x, POS_ESPECIAL_Y

	#CRIA UM NOVO MONSTRO
	monstro = Sprite(MONSTRO_SPRITE, 10);

	#DEFININDO OS TAMANHOS DA MATRIZ
	LARGURA = int(janela.width/monstro.width) - 2
	ALTURA = int(janela.height/monstro.height) - 4
	
	pos_aloc_matriz_x = monstro.width
	pos_aloc_matriz_y = monstro.height

	for x in range(1,ALTURA):
		vetor = []
		for y in range(1,LARGURA):
			m = Sprite(MONSTRO_SPRITE,10);
			m.set_position(pos_aloc_matriz_x , pos_aloc_matriz_y)
			m.set_total_duration(400)
			vetor.append(m)
			pos_aloc_matriz_x = pos_aloc_matriz_x + monstro.width + (monstro.width/5)

		MONSTROS.append(vetor)
		pos_aloc_matriz_y = pos_aloc_matriz_y + monstro.height + (monstro.height/5)
		pos_aloc_matriz_x = monstro.width			



	i = randint(1, LARGURA-3)
	j = randint(1, ALTURA-3)
	print(i, j)
	POS_ESPECIAL_X[0] = MONSTROS[j][i].x
	POS_ESPECIAL_Y[0] = MONSTROS[j][i].y
	MONSTROS[j][i] = 1


	obtemPosMatriz()

#LIMPA A MATRIZ DE MONSTROS
def limpa_monstros():
	#IMPORTA AS VARIÁVEIS GLOBAIS
	global MONSTROS, tiros
	tiros.clear()
	MONSTROS.clear()

#MOVE A MATRIZ DE MONSTROS
def moveMatrizMonstros(janela):
	global POS_MATRIZ_D, POS_MATRIZ_S, SENTIDO_MOV_MONSTRO
	global ALTURA, LARGURA, VELOCIDADE_MONSTRO
	global POS_ESPECIAL_x, POS_ESPECIAL_Y

	for vet in MONSTROS:			
		for monstro in vet:
			if(monstro != 0):

				if(monstro == 1):
					especial = Sprite("sprites/especial.png")
					especial.set_position(POS_ESPECIAL_X[0], POS_ESPECIAL_Y[0])
					especial.x += ((VELOCIDADE_MONSTRO[0] * delta) + 0.01) * SENTIDO_MOV_MONSTRO
					especial.draw()
					POS_ESPECIAL_X[0] = especial.x
				else:		
					delta = janela.delta_time()
					monstro.x += ((VELOCIDADE_MONSTRO[0] * delta) + 0.01) * SENTIDO_MOV_MONSTRO 
					monstro.update()
					monstro.draw()
	

	if(POS_MATRIZ_D > janela.width or POS_MATRIZ_E < 0):
		moveMatrizMonstrosBaixo(janela)
		if(SENTIDO_MOV_MONSTRO == 1): 
			SENTIDO_MOV_MONSTRO = - 1
		else:
			SENTIDO_MOV_MONSTRO = 1

	return obtemPosMatriz()

#MOVE A MATRIZ DE MONSTROS PARA BAIXO
def moveMatrizMonstrosBaixo(janela):
	global SENTIDO_MOV_MONSTRO
	global POS_ESPECIAL_X, POS_ESPECIAL_Y

	for vet in MONSTROS:
		for monstro in vet:
			if(monstro != 0):

				if(SENTIDO_MOV_MONSTRO == 1):	
					if(monstro == 1):
						especial = Sprite("sprites/especial.png")
						especial.set_position(POS_ESPECIAL_X[0], POS_ESPECIAL_Y[0])
						especial.x -= 10
						especial.draw()
						POS_ESPECIAL_X[0] = especial.x
					else:
						monstro.x -= 10
				else:
					if(monstro == 1):
						especial = Sprite("sprites/especial.png")
						especial.set_position(POS_ESPECIAL_X[0], POS_ESPECIAL_Y[0])
						especial.x += 10
						especial.draw()
						POS_ESPECIAL_X[0] = especial.x
					else:
						monstro.x += 10
	
				if(monstro == 1):
					especial = Sprite("sprites/especial.png")
					especial.set_position(POS_ESPECIAL_Y[0], POS_ESPECIAL_Y[0])
					especial.y += 1
					especial.draw()
					POS_ESPECIAL_Y[0] = especial.y
				else:
					monstro.y += 1
					monstro.draw()

#OBTEM AS POSIÇÕES EXTRATÉGICAS DOS MONSTROS
def obtemPosMatriz():
	global POS_MATRIZ_D, POS_MATRIZ_E, POS_MATRIZ_B
	global ALTURA, LARGURA
	global POS_ESPECIAL_X, POS_ESPECIAL_Y

	def esquerda():
		for j in range(LARGURA-1):
			for i in range(ALTURA-1):
				if(MONSTROS[i][j] != 0):
					if(MONSTROS[i][j] == 1):
						return POS_ESPECIAL_X[0]
					else:
						return MONSTROS[i][j].x

	def direita():
		for j in range(LARGURA-2,-1,-1):
			for i in range(ALTURA-1):
				if(MONSTROS[i][j] != 0):
					if(MONSTROS[i][j] == 1):
						return POS_ESPECIAL_X[0] + 52
					else:
						return MONSTROS[i][j].x + MONSTROS[i][j].width

	def baixo():
		for i in range(ALTURA-2, -1, -1):
			for j in range(LARGURA-1):
				if(MONSTROS[i][j] != 0 and MONSTROS[i][j] != 1):
					if(MONSTROS[i][j] == 1):
						return POS_ESPECIAL_Y[0] + 72
					else:
						return MONSTROS[i][j].y + MONSTROS[i][j].height
	

	POS_MATRIZ_B = baixo()
	POS_MATRIZ_B = POS_MATRIZ_B if POS_MATRIZ_B else 0
	POS_MATRIZ_E = esquerda()
	POS_MATRIZ_E = POS_MATRIZ_E if POS_MATRIZ_E else 0
	POS_MATRIZ_D = direita()
	POS_MATRIZ_D = POS_MATRIZ_D if POS_MATRIZ_D else 0

	if(POS_MATRIZ_D == 0 and POS_MATRIZ_E == 0):
		return 1;
	else:
		return 0;


#VERIFICA COLISÃO DOS TIROS COM OS ALIENÍGENAS
def colisao_monstros():
	#IMPORTA AS VARIÁVEIS GLOBAIS
	global POS_MATRIZ_D, POS_MATRIZ_E, POS_MATRIZ_B
	global MONSTROS, PONTOS, F_JOGO, VIDAS
	global POS_ESPECIAL_X, POS_ESPECIAL_Y

	ids = []
	cont = 0;
	
	if(POS_MATRIZ_B > (80/100)*MENU_HEIGHT):
		F_JOGO[0] = 1

	for tiro in tiros:
		if(tiro.y <= POS_MATRIZ_B and tiro.x + tiro.width >= POS_MATRIZ_E and tiro.x <= POS_MATRIZ_D):
			for i in range(0,ALTURA-1):
				for j in range(0,LARGURA-1):
					if(MONSTROS[i][j] != 0):
						if(MONSTROS[i][j] == 1):
							especial = Sprite("sprites/especial.png")
							especial.set_position(POS_ESPECIAL_X[0], POS_ESPECIAL_Y[0])

							if(tiro.collided(especial)):
								MONSTROS[i][j] = 0
								if cont not in ids:	
									PONTOS [0] += 1 * DIFICULDADE[0]
									VIDAS [0] += 1
									ids.append(cont)

						else:
							if(tiro.collided(MONSTROS[i][j])):
								MONSTROS[i][j] = 0
								if cont not in ids:	
									PONTOS [0] += 1 * DIFICULDADE[0]				
									ids.append(cont)				
		
		if(tiro.y < 0 - tiro.height):
			if cont not in ids:
				ids.append(cont)

		cont += 1;

	for k in range(len(ids)-1,-1,-1):
		tiros.pop(ids[k])
