# Linefollower Maze Solver
<b>Autor:</b> Niesuch <br />
<b>Jezyk programowania:</b> C <br />

## O projekcie:
<p align="justify">
Projekt pojazdu typu linefollower, czyli pojazdu poruszającego się po wyznaczonej trasie. Został on wyposażony w algorytm rozwiązywania problemu wyjścia z labiryntu, tj. algorytm lewej ręki. Po uruchomieniu pojazdu poszukiwana jest droga do wyjścia, a w międzyczasie skracana o niepotrzebne odcinki ścieżka. Gdy linefollower osiągnie swój cel, a następnie przemieścimy go z powrotem na linię startu, przejedzie on najkrótszą znalezioną trasą do mety. Zasada algorytmu lewej ręki jest trywialna. Do momentu wystąpienia skrzyżowania pojazd jedzie przed siebie. W przeciwnym wypadku wybiera skręt w lewo (o ile istnieje). Kolejnym priorytetem jest wybór drogi prosto, a jeżeli ta nie występuje wykonuje skręt w prawo lub zawraca w przypadku dotarcia do ślepego zaułka.

<b>Do budowy pojazdu wykorzystano:</b>
1) Arduino UNO
2) Ardumoto Shield Kit
3) Pololu QTR-8RC
4) 2x silniki prądu stałego
5) Szkielet pojazdu
6) Ball caster 3/4"
7) 2x koła z oponami
8) Koszyk na 4 baterie AA
9) 4x baterie AA
10) Przewody połączeniowe

<b>Schemat połączeń podzespołów pojazdu:</b>
* Ardumoto PIN 2 cyfrowy ↔ Pololu QTR PIN 1
* Ardumoto PIN 4 cyfrowy ↔ Pololu QTR PIN 2
* Ardumoto PIN 5 cyfrowy ↔ Pololu QTR PIN 3
* Ardumoto PIN 6 cyfrowy ↔ Pololu QTR PIN 4
* Ardumoto PIN 7 cyfrowy ↔ Pololu QTR PIN 5
* Ardumoto PIN 8 cyfrowy ↔ Pololu QTR PIN 6
* Ardumoto PIN 9 cyfrowy ↔ Pololu QTR PIN 7
* Ardumoto PIN 10 cyfrowy ↔ Pololu QTR PIN 8
* Ardumoto PIN GND ↔ Pololu QTR PIN GND
* Ardumoto PIN 5V ↔ Pololu QTR PIN VCC
* Ardumoto przyłacze A 1 ↔ Czarny kabel lewego silnika
* Ardumoto przyłącze A 2 ↔ Czerwony kabel lewego silnika
* Ardumoto przyłącze B 3 ↔ Czarny kabel prawego silnika
* Ardumoto przyłącze B 4 ↔ Czerwony kabel prawego silnika
* Ardumoto przyłącze VIN (+) ↔ Czerwony kabel koszyczka z bateriami
* Ardumoto przyłącze VIN (-) ↔ Czarny kabel koszyczka z bateriami
* Ardumoto ↔ Arduino UNO
</p>

## Zdjęcia:
![Alt text](/images/IMG_1111.JPG)
![Alt text](/images/IMG_1112.JPG)
![Alt text](/images/IMG_1113.JPG)

## Wideo:
https://www.youtube.com/watch?v=OyrQlJrjheg

## Prawa autorskie i licencja
Copyright 2017 Niesuch, Inc. Kod wydany na licencji [MIT](https://github.com/niesuch/linefollower-maze-solver/blob/master/LICENSE.md).
