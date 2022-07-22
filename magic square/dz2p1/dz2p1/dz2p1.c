#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int arrayNum = 0, n, busyNodes = 0, magicSum = 0, counter = 0;

typedef struct Node {
	int **square;
	struct Node **descs;
	int numDescs;
	int numUsed;
} node;

typedef struct {
	int numNodes;
	node *root;
} tree;

typedef struct Elem {
	node *info;
	int level;
	struct Elem *next;
} elem;

typedef struct {
	elem *front, *tail;
	int numInfos;
} header;

typedef struct {
	elem *top;
	int numInfos;
} stek;

// ucitavanje dimenzija kvadrata
int loadDims() {
	int tN;
	printf("Unesite dimenzije: ");
	scanf("%d", &tN);
	return tN;
}

// ucitavanje kvadrata
int **loadSquare(int tN, int **tA) {
	tA = (int **)malloc(tN * sizeof(int *));
	if (tA == NULL) {
		perror("Error: ");
		exit(1);
	}
	for (int i = 0; i < tN; i++) {
		*(tA + i) = (int *)calloc(tN, sizeof(int));
		if (*(tA + i) == NULL) {
			perror("Error: ");
			exit(1);
		}
	}
	return tA;
}

// oslobadjanje kvadrata
int **freeSquare(int tN, int **tA) {
	if (tA) {
		for (int i = 0; i < tN; i++)
			free(*(tA + i));
		free(tA);
	}
	return NULL;
}

// inicijalizacija kvadrata
// ukoliko korisnik unese vec unet element, samo ce mu ponovo promeniti vrednost
// popunjavanje skupa vrednosti inicijalizovanim vrednostima
void initalizeSquare(int tN, int **tA, int *tArray) {
	int i, j, val, iter;
	printf("Unesite inicijalno stanje kvadrata - vrsta kolona vrednost\n");
	printf("Da biste zavrsili, unesite nekorektan broj za vrstu, kolonu ili vrednost\n");
	while (1) {
		scanf("%d%d%d", &i, &j, &val);
		if (val <= 0 || i < 0 || i >= tN || j < 0 || j >= tN) return;
		if (!*(*(tA + i) + j)) {
			busyNodes++;
			*(tArray + arrayNum++) = val;
		}
		else {
			for (iter = 0; iter < arrayNum; iter++)
				if (*(tArray + iter) == *(*(tA + i) + j))
					*(tArray + iter) = val;
		}
		*(*(tA + i) + j) = val;
	}
}

// provera savrsenog kvadrata
int isItPerfectSquare(int **tNode, int tMagicSum) {
	int k, i, j, s;
	for (k = 1; k < n; k++) {
		s = 0;
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (i + k == j || i == j + n - k)
					s += *(*(tNode + i) + j);
		if (s != tMagicSum) return 0;
	}
	for (k = 1; k < n; k++) {
		s = 0;
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (i + j == n - 1 + k || i + j == k - 1)
					s += *(*(tNode + i) + j);
		if (s != tMagicSum) return 0;
	}
	return 1;
}

// ispisvanje jednog kvadrata
void printOneSquare(int tN, int **tA) {
	for (int i = 0; i < tN; i++)
		for (int j = 0; j < tN; j++)
				printf("%2d%c", *(*(tA + i) + j), j == tN - 1 ? '\n' : ' ');
	if (isItPerfectSquare(tA, magicSum)) printf("Jeste savrsen\n");
	else printf("Nije savrsen\n");
	putchar('\n');
}

// ucitavanje niza
int *loadArray(int tN, int *tArray) {
	tArray = (int *)malloc(tN * sizeof(int));
	if (tArray == NULL) {
		perror("Error: ");
		exit(1);
	}
	return tArray;
}

// oslobadjanje niza
int *freeArray(int *tArray) {
	if (tArray) free(tArray);
	return NULL;
}

