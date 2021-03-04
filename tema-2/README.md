[include]: include/
[src]: src/

[Data-h]: include/Data.h
[Data-c]: src/Data.c

[Tree-h]: include/Tree.h
[Tree-c]: src/Tree.c

[List-h]: include/List.h
[List-c]: src/List.c

[Server-h]: include/Server.h
[Server-c]: src/Server.c

[User-h]: include/User.h
[User-c]: src/User.c

[main-c]: src/main.c

[tree-construction-c]: src/tree-construction.c
[hierarchy-initialization-c]: src/hierarchy-initialization.c
[query-processing-c]: src/query-processing.c

# Ierarhie DNS
Acest subdirector conține **Tema 2** din cadrul cursului _Structuri de date_ ce presupune lucrul cu arbori. Programul este împărțit în mai multe componente/fișiere, fiecare din acestea rezolvând o anumită problemă.

## Structura codului sursă
În fișierele antet, aflate în [directorul include][include], se află declarațiile/definițiile de constante, structuri, funcții ce sunt folosite în cadrul fișierelor sursă. Unde este cazul, există și comentarii ce însoțesc declarațiile/definițiile menite să ușureze înțelegerea scopului acestora.

În fișierele sursă, aflate în [directorul src][src], se află implementările, fiind comentate părțile mai importante din logica programului.

## Încapsularea datelor ([Data.h][Data-h], [Data.c][Data-c])
Pentru a putea obține o implementare generică a tipurilor de date abstracte (în cazul de față liste înlănțuite și arbori), care poate stoca orice fel de date, se folosește o încapsulare într-o structură ce va conține atât un _pointer_ la zona de memorie unde se află de fapt datele propriu-zise, cât și dimensiunea datelor. Pentru accesarea datelor la nevoie se folosesc _cast_-uri de la `void *`.

## Implementarea arborelui ([Tree.h][Tree-h], [Tree.c][Tree-c])
Ca și în cazul **Temei 1**, s-a încercat implementarea arborelui într-un mod _Object Oriented_. Pentru reținerea copiilor unui nod din arbore se folosește o listă înlănțuită.

## Implementarea listei înlănțuite ([List.h][List-h], [List.c][List-c])
Implementarea de listă înlănțuită este cea folosită și în cadrul **Temei 1**.

## Reprezentarea serverelor în memorie ([Server.h][Server-h], [Server.c][Server-c])
Reprezentarea serverelor primite ca date de intrare se face prin intermediul unei structuri. Sunt implementate de asemenea și funcții de comparare și printare.

## Reprezentarea utilizatorilor în memorie ([User.h][User-h], [User.c][User-c])
Pentru reprezentarea utilizatorilor se folosește o structură. Sunt implementate de asemenea și funcții de comparare a doi utilizatori.

## Coordonarea sarcinilor ([main.c][main-c])
Reprezintă partea ce se ocupă de apelarea funcțiilor corespunzătoare efectuării sarcinilor de lucru.

## Rezolvarea sarcinilor de lucru
1. construcția arborelui ([tree-construction.c][tree-construction-c])
2. inițializarea ierarhiei ([hierarchy-initialization.c][hierarchy-initialization-c])
3. rezolvarea cererilor de la utilizatori, gestionarea defectelor ([query-processing.c][query-processing-c])
