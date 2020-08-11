/*
    Bruteforce my Sudoku v2.0

    Comments are in german.
    For any questions don't hesitate to contact me (felix.mark@hotmail.com).
    This program is developed for Linux Distributions, but can be adopted for Windows quite easily.

    Compile with g++ o.e. (for g++ the command would be: g++ -o3 -Wall BMS2.0.cpp -o BMS2.0).

    ~*.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~.~*~
    Copyright (c) 2016, Felix Mark

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


//INCLUDE SECTION--------------------------------------------
#include <iostream>
#include <cstdlib> //verwendet für system("cls");
#include <fstream> //für getchar() verwendet
#include "Clock.h" //Zum Zeitstoppen
#include "Color.h"
#include <unistd.h>
//INCLUDE SECTION--------------------------------------------

using namespace std;

//GLOBALE VARIABLEN------------------------------------------
int originalfeld[9][9]; //vom Benutzer eingegebenes Sudoku
int feldkopie[9][9];    //oft verwendetes Feld, welches im Laufe des Programms oft verändert wird.
long int steps = 0;     //Schritte die gemacht wurden, um Sudoku zu lösen
int x = 0, y = 0;       //x und y pos der zu betrachtenden Stelle
int test = 1;           //Zahl die getestet wird, ob Sie an x, y passt
bool showsteps = false, optionen = false;
int speedshow = 300000;
short int zweimalgleich = 0;
//GLOBALE VARIABLEN------------------------------------------

//FUNKTIONSDEFINITIONEN--------------------------------------
void vordefinieren();
void showboard(int a = 10, int b = 10);
void shownewboard();
void copyboard();
void einlesen();
void nullthefield();
bool bruteforce();
bool check();
int quadratbestimmung();
bool solved();
void credits();
bool eingabefehler();
//FUNKTIONSDEFINITIONEN--------------------------------------

//MAINFUNKTION-----------------------------------------------
int main()
{
	Clock stoppuhr;
	bool f = true;
    while (true)            //Programm hat interne abbruchbedingung, daher die endlosschleife
    {
        system("clear");                              //Wird noch öfter verwendet, löscht die Anzeige (auf Windows Rechnern!)
        nullthefield();
        x = 0; y = 0; test = 1; steps = 0;
        blue();

        if (f == true)
        	cout << endl;
        f = false;

        cout << endl << endl << endl << endl <<  "               " <<"Bruteforce my Sudoku V2.0" << endl << endl << endl; //Titel wird angezeigt
        red();
        cout << "               " << "[1] Sudoku einlesen" << endl;                //Sudoku muss in Folge vom Benutzer definiert werden
        white();
        cout << "               " << "[2] Beispielsudoku laden" << endl;          //Beispielsudoku wird aus dem Programm geladen
        cout << "               " << "[3] Züge anzeigen: " << (showsteps==true?"AN":"AUS") << endl;
        if (showsteps == true) cout << "               " << "[4] Anzeigegeschwindigkeit: " << speedshow << endl;
        cout << "               " << "[5] Mehr Sudokuvorlagen" << endl;
        cout << "               " << "[6] Credits" << endl;
        cout << "               " << "[0] Programm beenden" << endl;               //Programm wird beendet
        char choice = 0;
        system ("/bin/stty raw");
		choice = getchar();
		system ("/bin/stty cooked");
        if (choice != '4' && choice != '6')
        	system ("clear");
        if (choice == '0')            //Programm beenden wurde gewählt
            return 0;
		if (choice == '3')
			showsteps = !showsteps;
		if (choice == '4' && showsteps == true)
		{
			cout << endl << "               " << "Neue Geschwindigkeit (1.000.000 = 1s): ";
			cin >> speedshow;
		}
		if (choice == '5')
		{
			cout << "Sudokus zum Copy-and-Pasten in die Sudokueingabe:\n\n\nSehr einfach (69 Züge) :\n135740268640280139298163745723096850869501473514837092980654327450370916376912584\n\nEinfach (181 Züge) :\n095002100060008407004701035380040209000000000201090076410309600502100090006500380\n\nSudoku-Herz (4.186 Züge) :\n092000530400302001300060004506000409100806002030020010004000600000108000000030000\n\nSchwer (21.096 Züge) :\n100007090030020008009600500005300900010080002600004000300000010040000007007000300\n\nDas schwierigste Sudoku der Welt (24.526 Züge) :\n005300000800000020070010500400005300010070006003200080060500009004000030000009700\n\nSchwer Bruteforcebares Sudokuherz (180.786 Züge) :\n059000380200901006800070002900000003100000005020000070001000800000506000000020000" << endl; getchar();
		}
		if (choice == '6')
			credits();
        else if (choice == '1' || choice == '2')
		{
			if (choice == '2')
            	vordefinieren();        //zum Testen wird ein Sudoku vordefiniert
            else
		    	einlesen();              //Liest ein Sudoku ein

		    copyboard();                //Kopiert "originalfeld" in die "feldkopie" (Feldkopie wird später bearbeitet)
		    system("clear");
		    showboard();              //Die Funktion showboard zeigt den aktuellen Stand des Sudokus an
		    cout << endl << endl;
		    cout << "               " << "[1] Starten" << endl;
		    cout << "               " << "[0] Abbrechen" << endl << endl;
		    char start = '0';
			start = getchar();

		    if (start == '1')            //Es wurde starten gewählt
			{
				stoppuhr.start();
		        if (!eingabefehler() && bruteforce())       //Wenn die Funktion Bruteforce "true" zurück liefert, so ist das Sudoku gelöst
		        {
		            system("clear");
		            shownewboard();               //das gelöste Sudoku wird ausgegeben
		            cout << endl << endl << "               " << steps << " Züge ";
		            if (showsteps!=true) cout << "(" << stoppuhr.stop() << "s)" << endl; //Die Anzahl der Züge werden ausgegeben
		            getchar();                      //Wird öfters verwendet um auf die Eingabe des Benutzers zu warten
		        }
				else {cout << "               " << "Angabe fehlerhaft!" << endl; getchar();}
				getchar();
			}
		}
    }
}
//MAINFUNKTION-----------------------------------------------

//CREDITS----------------------------------------------------
void credits()
{
	cout<<endl<<"               "<<"CREDITS:"<<endl<<"               "<<"Programmierer: Felix Mark"<<endl<<"               "<<"Kontakt: felix.mark@hotmail.com" <<endl<<"               "<<"Copyright: Felix Mark © 2014" << endl;
	getchar();
}
//CREDITS----------------------------------------------------

//FELDER LÖSCHEN---------------------------------------------
void nullthefield() //Initialisieren der beiden Felder originalfeld und feldkopie
{
    for (int y = 0; y<9; y++)
        for (int x=0; x<9;x++)
        {
            feldkopie[x][y] = 0; //feldkopie = 0 an jeder Stelle des Feldes
            originalfeld[x][y] = 10; //originalfeld (ursprungseingabe) = 10 an jeder Stelle des Feldes
        }
}
//FELDER LÖSCHEN---------------------------------------------

//BENUTZER GIBT SUDOKU EIN-----------------------------------
void einlesen() //Liest das Sudoku Feld für Feld ein
{
    for (int y = 0; y<9; y++)
    {
        for (int x = 0; x<9; x++)
        {
        	system("clear");
        	cout << "\n\n\n\n";
    		cout << "               " << "Geben Sie ein Sudoku ein (Leer = 0) :" << endl;
        	showboard(x,y);
            system ("/bin/stty raw");
            while (originalfeld[x][y] > 9 || originalfeld[x][y] < 0)
            {
				originalfeld[x][y] = getchar()-48;
				system ("/bin/stty cooked");
				if (originalfeld[x][y] == 79)
				{
					originalfeld[x][y] = 10;
					if (x==0 && y > 0)
					{
						x = 7;
						y -= 1;
						originalfeld[8][y] = 10;
						break;
					}
					if (x >= 0)
						x = x-1;
					if (x >= 0 && y >= 0)
						originalfeld[x][y] = 10;
					if (x >= 0)
						x = x-1;
					break;
				}
			}
			cout << endl;
        }
    }
}
//BENUTZER GIBT SUDOKU EIN-----------------------------------

//SUDOKU WIRD GELÖST-----------------------------------------
bool bruteforce() //Löst das Sudoku mittels Backtracking
{
	while(true)
	{
	    steps++;
		if (showsteps == true)
		{
			system("clear");
			shownewboard();
			usleep(speedshow);
		}

	    for (; test < 10; test++) //test durchlaufen von test-9 (anfangs von 1-9, später von z.B. 5-9 weil im Feld bereits 5 probiert wurde)
	    {
			if (feldkopie[x][y] == 0) //Wenn das derzeit betrachtete Feld frei ist..
			{
		    	if (check()) //Wenn check "true" für "test" und die Position (x und y) zurückliefert
		    	{
		    	    feldkopie[x][y] = test; //..dann passt test hier hin, und dem Feld wird an der Stelle x,y, der Wert test zugewiesen
		    	    break; //damit nicht weiter probiert wird "break"-en wir die Schleife
		    	}
			}
	    }

	    if (solved()) //wenn "solved" true zurückliefert..
	    {
			return true; //gib true zurück (an die aufruffunktion (ursprünglich die main()))
	    }

	    if (feldkopie[x][y] == 0) //Wenn das Feld 0 ist, nachdem alle Zahlen von test-9 getestet wurden stimmt etwas im Sudoku davor nicht.
	    {
		    while (true)
		    {
		        x--;       //Eine Stelle zurück
		        if (x<0 && y>0)   //Wenn die Zeile zu Ende ist gehe eine Zeile höher
		        {
		            x = 8;
		            y--;
		        }
		        if (x < 0 && y == 0)
		        	return false;
		        if (feldkopie[x][y] != originalfeld[x][y]) //Wenn das jetzt betrachtete Feld nicht schon Ursprünglich da stand..
		        {
		            test = feldkopie[x][y]+1; //dann bekommt test den Wert von der Feldkopie +1 damit nicht noch einmal der selbe Wert wie vorhin genommen wird
		            feldkopie[x][y] = 0;      //Die Feldkopie an der neuen Stelle wird zudem auf 0 gesetzt, da Sie nun wieder beschreibbar sein soll
					break;
		        }
		    }
	    }
	    //"Normaler Fall"
	    else
	    {
			x++; //Die nächste Stelle wird betrachtet
			if (x > 8) //Nächste Zeile?
			{
		    	x = 0;
		    	y++;
			}
			test = 1; //test wird auf 1 gesetzt um alle Möglichkeiten für die nächste Stelle offen zu haben
		}
	}
}
//SUDOKU WIRD GELÖST-----------------------------------------

//SUDOKU GELÖST PRÜFER---------------------------------------
bool solved() //Überprüft ob Sudoku gelöst ist
{
    bool leer = 0;     //Hilfvariable
    for (int x = 0; x < 9; x++)
        for (int y = 0; y < 9; y++)
            if (feldkopie[x][y] == 0) //Ist irgendwo im Sudoku noch eine 0 enthalten..
                leer = 1;             //Setze leer auf 1
    if (leer == 0)                    //Wenn leer noch auf 0 steht, ist das Sudoku gelöst (daher wird true returned)
        return true;
    else
        return false;                 //das Sudoku ist nicht gelöst -> false wird retourned
}
//SUDOKU GELÖST PRÜFER---------------------------------------

//ÜBERPRÜFT OB TEST AN X Y MÖGLICH IST-----------------------
bool check() //Checkt ob Zahl passt oder nicht (Zeile, Spalte und Quadrat)
{
    for (int yc = 0; yc < 9; yc++) //vertikale Überprüfung
        if (feldkopie[x][yc] == test)
			return false;

    for (int xc = 0; xc < 9; xc++) //horizontale Überprüfung
        if (feldkopie[xc][y] == test)
            return false;

    for (int sq = 0; sq < 9; sq++) //Quadrat überprüfung
    {
        switch (quadratbestimmung()) //Je nachdem in welchem Fels x, y, derzeit sind, wir eine zahl (1-9) retourned
        {
            case 1:                   //Zahl liegt im 1.Feld
            {
				for (int x = 0; x < 3; x++)
					for (int y = 0; y < 3; y++)
						if (feldkopie[x][y] == test) //Gesamtes Feld wir überprüft
							return false; //Wenn die Zahl bereits vorkommt return false
				break;
            }
            case 2:                   //Zahl liegt im 2.Feld
            {
		        for (int x = 3; x < 6; x++) //3 / 0
		            for (int y = 0; y < 3; y++)
		                if (feldkopie[x][y] == test)
		                    return false;
		        break;
            }
            case 3:                   //Zahl liegt im 3.Feld
            {
                for (int x = 6; x < 9; x++) // 6 / 0
                    for (int y = 0; y < 3; y++)
                        if (feldkopie[x][y] == test)
                            return false;
                break;
            }
            case 4:                   //Zahl liegt im 4.Feld
            {
                for (int x = 0; x < 3; x++) // 0 / 3
                    for (int y = 3; y < 6; y++)
                        if (feldkopie[x][y] == test)
                        	return false;
                break;
            }
            case 5:                   //Zahl liegt im 5.Feld
            {
                for (int x = 3; x < 6; x++) // 3 / 3
                    for (int y = 3; y < 6; y++)
                        if (feldkopie[x][y] == test)
                            return false;
                break;
            }
            case 6:                   //Zahl liegt im 6.Feld
            {
                for (int x = 6; x < 9; x++) // 6 / 3
                    for (int y = 3; y < 6; y++)
                        if (feldkopie[x][y] == test)
                            return false;
                break;
            }
            case 7:                   //Zahl liegt im 7.Feld
            {
                for (int x = 0; x < 3; x++) // 0 / 6
                    for (int y = 6; y < 9; y++)
                        if (feldkopie[x][y] == test)
                            return false;
                break;
            }
            case 8:                   //Zahl liegt im 8.Feld
            {
				for (int x = 3; x < 6; x++) // 3 / 6
                    for (int y = 6; y < 9; y++)
                        if (feldkopie[x][y] == test)
                            return false;
                break;
            }
            case 9:                   //Zahl liegt im 9.Feld
            {
                for (int x = 6; x < 9; x++) // 6 / 6
                    for (int y = 6; y < 9; y++)
                        if (feldkopie[x][y] == test)
                            return false;
                break;
            }
        }
    }
	return true;
}
//ÜBERPRÜFT OB TEST AN X Y MÖGLICH IST-----------------------

//QUADRATBESTIMMER-------------------------------------------
int quadratbestimmung() //bestimmung des Quadrats in dem die Zahl liegt
{
    if (x < 3) //1.Quadratspalte
    {
        if (y < 3) //1.Quadratreihe
            return 1;
        if (y < 6) //2.Quadratreihe
            return 4;
        if (y < 9) //3.Quadratreihe
            return 7;
    }
                   //Und so weiter..
    else if (x >= 3 && x < 6)
    {
        if (y < 3)
            return 2;
        if (y < 6)
            return 5;
        if (y < 9)
            return 8;
    }

    else if (x >= 6 && x < 9)
    {
        if (y < 3)
            return 3;
        if (y < 6)
            return 6;
        if (y < 9)
            return 9;
    }
	return 0; //wird NIE zurückgegeben (der compiler will das trotzdem..)
}
//QUADRATBESTIMMER-------------------------------------------

//DEFINIERT EIN SUDOKU AUS DER ZEITUNG VOR-------------------
void vordefinieren() //Zum Testen des Programms
{
	for (int y = 0; y<9; y++)
        for (int x=0; x<9;x++)
            originalfeld[x][y] = 0; //originalfeld (ursprungseingabe) = 10 an jeder Stelle des Feldes
    originalfeld[4][0] = 7; //bestimmten Feldern werden Werte zugewiesen.. der Rest ist 0
    originalfeld[6][0] = 6;
    originalfeld[8][0] = 5;
    originalfeld[2][1] = 8;
    originalfeld[3][1] = 5;
    originalfeld[5][1] = 6;
    originalfeld[6][1] = 4;
    originalfeld[0][2] = 3;
    originalfeld[3][2] = 8;
    originalfeld[5][2] = 9;
    originalfeld[6][2] = 7;
    originalfeld[7][2] = 1;
    originalfeld[1][3] = 2;
    originalfeld[2][3] = 4;
    originalfeld[6][3] = 3;
    originalfeld[0][4] = 6;
    originalfeld[6][4] = 2;
    originalfeld[1][5] = 3;
    originalfeld[5][5] = 7;
    originalfeld[8][5] = 4;
    originalfeld[1][6] = 6;
    originalfeld[3][6] = 7;
    originalfeld[4][6] = 8;
    originalfeld[7][6] = 5;
    originalfeld[0][7] = 5;
    originalfeld[1][7] = 1;
    originalfeld[2][7] = 3;
    originalfeld[4][7] = 9;
    originalfeld[5][7] = 4;
    originalfeld[1][8] = 8;
    originalfeld[3][8] = 1;
    originalfeld[4][8] = 3;
    originalfeld[6][8] = 9;
    originalfeld[8][8] = 6;
}
//DEFINIERT EIN SUDOKU AUS DER ZEITUNG VOR-------------------

//ORIGINALFELDANZEIGE----------------------------------------
void showboard(int a, int b) //Zeigt das Originalfeld an
{
	cout << "\n\n\n\n";
    for (int y = 0; y < 9; y++)
    {
    	cout << "               ";
        for (int x = 0; x < 9; x++)
        {
            if (originalfeld[x][y] == 0 || originalfeld[x][y] == 10)
            {
            	if (x == a && y == b)
            		red();
            	else
            		black();
                cout << "0" << " ";
                white();
            }
            else
            {
                cout << originalfeld[x][y] << " ";
            }
            if (x == 2 || x == 5)
                cout << "| ";
        }
        if (y == 2 || y == 5)
            cout << endl << "               " << "------+-------+------";
        cout << endl;
    }
}
//ORIGINALFELDANZEIGE----------------------------------------

//ORIGINALFELDCHECK------------------------------------------
bool eingabefehler() //Zeigt das Originalfeld an
{
    for (; y < 9; y++)
    {
        for (; x < 9; x++)
        {
            if (originalfeld[x][y] != 0)
            {
            	test = originalfeld[x][y];
            	feldkopie[x][y] = 0;
            	if (!check())
            		return true;
            	feldkopie[x][y] = originalfeld[x][y];
            }
        }
    }
    x=0; y=0; test=1;
    return false;
}
//ORIGINALFELDCHECK------------------------------------------

//VERÄNDERTESFELDANZEIGE-------------------------------------
void shownewboard() //Zeigt das veränderte Feld an
{
	cout << "\n\n\n\n";
    for (int y = 0; y < 9; y++)
    {
    	cout << "               ";
        for (int x = 0; x < 9; x++)
        {
        	if (feldkopie[x][y] == originalfeld[x][y])
        	{
        		if (feldkopie[x][y] != 0)
        			white();
        		else black();
        		cout << feldkopie[x][y] << " ";
        		white();
        	}
        	else
        	{
        		blue();
            	cout << feldkopie[x][y] << " ";
            	white();
            }
            if (x == 2 || x == 5)
                cout << "| ";
        }
        if (y == 2 || y == 5)
            cout << endl << "               "<<"------+-------+------";
        cout << endl;
    }
}
//VERÄNDERTESFELDANZEIGE-------------------------------------

//FELD ZU FELDKOPIE KOPIERER---------------------------------
void copyboard() //Das Ursprungsbrett wird in die "feldkopie" kopiert
{
    for (int y = 0; y<9; y++)
        for (int x=0; x<9;x++)
            feldkopie[x][y] = originalfeld[x][y]; //Jedes Feld von feldkopie wird dem des Feldes auf dem "originalfeld" (originalfeld) gleichgesetzt
}
//FELD ZU FELDKOPIE KOPIERER---------------------------------