// ucitavanje i inicijalizacija skupa vrednosti
void initalizeArray(int tN, int *tArray, int *tUser) {
	int i, j, t;
	int num = tN * tN - arrayNum;
	printf("Unesite preostalih %d vrednosti: ", num);
	for (int i = 0; i < num; i++) {
		scanf("%d", tUser + i);
		*(tArray + arrayNum++) = *(tUser + i);
	}
	for (i = 0; i < num - 1; i++)
		for (j = i + 1; j < num; j++)
			if (*(tUser + j) > *(tUser + i)) {
				t = *(tUser + j);
				*(tUser + j) = *(tUser + i);
				*(tUser + i) = t;
			}
}

// provera validnosti unetih vrednosti (da li se moze dobiti magicni kvadrat)
int checkValidity(int *tArray, int tN) {
	// sortiranje
	for (int *p = tArray; p < tArray + tN * tN - 1; p++)
		for (int *q = p + 1; q < tArray + tN * tN; q++)
			if (*q < *p) {
				int t = *q;
				*q = *p;
				*p = t;
			}
	// provera validnosti
	for (int *p = tArray; p < tArray + tN * tN - 2; p++)
		if (*(p + 2) - *(p + 1) != *(p + 1) - *p) return 0;
	return 1;
}

// odredjivanje magicne sume
int getMagicSum(int *tArray, int tN) {
	int tSum = 0;
	for (int *p = tArray; p < tArray + tN * tN; p++)
		tSum += *p;
	return tSum /= tN;
}

// ucitavanje reda
header *loadQueue(header *tHeader) {
	tHeader = (header *)malloc(sizeof(header));
	if (tHeader == NULL) {
		perror("Error: ");
		exit(1);
	}
	tHeader->front = NULL;
	tHeader->tail = NULL;
	tHeader->numInfos = 0;
	return tHeader;
}

// brisanje elementa iz reda
node *DELETE(header *tHeader, int *tLevel) {
	elem *tElem = tHeader->front;
	node *tNode = tElem->info;
	*tLevel = tElem->level;
	if (tHeader->numInfos == 1) {
		tHeader->front = NULL;
		tHeader->tail = NULL;
	}
	else
		tHeader->front = tHeader->front->next;
	tHeader->numInfos--;
	free(tElem);
	return tNode;
}

// oslobadjanje reda
header *freeQueue(header *tHeader) {
	if (tHeader) {
		int tLevel;
		node *tempNode = NULL;
		if (tHeader->front) tempNode = DELETE(tHeader, &tLevel);
		free(tHeader);
	}
	return NULL;
}

// dodavanje elemenata u red
void INSERT(header *tHeader, node* tNode, int tLevel) {
	elem *newElem = (elem *)malloc(sizeof(elem));
	newElem->info = tNode;
	newElem->level = tLevel;
	newElem->next = NULL;
	if (tHeader->tail != NULL) tHeader->tail->next = newElem;
	tHeader->tail = newElem;
	if (tHeader->front == NULL) tHeader->front = newElem;
	tHeader->numInfos++;
}

// ucitavanje steka
stek *loadStack(stek *tStek) {
	tStek = (stek *)malloc(sizeof(stek));
	if (tStek == NULL) {
		perror("Error: ");
		exit(1);
	}
	tStek->top = NULL;
	tStek->numInfos = 0;
	return tStek;
}

// brisanje iz steka
node *POP(stek *tStek, int *tLevel) {
	elem *tElem = tStek->top;
	node *tNode = tElem->info;
	*tLevel = tElem->level;
	tStek->top = tStek->top->next;
	tStek->numInfos--;
	free(tElem);
	return tNode;
}

// dodavanje u stek
void PUSH(stek *tStek, node *tNode, int tLevel) {
	elem *newElem = (elem *)malloc(sizeof(elem));
	newElem->info = tNode;
	newElem->level = tLevel;
	newElem->next = tStek->top;
	tStek->top = newElem;
	tStek->numInfos++;
}

// oslobadjanje steka
stek *freeStack(stek *tStek) {
	if (tStek) {
		int tLevel;
		node *tempNode = NULL;
		if (tStek->top) tempNode = POP(tStek, &tLevel);
		free(tStek);
	}
	return NULL;
}

