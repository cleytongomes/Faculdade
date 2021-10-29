<?php

	// Conecta na porta
	$port = fopen("/dev/ttyACM2", "w+");
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
	$umiAr = fgets($port);
	$regador1 = fgets($port);
	$cooler = fgets($port);
	$nivel = fgets($port);
	$estado = fgets($port);
	$lux = fgets($sport);
	$my = array('iluminacao'=>$iluminacao,'temperatura'=>$temperatura,'umi1'=>$umi1, 'umiAr'=>$umiAr,
					'regador1'=>$regador1, 'cooler'=>$cooler, 'nivel'=>$nivel, 
					'estado'=>$estado, 'lux'=>$lux);
	$myJSON = json_encode($my);
	echo($myJSON);
	// Fecha a conexão com a porta
	fclose($port);
?>
