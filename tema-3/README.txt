Am incercat modularizarea temei cat mai mult cu putinta.
Astfel, toate structurile de date sunt generice si pot fi folosite oriunde este nevoie.

Algoritmii ce opereaza pe grafuri folosesc field-ul metadata al nodurilor grafurilor
pentru a-si stoca informatiile de care au nevoie.
Field-ul metadata este implementat sub o lista inlantuita.

Rezolvarea cerintelor:

Problema 1:
	Task 1: - s-a folosit parcurgerea BFS pentru determinarea numarului de componente conexe
	
	Task 2: - s-a folosit, de asemenea, o parcurgere BFS pana la gasirea unui drum de la
	          unul dintre actori la celalat
	
	Task 3: - s-a folosit algoritmul lui Tarjan pentru determinarea nodurilor critice
	
	Bonus:  - s-a folosit algoritmul Born Kerbosch cu reprezentarea multimilor ca arbori binari
	          de cautare pentru determinarea clicilor
	        - pentru a extrage clica maxima, toate clicle obtinute din algoritm se introduc
	          intr-un Maxheap si la final se extrage prima clica din Heap

Problema 2:
	- s-a folosit algoritmul lui Dijkstra la care se adauga in plus timpul de asteptare
	  intr-o camera daca este cazul, prin adaugarea timpului asteptat la distanta pana la
	  un anumit nod