// provera mogucnosti magicnog kvadrata
int canItBeMagicSquare(node *tCvor, int tSum) {
	int currSum, i, j, ind;
	// provera po vrstama
	for (i = 0; i < n; i++) {
		currSum = 0;
		ind = 0;
		for (j = 0; j < n; j++) {
			if (!*(*(tCvor->square + i) + j)) ind = 1;
			else currSum += *(*(tCvor->square + i) + j);
		}
		if (!ind && (currSum != tSum)) return 0;
		else if (ind && (currSum >= tSum)) return 0;
	}
	// provera po kolonama
	ind = 0;
	for (j = 0; j < n; j++) {
		currSum = 0;
		ind = 0;
		for (i = 0; i < n; i++) {
			if (!*(*(tCvor->square + i) + j)) ind = 1;
			else currSum += *(*(tCvor->square + i) + j);
		}
		if (!ind && (currSum != tSum)) return 0;
		else if (ind && (currSum >= tSum)) return 0;
	}
	// provera za glavnu dijagonalu
	ind = 0;
	currSum = 0;
	for (i = 0; i < n; i++) {
		if (!*(*(tCvor->square + i) + i)) ind = 1;
		else currSum += *(*(tCvor->square + i) + i);
	}
	if (!ind && (currSum != tSum)) return 0;
	else if (ind && (currSum >= tSum)) return 0;
	// provera za sporednu dijagonalu
	ind = 0;
	currSum = 0;
	for (i = n - 1, j = 0; j < n; i--, j++) {
		if (!*(*(tCvor->square + i) + j)) ind = 1;
		else currSum += *(*(tCvor->square + i) + j);
	}
	if (!ind && (currSum != tSum)) return 0;
	else if (ind && (currSum >= tSum)) return 0;
	return 1;
}

// kopiranje kvadrata iz oca u sina
int **copySquare(int **tA) {
	int **tB = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++) {
		*(tB + i) = (int *)malloc(n * sizeof(int));
		memcpy(*(tB + i), *(tA + i), n * sizeof(int));
	}
	return tB;
}

// kreiranje sinova
node *createDescendants(node *tCvor, int tValue, header *tQueue, int tLevel) {
	int i = 0, j = 0, k;
	int freeNodes = n * n - tCvor->numUsed;
	if (freeNodes) {
		tCvor->descs = (node **)malloc(freeNodes * sizeof(node *));
		if (tCvor->descs == NULL) {
			perror("Error1: ");
			exit(1);
		}
		for (k = 0; k < freeNodes; k++) {
			tCvor->descs[k] = (node *)malloc(sizeof(node));
			tCvor->descs[k]->square = loadSquare(n, tCvor->descs[k]->square);
			tCvor->descs[k]->square = copySquare(tCvor->square);
			while (*(*(tCvor->square + i) + j)) {
				j++;
				if (j == n) {
					i++;
					j = 0;
				}
			}
			*(*(tCvor->descs[k]->square + i) + j) = tValue;
			j++;
			if (j == n) {
				i++;
				j = 0;
			}
			if (!canItBeMagicSquare(tCvor->descs[k], magicSum)) {
				tCvor->descs[k]->square = freeSquare(n, tCvor->descs[k]->square);
				free(tCvor->descs[k]);
				k--;
				freeNodes--;
				if (!freeNodes) {
					free(tCvor->descs);
					tCvor->descs = NULL;
				}
				else {
					tCvor->descs = (node **)realloc(tCvor->descs, freeNodes * sizeof(node *));
					if (freeNodes && tCvor->descs == NULL) {
						perror("Error2: ");
						exit(1);
					}
				}
			}
			else {
				tCvor->descs[k]->numUsed = tCvor->numUsed + 1;
				tCvor->descs[k]->numDescs = 0;
				tCvor->descs[k]->descs = NULL;
				INSERT(tQueue, tCvor->descs[k], tLevel + 1);
				tCvor->numDescs++;
				counter++;
			}
		}
	}
	return tCvor;
}

// kreiranje stabla
void formTree(tree *tRoot, header *tQueue, int *tUser) {
	int lev;
	while (tQueue->front) {
		node *current = DELETE(tQueue, &lev);
		// printOneSquare(n, current->square);
		if (canItBeMagicSquare(current, magicSum)) {
			current = createDescendants(current, tUser[lev], tQueue, lev);
		}
	}
}

