[include]: include/
[src]: src/

[Data-h]: include/Data.h
[Data-c]: src/Data.c

[List-h]: include/List.h
[List-c]: src/List.c

[Point-h]: include/Point.h
[Point-c]: src/Point.c

[Interval-h]: include/Interval.h
[Interval-c]: src/Interval.c

[main-c]: src/main.c

[outlier-removal-c]: src/outlier-removal.c
[noise-removal-median-c]: src/noise-removal-median.c
[noise-removal-a-mean-c]: src/noise-removal-a-mean.c
[time-levelling-c]: src/time-levelling.c
[data-filling-c]: src/data-filling.c
[statistics-c]: src/statistics.c

# Corecție, eliminarea excepțiilor și completarea datelor secvențiale
Acest subdirector conține **Tema 1** din cadrul cursului _Structuri de date_ ce presupune lucrul cu liste înlănțuite. Programul este împărțit în mai multe componente/fișiere, fiecare din acestea rezolvând o anumită problemă.

## Structura codului sursă
În fișierele antet, aflate în [directorul include][include], se află declarațiile/definițiile de constante, structuri, funcții ce sunt folosite în cadrul fișierelor sursă. Unde este cazul, există și comentarii ce însoțesc declarațiile/definițiile menite să ușureze înțelegerea scopului acestora.

În fișierele sursă, aflate în [directorul src][src], se află implementările, fiind comentate părțile mai importante din logica programului.

## Încapsularea datelor ([Data.h][Data-h], [Data.c][Data-c])
Pentru a putea obține o implementare de listă înlănțuită generică, care poate stoca orice fel de date, se folosește o încapsulare într-o structură ce va conține atât un _pointer_ la zona de memorie unde se află de fapt datele propriu-zise, cât și dimensiunea datelor. Pentru accesarea datelor la nevoie se folosesc _cast_-uri de la `void *`.

## Implementarea listei înlănțuite ([List.h][List-h], [List.c][List-c])
S-a încercat implementarea listei înlănțuite într-un mod _Object Oriented_, în [fișierul sursă][List-c] logica fiind implementată în funcții _private_, vizibile doar în cadrul unui singur _translation unit_. Funcțiile _publice_ aflate în a doua jumătate a fișierului sursă împachetează apeluri către funcțiile private ce în consecință efectuează operații pe lista/nodul pasat(ă) ca argument. În acest fel se obține un control mai fin asupra proprietăților listei înlănțuite la care avem acces în mod public, având în vedere că în [fișierul antet][List-h] există doar _forward declaration_-uri pentru structuri, în consecință neputând accesa în mod direct membri acestora, fiind nevoiți să folosim funcțiile puse la dispoziție tot în fișierul antet.

## Reprezentarea punctelor în memorie ([Point.h][Point-h], [Point.c][Point-c])
Reprezentarea punctelor primite ca date de intrare se face prin intermediul unei structuri. Sunt implementate de asemenea și funcții de comparare și printare a punctelor ce se găsesc în liste înlănțuite. Funcția de comparare se folosește în cadrul sortărilor listelor înlănțuite de puncte (necesare pentru sarcina de filtrare). Funcția de printare se folosește în cadrul afișării finale a listei de puncte procesate.

## Reprezentarea intervalelor în memorie ([Interval.h][Interval-h], [Interval.c][Interval-c])
Pentru reprezentarea intervalelor necesare generării statisticilor din cadrul sarcinii bonus se folosește o structură. Sunt implementate de asemenea și funcții de comparare și printare a intervalelor ce se găsesc în liste înlănțuite. Funcția de comparare se folosește în cadrul sortărilor listelor înlănțuite de intervale (necesare pentru sarcina bonus). Funcția de printare se folosește în cadrul rezolvării sarcinii bonus pentru afișarea finală a intervalelor sortate.

## Coordonarea sarcinilor ([main.c][main-c])
Reprezintă partea ce se ocupă de evaluarea argumentelor transmise prin linia de comandă, citirea datelor de intrare, apelarea funcțiilor corespunzătoare procesării acestora și afișarea datelor de ieșire după ce procesările au luat sfârșit.

## Rezolvarea sarcinilor de lucru
1. eliminarea de excepții folosind statistici ([outlier-removal.c][outlier-removal-c])
2. eliminarea de zgomot prin filtru median ([noise-removal-median.c][noise-removal-median-c])
3. eliminarea de zgomot prin flitru bazat pe media aritmetică ([noise-removal-a-mean.c][noise-removal-a-mean-c])
4. uniformizarea frecvenței ([time-levelling.c][time-levelling-c])
5. completarea datelor lipsă ([data-filling.c][data-filling-c])
6. calculul statisticilor ([statistics.c][statistics-c])
