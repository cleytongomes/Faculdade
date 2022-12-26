from PPlay.window import *
from PPlay.gameimage import *
from PPlay.sound import *
from PPlay.sprite import *

from cons import *

class Pause:
	
	def __init__(self, janela, teclado, mouse):
		self.b_sair = Sprite(BUTTON_SAIR_W)
		self.b_sair.set_position((janela.width/2)-(self.b_sair.width/2),(janela.height/2)-(self.b_sair.height/2))
		self.tempo  = janela.time_elapsed()
		self.bloqueio_pause = True
		self.exit = 0
		self.loop_pause(janela, teclado, mouse)

	def loop_pause(self, janela, teclado, mouse):
		while 1:
			
			if(teclado.key_pressed("ESC")):
				if(self.bloqueio_pause == 0):				
					return 0
				self.bloqueio_pause = 1
			else:
				self.bloqueio_pause = 0

			if(mouse.is_button_pressed(1)):
				if(self.button_hover(self.b_sair, mouse)):
					self.exit = 1;
					return 0;

			if(self.button_hover(self.b_sair, mouse)):
				self.b_sair = Sprite(BUTTON_SAIR_R);
				self.b_sair.set_position((janela.width/2)-(self.b_sair.width/2),(janela.height/2)-(self.b_sair.height/2))
			else:
				self.b_sair = Sprite(BUTTON_SAIR_W);
				self.b_sair.set_position((janela.width/2)-(self.b_sair.width/2),(janela.height/2)-(self.b_sair.height/2))

			self.b_sair.draw()
			janela.update()

	def button_hover(self, botao, mouse):
		dx, dy = mouse.get_position()
		if(dx in range(int(botao.x), int(botao.x + botao.width) )):
			if(dy in range(int(botao.y), int(botao.y + botao.height) )):
				return 1;
		return 0;
