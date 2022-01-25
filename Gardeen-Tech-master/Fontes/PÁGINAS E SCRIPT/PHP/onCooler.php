<?php
	$port = fopen("/dev/ttyACM0", "w");
    exec("MODE $port BAUD=9600 PARITY=n DATA=8 XON=on STOP=1");
    fwrite ($port, "B");
    fclose($port);
?>

