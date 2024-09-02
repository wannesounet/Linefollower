# Gebruiksaanwijzing

### opladen / vervangen batterijen
Wanneer de batterijen plat zijn, moeten deze worden vervangen, dit zijn 6 AA batterijen.

### draadloze communicatie
#### verbinding maken
Na het uploaden van de code en terug verbinden van de rx en tx pin. Kunnen we gaan verbinden, neem de arduino bluetooth controller app. En pair met het juiste device (pin: 0000 of 1234). Na het pairen klikt u op het juiste apparaat en selecteer je terminal.

#### commando's
c: met c gaat de robot in calibratie modus. Na 10 seconden is dit over, dan krijg je de melding of de calibratie goed gelukt is (goed meet bereik)
Anders moet met nog maals gaan calibreren.
1: start de auto
0: stopt de auto

### kalibratie
De kalibratie is nodig zodat de sensor weet welke de uiterste waarden zijn min en max, en past aan de hand daar van de gemeten waarden aan. DIt wordt gedaan door middel van een programma die 10 seconden draait en die alle waarden op meet en daar van de min en de max opslaat. De bedoeling is dat je tijdens dit programma de meest donkere en lichte vlakken toont onder de sensoren, zo dat hij zijn uitersten kan opslaan.

### settings
De robot rijdt stabiel met volgende parameters:  
Speed:38
Kp = 0.33;
Ki = 0;
Kd = 0.6;

### start/stop button
Er is geen start/stop button aanwezig op de auto omdat dit niet gemakklijk is om te bedienen tijdens het beegen vna de robot. Daarom koos ik ervoor om te sturen via bluetooth.
