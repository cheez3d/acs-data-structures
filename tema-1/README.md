[include]: include/
[src]: src/

[main-c]: src/main.c

[outlier-removal-c]: src/outlier-removal.c
[noise-removal-median-c]: src/noise-removal-median.c
[noise-removal-a-mean-c]: src/noise-removal-a-mean.c
[time-levelling-c]: src/time-levelling.c
[data-filling-c]: src/data-filling.c
[statistics-c]: src/statistics.c

[List-h]: include/List.h
[List-c]: src/List.c

[Data-h]: include/Data.h
[Data-c]: src/Data.c

[Point-h]: include/Point.h
[Point-c]: src/Point.c

[Interval-h]: include/Interval.h
[Interval-c]: src/Interval.c

[helpers-h]: include/helpers.h
[helpers-c]: src/helpers.c

# Corecție, eliminarea excepțiilor și completarea datelor secvențiale
Tema 1 din cadrul cursului ***Structuri de date*** ce presupune lucrul cu liste înlănțuite.
Programul este împărțit în mai multe componente/fișiere, fiecare din acestea rezolvând o anumită problemă.

## Structura codului sursă
În fișierele antet, aflate în [directorul include][include], se află declarațiile/definițiile de constante, structuri, funcții ce sunt folosite predilect în cadrul fișierelor sursă. Unde este cazul, există și comentarii ce însoțesc declarațiile/definițiile menite să ușureze înțelegerea scopului acestora.
În fișierele sursă, aflate în [directorul src][src], se află implementările, fiind comentate părțile mai importante din logica programului.

## Încapsularea datelor ([Data.h][Data-h], [Data.c][Data-c])
Pentru a putea obține o implementare de listă înlănțuită generică, care poate stoca orice fel de date, se folosește o încapsulare într-o structură ce va conține atât un *pointer* la zona de memorie unde se află de fapt datele propriu-zise, cât și dimensiunea datelor. Pentru accesarea datelor la nevoie se folosesc *type cast*-uri de la `void *`.

## Implementarea listei înlănțuite ([List.h][List-h], [List.c][List-c])
S-a încercat implementarea listei înlănțuite într-un stil *Object Oriented*, în [fișierul sursă][List-c] logica fiind implementată în funcții „*private*”, vizibile doar în cadrul unui singur *translation unit*. Funcțiile „*publice*” aflate în a doua jumătate a fișierului sursă împachetează apeluri către funcțiile private ce în consecință efectuează operații pe lista/nodul pasat(ă) ca argument. În acest fel se obține un control mai fin asupra proprietăților listei înlănțuite la care avem acces în mod public, având în vedere că în [fișierul antet][List-h] există doar *forward declaration*-uri pentru structuri, în consecință neputând accesa în mod direct membri acestora, fiind nevoiți să folosim funcțiile puse la dispoziție tot în fișierul antet.
Având în vedere genericitatea implementării, fișierul antet conține și `typedef`-uri pentru funcții de comparație și printare a nodurilor ce sunt implementate mai apoi pentru orice tip de date este nevoie.

## Reprezentarea punctelor în memorie ([Point.h][Point-h], [Point.c][Point-c])
Reprezentarea punctelor primite ca date de intrare se face prin intermediul unei structuri.
Sunt implementate de asemenea și funcții de comparare și printare a punctelor ce se găsesc în liste înlănțuite.
Funcția de comparare se folosește în cadrul sortărilor listelor înlănțuite de puncte (necesare pentru sarcina de filtrare).
Funcția de printare se folosește în cadrul afișării finale a listei de puncte procesate.

## Reprezentarea intervalelor în memorie ([Interval.h][Interval-h], [Interval.c][Interval-c])
Pentru reprezentarea intervalelor necesare generării statisticilor din cadrul sarcinii bonus se folosește o structură.
Sunt implementate de asemenea și funcții de comparare și printare a intervalelor ce se găsesc în liste înlănțuite.
Funcția de comparare se folosește în cadrul sortărilor listelor înlănțuite de intervale (necesare pentru sarcina bonus).
Funcția de printare se folosește în cadrul rezolvării sarcinii bonus pentru afișarea finală a intervalelor sortate.

## Coordonarea sarcinilor ([main.c][main-c])
Partea principală ce se ocupă de evaluarea argumentelor transmise prin linia de comandă, citirea datelor de intrare, apelarea funcțiilor corespunzătoare procesării acestora și afișarea datelor de ieșire după ce procesările au luat sfârșit.

## Funcțiile ajutătoare ([helpers.h][helpers-h], [helpers.c][helpers-c])
Acest program nu se folosește de funcții ajutătoare implementate separat în fișierul sursă [helpers.c][helpers-c].

## Rezolvarea sarcinilor de lucru
1. eliminarea de excepții folosind statistici ([outlier-removal.c][outlier-removal-c])
2. eliminarea de zgomot prin filtru median ([noise-removal-median.c][noise-removal-median-c])
3. eliminarea de zgomot prin flitru bazat pe media aritmetică ([noise-removal-a-mean.c][noise-removal-a-mean-c])
4. uniformizarea frecvenței ([time-levelling.c][time-levelling-c])
5. completarea datelor lipsă ([data-filling.c][data-filling-c])
6. calculul statisticilor ([statistics.c][statistics-c])