// post-order obilazak generalisanog stabla
void postOrder(tree *tRoot, stek *tStek) {
	int currLevel, numDes;
	node *tNode = tRoot->root;
	while (tNode->descs) {
		PUSH(tStek, tNode, 0);
		tNode = tNode->descs[0];
	}
	if (isItMagicSquare(tNode)) printOneSquare(n, tNode->square);
	else if (tNode == tRoot->root) putchar('\n');
	while (tStek->numInfos) {
		tNode = POP(tStek, &currLevel);
		currLevel++;
		if (currLevel < tNode->numDescs) {
			PUSH(tStek, tNode, currLevel);
			tNode = tNode->descs[currLevel];
			while (tNode->descs) {
				PUSH(tStek, tNode, 0);
				tNode = tNode->descs[0];
			}
			if (isItMagicSquare(tNode)) printOneSquare(n, tNode->square);
		}
		else {
			if (isItMagicSquare(tNode)) printOneSquare(n, tNode->square);
			else if (tNode == tRoot->root) putchar('\n');
		}
	}
}

// ispis sa identacijom
void identationPrint(node *tNode, int level) {
	for (int i = 0; i < n; i++) {
		for (int k = 0; k < 3 * level * n; k++)
			printf(" ");
		for (int j = 0; j < n; j++)
			printf("%2d%c", *(*(tNode->square + i) + j), j == n - 1 ? '\n' : ' ');
	}
	// if (isItPerfectSquare(tA, magicSum)) printf("Jeste savrsen\n");
	// else printf("Nije savrsen\n");
	putchar('\n');
}

// formatirani ispis stabla po nivoima
void preOrder(tree *tRoot, stek *tStek) {
	int tLevel;
	node *tNode = tRoot->root;
	PUSH(tStek, tNode, 0);
	while (tStek->numInfos) {
		tNode = POP(tStek, &tLevel);
		while (tNode->descs) {
			identationPrint(tNode, tLevel);
			for (int i = tNode->numDescs-1; i >= 1; i--)
				PUSH(tStek, tNode->descs[i], tLevel + 1);
			tNode = tNode->descs[0];
			tLevel++;
		}
		identationPrint(tNode, tLevel);
	}
}

// provera magicnog kvadrata
int isItMagicSquare(node *tNode) {
	if (tNode->numUsed != n * n) return 0;
	else return 1;
}

// oslobadjanje stabla
tree *freeTree(tree *tStablo) {
	if (tStablo) {
		int level = 0;
		stek *tStek = NULL; tStek = loadStack(tStek);
		header *tQueue = NULL; tQueue = loadQueue(tQueue);
		node *tNode = tStablo->root;
		INSERT(tQueue, tNode, level);
		while (tQueue->numInfos) {
			tNode = DELETE(tQueue, &level);
			PUSH(tStek, tNode, level);
			for (int i = 0; i < tNode->numDescs; i++)
				INSERT(tQueue, tNode->descs[i], level + 1);
		}
		while (tStek->numInfos != 1) {
			tNode = POP(tStek, &level);
			tNode->square = freeSquare(n, tNode->square);
			if (tNode->descs) free(tNode->descs);
			free(tNode);
		}
		free(tStablo);
		tQueue = freeQueue(tQueue);
		tStek = freeStack(tStek);
	}
	return NULL;
}

// ispisivanje menija
void menuPrint() {
	printf("Izaberite jednu od sledecih opcija ---->\n");
	printf("1 - Inicijalizuj kvadrat i unesi skup vrednosti\n");
	printf("2 - Ispisi magicnu sumu\n");
	printf("3 - Formiraj stablo\n");
	printf("4 - Ispisi stablo u level-orderu\n");
	printf("5 - Ispisi magicne kvadrate u post-orderu\n");
	printf("6 - Ponovi program\n");
	printf("7 - Zavrsi program\n");
}

