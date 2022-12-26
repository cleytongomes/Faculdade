def hover(mouse, buttons):
	dx, dy = mouse.get_position()

	bt = 1	
	for k in buttons:
		if(dy in range(int(k.y),  int (k.y + k.height))):
			if(dx in range(int(k.x), int (k.x + k.width))):
				return bt
				pass
		bt += 1 

def click(mouse, buttons):
	dx, dy = mouse.get_position()
	
	bt = 1
	for k in buttons:
		if(dy in range(int(k.y),  int (k.y + k.height))):
			if(dx in range(int(k.x), int (k.x + k.width))):
				return bt
				pass
		bt += 1 
