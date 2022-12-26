<?php

	// Conecta na porta
	$port = fopen("/dev/ttyACM0", "w+");
	exec("MODE $port BAUD=9600 PARITY=n DATA=8 XON=on STOP=1");

	// Em alguns casos a Arduino pode reiniciar, por isso
	// é bom esperar para enviar informação depois de conectar
	//sleep(2);

	// Envia "1" para o programa na Arduino saber que deve responder
	fwrite($port, 'R');

	// Espera para que o dado enviado pelo PHP chegue até a Arduino
	sleep(1);

	// Agora que a Arduino "Provavelmente já respondeu", pega
	// o valor da resposta
	$iluminacao = fgets($port);
	$temperatura = fgets($port);
	$umi1 = fgets($port);
	$umi2 = fgets($port);
	$regador1 = fgets($port);
	$regador2 = fgets($port);
	$cooler = fgets($port);
	$nivel = fgets($port);
	$estado = fgets($port);

	$my = array('iluminacao'=>$iluminacao,'temperatura'=>$temperatura,'umi1'=>$umi1, 'umi2'=>$umi2,
					'regador1'=>$regador1, 'regador2'=>$regador2, 'cooler'=>$cooler, 'nivel'=>$nivel, 
					'estado'=>$estado);
	$myJSON = json_encode($my);
	echo($myJSON);
	// Fecha a conexão com a porta
	fclose($port);
?>
