<?php
  $my = array(
              ['id'=>"1",'nome'=>"Solanum lycopersicum (Tomate)",'temperatura'=>"20-26°C",'germinacao'=>"-",'iluminacao'=>"Alta",'ph'=>"5,5-7",'irrigacao'=>"Úmido"],
              ['id'=>"2",'nome'=>"Lactuca sativa (Alface)",'temperatura'=>"10-24°C",'germinacao'=>"-",'iluminacao'=>"Média-Alta",'ph'=>"6-7",'irrigacao'=>"Úmido"],
              ['id'=>"3",'nome'=>"Allium schoenoprasum (Cebolinha)",'temperatura'=>"13-24°C",'germinacao'=>"-",'iluminacao'=>"Média",'ph'=>">5.5",'irrigacao'=>"Úmido"],
              ['id'=>"4",'nome'=>"Petroselinum crispum (Salsa)",'temperatura'=>"10-22°C",'germinacao'=>"2-6 semanas",'iluminacao'=>"Média-Alta",'ph'=>"5.8-7.2",'irrigacao'=>"Úmido"],
              ['id'=>"5",'nome'=>"Phaseolus vulgaris (Feijão)",'temperatura'=>"18-25°C",'germinacao'=>"2 semanas",'iluminacao'=>"Alta",'ph'=>"5.5-6.5",'irrigacao'=>"Úmido"],
              ['id'=>"6",'nome'=>"Fragaria chiloensis (Morango)",'temperatura'=>"04-20°C",'germinacao'=>"-",'iluminacao'=>"Média-Alta",'ph'=>"5.5-6.5",'irrigacao'=>"Úmido"],
              ['id'=>"7",'nome'=>"Solanum tuberosum (Batata)",'temperatura'=>"15-20°C",'germinacao'=>"-",'iluminacao'=>"Alta",'ph'=>"5.0-6.0",'irrigacao'=>"Levemente úmido"],
              ['id'=>"8",'nome'=>"Daucus carota sativus (Cenoura)",'temperatura'=>"16-22°C",'germinacao'=>"1-3 semanas",'iluminacao'=>"Alta",'ph'=>"6.0-7.5",'irrigacao'=>"Levemente úmido"],
              ['id'=>"9",'nome'=>"Capsicum frutescens (Pimenta Malagueta)",'temperatura'=>"16-34°C",'germinacao'=>"1-2 semanas",'iluminacao'=>"Alta",'ph'=>"5.0-8.0",'irrigacao'=>"Úmido"],
              ['id'=>"10",'nome'=>"Petroselinum crispum (Salsa)",'temperatura'=>"10-22°C",'germinacao'=>"2-6 semanas",'iluminacao'=>"Média",'ph'=>"5,8-7,2",'irrigacao'=>"Úmido"],
              ['id'=>"11",'nome'=>"Rosmarinus officinalis (Alecrim)",'temperatura'=>"-",'germinacao'=>"-",'iluminacao'=>"Média",'ph'=>"7-7,8",'irrigacao'=>"Levemente úmido"],
              ['id'=>"12",'nome'=>"Coriandrum sativum (Coentro)",'temperatura'=>"-",'germinacao'=>"-",'iluminacao'=>"Alta",'ph'=>"-",'irrigacao'=>"Levemente úmido"],
             );
  $myJSON = json_encode($my);
	echo($myJSON);
?>
