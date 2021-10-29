<?php


// SPECIFY USB PORT TO USE
$usb_comPort = "COM4";



	switch($_POST)
	{
		case isset($_POST['submitOn']):
			exec("ECHO 1 > $usb_comPort"); // Turn On LED 1
			break;
		case isset($_POST['submitOff']):
			exec("ECHO 2 > $usb_comPort "); // Turn Off LED 1
			break;
		case isset($_POST['submitOn1']):
			exec("ECHO 3 > $usb_comPort"); // Turn On LED 2
			break;
		case isset($_POST['submitOff1']):
			exec("ECHO 4 > $usb_comPort"); // Turn Off LED 1
			break;
		case isset($_POST['submitOn2']):
			exec("ECHO 5 > $usb_comPort"); // Turn On LED 3
			break;
		case isset($_POST['submitOff2']):
			exec("ECHO 6 > $usb_comPort"); // Turn Off LED 1
			break;
		case isset($_POST['submitOn3']):
			exec("ECHO 7 > $usb_comPort"); // Turn On LED 4
			break;
		case isset($_POST['submitOff3']):
			exec("ECHO  8 > $usb_comPort"); // Turn Off LED 1
			break;
		case isset($_POST['allon']):
			exec("ECHO  1,3,5,7 > $usb_comPort"); // Turn ON ALL 4 LED Bulbs
			break;
		case isset($_POST['alloff']):
			exec("ECHO  2,4,6,8 > $usb_comPort"); // Turn OFF ALL 4 LED Bulbs
			break;

	}

?>