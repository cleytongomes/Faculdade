<?php

	$port = fopen("/dev/ttyACM1", "w");
    exec("MODE $port BAUD=9600 PARITY=n DATA=8 XON=on STOP=1");
    fwrite ($port, "D");
    fclose($port);
?>