// glavna fja
int main() {
	int **square = NULL, *arr = NULL, *userArr = NULL, inp;
	tree *stablo = NULL; node *cvor = NULL;
	header *heder = NULL;
	stek *tStek = NULL, *stekZaPre = NULL;
	while (1) {
		menuPrint();
		printf("Vas izbor: ");
		scanf("%d", &inp);
		switch (inp) {
			case 1:
				if (!magicSum) {
					n1:
					n = loadDims();
					if (n <= 0) {
						printf("Nevalidna dimenzija, pokusajte ponovo.\n");
						goto n1;
					}
					square = loadSquare(n, square);
					arr = loadArray(n * n, arr);
					initalizeSquare(n, square, arr);
					userArr = loadArray(n * n - arrayNum, userArr);
					initalizeArray(n, arr, userArr);
					// ovo dodajem radi provere
					// printOneSquare(n, square);
					if (checkValidity(arr, n)) {
						printf("Unesene vrednosti su validne i mozda mozemo kreirati magicne kvadrate.\n\n");
						magicSum = getMagicSum(arr, n);
					}
					else {
						printf("Od unesenih vrednosti ne moze da se kreira niti jedan magican kvadrat.\n");
						magicSum = 0;
					fLab:
						printf("1 - Ponovi program\n");
						printf("2 - Zavrsi program\n");
						printf("Vas izbor: ");
						scanf("%d", &inp);
						switch (inp) {
							case 1:
								goto lab6;
								break;
							case 2:
								goto lab7;
								break;
							default:
								printf("Los izbor, pokusajte ponovo.\n");
								goto fLab;
								break;
						}
					}
				}
				else {
					printf("Ne mozete inicijalizovati vec inicijalizovan kvadrat.\n");
					printf("Takodje, ne mozete ponovo uneti skup vrednosti.\n\n");
				}
				break;
			case 2:
				if (magicSum) printf("Vrednost magicne sume je: %d\n\n", magicSum);
				else printf("Najpre morate inicijalizovati kvadrat i unesti skup vrednosti.\n\n");
				break;
			case 3:
				if (magicSum) {
					if (stablo) {
						printf("Vec ste napravili stablo.\n\n");
					}
					else {
						stablo = (tree *)malloc(sizeof(tree));
						cvor = (node *)malloc(sizeof(node));
						cvor->square = square;
						cvor->numUsed = busyNodes;
						cvor->numDescs = 0;
						cvor->descs = NULL;
						stablo->root = cvor;
						stablo->numNodes = 1;
						heder = loadQueue(heder);
						INSERT(heder, cvor, 0);
						counter++;
						formTree(stablo, heder, userArr);
						printf("U vasem stablu ima %d cvorova\n", counter);
						putchar('\n');
					}
				}
				else printf("Najpre morate inicijalizovati kvadrat i unesti skup vrednosti.\n\n");
				break;
			case 4: 
				if (magicSum) {
					stekZaPre = loadStack(stekZaPre);
					preOrder(stablo, stekZaPre);
				}
				else printf("Najpre morate inicijalizovati kvadrat i unesti skup vrednosti.\n\n");
				break;
			case 5:
				if (magicSum) {
					tStek = loadStack(tStek);
					postOrder(stablo, tStek);
				}
				else printf("Najpre morate inicijalizovati kvadrat i unesti skup vrednosti.\n\n");
				break;
			case 6:
			lab6:
				if (magicSum) {
					stablo = freeTree(stablo);
					free(cvor);
					heder = freeQueue(heder);
					tStek = freeStack(tStek);
					stekZaPre = freeStack(stekZaPre);
					arr = freeArray(arr);
					userArr = freeArray(userArr);
					square = freeSquare(n, square);
					magicSum = 0;
					counter = 0;
					arrayNum = 0;
					busyNodes = 0;
				}
				putchar('\n');
				break;
			case 7:
			lab7:
				if (magicSum) {
					stablo = freeTree(stablo);
					free(cvor);
					heder = freeQueue(heder);
					tStek = freeStack(tStek);
					stekZaPre = freeStack(stekZaPre);
					arr = freeArray(arr);
					userArr = freeArray(userArr);
					square = freeSquare(n, square);
				}
				putchar('\n');
				goto end;
				break;
			default:
				printf("Los izbor, pokusajte ponovo.\n\n");
				break;
		}
	}
	end:
	return 0;
}