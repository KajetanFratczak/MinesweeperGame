#include <iostream>
#include<winbgim.h>
#include<time.h>
#include<sstream>
#include<conio.h>
#include <fstream>

using namespace std;

int n=16, m=16, xp=120, yp=120, bok=40, iw, jw, liczba_min=40, wynik[6],k,q;
int pomoc = 1;
bool start_gry=true, przegrana, wygrana;
time_t czas_teraz, czas_start;
int stan_zegara, level;
fstream file, plik;
string pseudo[6];





struct pole{
	bool mina;
	int liczba_min;
	bool show;
	int na;
};

pole tablica[16][30];

void l_mysz(int x, int y);
void p_mysz(int x, int y);
void start();
void licz_miny();
void draw();
void klocek(int x, int y, int a);
void start_game();
void start_game_pocz();
void start_game_expert();
void tabela();
void tabela_pocz();
void tabela_expert();
void draw1(int i, int j);
int liczba_bomb();
int liczba_choragiewek();
int liczba_klockow();
void licznik();
void miny();
void lose_game();
void win_game();
void zegar();
void restart();
void menu();
void podpis();
void clear();
void log();
void log_2();
void save_sort(int k);
void wpis(int k);  


int main()
{
	char z;
	cout<<"Podaj nick: ";
	cin>>pseudo[0];
	przegrana = false;
	wygrana = false;
	initwindow(1300,1000);

	setbkcolor(COLOR(100,100,100));
	
	cleardevice();
	menu();
	registermousehandler(WM_LBUTTONDOWN,l_mysz);
	registermousehandler(WM_RBUTTONDOWN,p_mysz);
	podpis();
	stan_zegara = 0;

	
	do{
		if(kbhit())
		{
			z = getch();
		}
		if(!start_gry)
		{
			czas_teraz = time(NULL);
			if(czas_teraz-czas_start!=stan_zegara && !wygrana && !przegrana)
			{
				stan_zegara = czas_teraz-czas_start;
				cout<<stan_zegara<<endl;
				zegar();
			}
			
		}
		Sleep(1);
	}while(z!=27);
	
	closegraph();
	return 0;
}

void menu()
{
	char wybor;
	cleardevice();
	setbkcolor(COLOR(100,100,100));
	settextstyle(1,0,5);
	setcolor(COLOR(212,182,83));
	outtextxy(50,1,"SAPER - Kajetan Fr�tczak klasa 2ap");
	outtextxy(50,140,"Pocz�tkuj�cy - 1");
	outtextxy(50,220,"Zaawansowany - 2");
	outtextxy(50,300,"Expert - 3");

	
	
	wybor = getch();

	switch(wybor)
	{
		case '1':
		{
			cleardevice();
			start_game_pocz();
			level = 1;
			k = 1;
			break;
		}
		case '2':
		{
			cleardevice();
			start_game();
			level = 2;
			k = 2;
			break;
		}
		case '3':
		{
			cleardevice();
			start_game_expert();
			level = 3;
			k = 3;
			break;	
		}
	
		default:
		{
			
			cleardevice();
			outtextxy(50,70,"ABY ZAGRA� WCI�NIJ ");
			outtextxy(50,140,"ODPOWIEDNI NUMER");
			outtextxy(50,210,"NA KLAWIATURZE!!!");
			Sleep(1500);
			menu();
		}
	}
}


void l_mysz(int x, int y)
{
	if(x>399 && x<481 && y>19 && y<101)
	{
		restart();
	}
	if(x>959 && x<1139 && y>789 && y<829)
	{
		log_2();
	}
	if(x>499 && x<701 && y>799 && y<851)
	{
		closegraph();
	}
	iw = (y-yp)/bok;
	jw = (x-xp)/bok;
	if(tablica[iw][jw].na!=1 && !wygrana && !przegrana && x>xp && x<xp+bok*m && y>yp && y<yp+bok*n)
	{
		if(start_gry)
		{
			start();
			licz_miny();
			start_gry = false;
			licznik();
			zegar();
			czas_start = time(NULL);
		}
		draw1(iw,jw);
		if(tablica[iw][jw].mina )
		{
			lose_game();
			log();
		}
		win_game();
		log();
	}
}

void start()
{
	int i, j, los_i, los_j, miny;
	srand(time(NULL));
	if(level==1) miny = 10;
	if(level==2) miny = 40;
	if(level==3) miny = 99;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			tablica[i][j].mina = false;
			tablica[i][j].show = false;
			tablica[i][j].na = 0;
		}
	}
		for(i=0;i<miny;i++)
		{
			los_i = rand()%n;
			los_j = rand()%m;
			if(!(los_i==iw && los_j==jw) && !tablica[los_i][los_j].mina)
			{
				tablica[los_i][los_j].mina = true;
			}
			else
			{
				i--;
			}
		}
}

void licz_miny()
{
	int i, j, licz;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			licz = 0;
			if(tablica[i+1][j].mina && i<n-1) licz++;
			if(tablica[i-1][j].mina && i>0) licz++;
			if(tablica[i][j+1].mina && j<m-1) licz++;
			if(tablica[i][j-1].mina && j>0) licz++;
			if(tablica[i-1][j-1].mina && j>0 && i>0) licz++;
			if(tablica[i-1][j+1].mina && j<m-1 && i>0) licz++;
			if(tablica[i+1][j+1].mina && j<m-1 && i<n-1) licz++;
			if(tablica[i+1][j-1].mina && j>0 && i<n-1) licz++;
			tablica[i][j].liczba_min = licz;
		}
	}
}

void draw()
{
	int i, j;
	stringstream liczba_min;
	
	setbkcolor(COLOR(150,150,150));

	for(i=0;i<n;i++)
	{	
		for(j=0;j<m;j++)
		{
			setcolor(COLOR(200,200,250));
			rectangle(xp+j*bok, yp+i*bok,xp+j*bok+bok, yp+i*bok+bok);
			setfillstyle(1,COLOR(150,150,150));
			floodfill(xp+j*bok+bok/2,yp+i*bok+bok/2,COLOR(200,200,250));
			
			if(tablica[i][j].mina )
			{
				setcolor(COLOR(0,0,0));
				circle(xp+j*bok+bok/2,yp+i*bok+bok/2,(bok/2)-5);
				setfillstyle(1,COLOR(0,0,0));
				floodfill(xp+j*bok+bok/2,yp+i*bok+bok/2,COLOR(0,0,0));
			}
			else
			{
				if(tablica[i][j].liczba_min>0)
				{
					if(tablica[i][j].liczba_min == 1) setcolor(COLOR(0,0,205));
					if(tablica[i][j].liczba_min == 2) setcolor(COLOR(50,200,50));
					if(tablica[i][j].liczba_min == 3) setcolor(COLOR(205,50,50));
					if(tablica[i][j].liczba_min == 4) setcolor(COLOR(139,0,0));
					if(tablica[i][j].liczba_min >= 5) setcolor(COLOR(105,0,128));
					liczba_min.str("");
					liczba_min<<tablica[i][j].liczba_min;
					outtextxy(xp+j*bok+12, yp+i*bok+11,liczba_min.str().c_str());
				}
			}
		}
	}
	
}

void klocek(int x, int y, int a)
{
    int m=5;
    setbkcolor(COLOR(234,234,234));
    setcolor(COLOR(220,220,220));
    rectangle(x,y,x+a,y+a);
    setfillstyle(1,COLOR(234,234,234));
    floodfill(x+a/2,y+a/2,COLOR(220,220,220));
    setcolor(COLOR(189,189,189));
    rectangle(x+m,y+m,x+a-m,y+a-m);
    setfillstyle(1,COLOR(189,189,189));
    floodfill(x+a/2,y+a/2,COLOR(189,189,189));
    setcolor(COLOR(122,122,122));
    line(x,y,x+m,y+m);
    line(x+a,y,x+a-m,y+m);
    line(x,y+a,x+m,y+a-m);
    line(x+a,y+a,x+a-m,y+a-m);
    setcolor(COLOR(122,122,122));
    setfillstyle(1,COLOR(122,122,122));
    rectangle(x+m,y+m,x+a-m,y+a-m);
    rectangle(x,y,x+a,y+a);
    floodfill(x+a-1,y+a/2,COLOR(122,122,122));
    floodfill(x+a/2,y+a-1,COLOR(122,122,122));
}

void start_game()
{
	int i, j;
	n=16;
	m=16;
	bok = 40;
	liczba_min=40;
	setbkcolor(COLOR(150,150,150));
	setcolor(COLOR(255,0,0));
	settextstyle(1,0,7);
	outtextxy(120,yp-80,"00");
	outtextxy(680,yp-80,"000");
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
				klocek(xp+j*bok,yp+i*bok,bok);
		}
	}
	klocek(400,20,80);
	setcolor(COLOR(255,255,0));
	circle(440,60,33);
	setfillstyle(1,COLOR(255,255,0));
	floodfill(440,60,COLOR(255,255,0));
	
	setcolor(COLOR(100,100,100));
	rectangle(960,790,1140,830);
		settextstyle(1,0,3);
	setcolor(COLOR(0,0,0));
		setbkcolor(COLOR(150,150,150));
	outtextxy(950,800,"REKORDY");
	
	
}
void start_game_pocz()
{
	int i, j;
	n=8;
	m=8;
	liczba_min=10;
	bok = 40;
	xp = 280;
	setbkcolor(COLOR(150,150,150));
	setcolor(COLOR(255,0,0));
	settextstyle(1,0,7);
	outtextxy(120,yp-80,"00");
	outtextxy(680,yp-80,"000");
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
				klocek(xp+j*bok,yp+i*bok,bok);
		}
	}
	klocek(400,20,80);
	setcolor(COLOR(255,255,0));
	circle(440,60,33); 
	setfillstyle(1,COLOR(255,255,0));
	floodfill(440,60,COLOR(255,255,0));
	
setcolor(COLOR(100,100,100));
	rectangle(960,790,1140,830);
		settextstyle(1,0,3);
	setcolor(COLOR(0,0,0));
		setbkcolor(COLOR(150,150,150));
	outtextxy(950,800,"REKORDY");

}

void start_game_expert()
{
	int i, j;
	n=16;
	m=30;
	liczba_min=99;
	bok = 40;
	xp = 50;
	setbkcolor(COLOR(150,150,150));
	setcolor(COLOR(255,0,0));
	settextstyle(1,0,7);
	outtextxy(120,yp-80,"00");
	outtextxy(680,yp-80,"000");
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
				klocek(xp+j*bok,yp+i*bok,bok);
		}
	}
	klocek(400,20,80);
	setcolor(COLOR(255,255,0));
	circle(440,60,33);
	setfillstyle(1,COLOR(255,255,0));
	floodfill(440,60,COLOR(255,255,0));
	
	setcolor(COLOR(100,100,100));
	rectangle(960,790,1140,830);
		settextstyle(1,0,3);
	setcolor(COLOR(0,0,0));
		setbkcolor(COLOR(150,150,150));
	outtextxy(950,800,"REKORDY");

}

void tabela()
{
	plik.open("wyniki_zaaw.txt",ios::in);
	string linia1,linia2,linia3,linia4,linia5;
	stringstream slowa1,slowa2,slowa3,slowa4,slowa5;
	slowa1.str("");
	slowa2.str("");
	slowa3.str("");
	slowa4.str("");
	slowa5.str("");
	for(int i=1;i<5;i++)
	{	
	getline(plik,linia1);
	getline(plik,linia2);
	getline(plik,linia3);
	getline(plik,linia4);
	getline(plik,linia5);
    

	slowa1<<linia1;
}
	slowa2<<linia2;
	slowa3<<linia3;
	slowa4<<linia4;
	slowa5<<linia5;
    
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,5);
	outtextxy(100,200,slowa1.str().c_str());
	outtextxy(100,300,slowa2.str().c_str());
	outtextxy(100,400,slowa3.str().c_str());
	outtextxy(100,500,slowa4.str().c_str());
	outtextxy(100,600,slowa5.str().c_str());
}
	


void tabela_pocz()
{
	plik.open("wyniki_pocz.txt",ios::in);
	string linia1,linia2,linia3,linia4,linia5;
	stringstream slowa1,slowa2,slowa3,slowa4,slowa5;
	slowa1.str("");
	slowa2.str("");
	slowa3.str("");
	slowa4.str("");
	slowa5.str("");
	
	for(int i=1;i<5;i++)
	{	
	getline(plik,linia1);
	getline(plik,linia2);
	getline(plik,linia3);
	getline(plik,linia4);
	getline(plik,linia5);
    

	slowa1<<linia1;
}
	slowa2<<linia2;
	slowa3<<linia3;
	slowa4<<linia4;
	slowa5<<linia5;
    
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,5);
	outtextxy(100,200,slowa1.str().c_str());
	outtextxy(100,300,slowa2.str().c_str());
	outtextxy(100,400,slowa3.str().c_str());
	outtextxy(100,500,slowa4.str().c_str());
	outtextxy(100,600,slowa5.str().c_str());


}

void tabela_expert()
{
	plik.open("wyniki_exp.txt",ios::in);
	string linia1,linia2,linia3,linia4,linia5;
	stringstream slowa1,slowa2,slowa3,slowa4,slowa5;
	slowa1.str("");
	slowa2.str("");
	slowa3.str("");
	slowa4.str("");
	slowa5.str("");
	for(int i=1;i<5;i++)
	{	
	getline(plik,linia1);
	getline(plik,linia2);
	getline(plik,linia3);
	getline(plik,linia4);
	getline(plik,linia5);
    

	slowa1<<linia1;
}
	slowa2<<linia2;
	slowa3<<linia3;
	slowa4<<linia4;
	slowa5<<linia5;
    
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,5);
	outtextxy(100,200,slowa1.str().c_str());
	outtextxy(100,300,slowa2.str().c_str());
	outtextxy(100,400,slowa3.str().c_str());
	outtextxy(100,500,slowa4.str().c_str());
	outtextxy(100,600,slowa5.str().c_str());
}


void draw1(int i, int j)
{	
	stringstream liczba_min;
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,3);
	
	
	if(!tablica[i][j].show && tablica[i][j].na!=1)
	{
		setcolor(COLOR(200,200,250));
		rectangle(xp+j*bok, yp+i*bok,xp+j*bok+bok, yp+i*bok+bok);
		setfillstyle(1,COLOR(150,150,150));
		floodfill(xp+j*bok+bok/2,yp+i*bok+bok/2,COLOR(200,200,250));
		if(tablica[i][j].mina )
		{
			setcolor(COLOR(0,0,0));
			circle(xp+j*bok+bok/2,yp+i*bok+bok/2,(bok/2)-5);
			setfillstyle(1,COLOR(0,0,0));
			floodfill(xp+j*bok+bok/2,yp+i*bok+bok/2,COLOR(0,0,0));
		}
		else
		{
			if(tablica[i][j].liczba_min>0)
			{
				if(tablica[i][j].liczba_min == 1) setcolor(COLOR(0,0,205));
				if(tablica[i][j].liczba_min == 2) setcolor(COLOR(50,200,50));
				if(tablica[i][j].liczba_min == 3) setcolor(COLOR(205,50,50));
				if(tablica[i][j].liczba_min == 4) setcolor(COLOR(139,0,0));
				if(tablica[i][j].liczba_min >= 5) setcolor(COLOR(105,0,128));
				liczba_min.str("");
				liczba_min<<tablica[i][j].liczba_min;
				outtextxy(xp+j*bok+12, yp+i*bok+11,liczba_min.str().c_str());
			}
		}
		tablica[i][j].show = true;
	}
	if(tablica[i][j].liczba_min==0 && i>0 && !tablica[i][j].mina)
	{
		if(!tablica[i-1][j].show) draw1(i-1,j);
	}
	if(tablica[i][j].liczba_min==0 && i<n-1 && !tablica[i][j].mina)
	{
		if(!tablica[i+1][j].show) draw1(i+1,j);
	}
	if(tablica[i][j].liczba_min==0 && j>0 && !tablica[i][j].mina)
	{
		if(!tablica[i][j-1].show) draw1(i,j-1);
	}
	if(tablica[i][j].liczba_min==0 && j<m-1 && !tablica[i][j].mina)
	{
		if(!tablica[i][j+1].show) draw1(i,j+1);
	}
	if(tablica[i][j].liczba_min==0 && j>0 && i>0 && !tablica[i][j].mina)
	{
		if(!tablica[i-1][j-1].show) draw1(i-1,j-1);
	}
	if(tablica[i][j].liczba_min==0 && j<m-1 && i>0 && !tablica[i][j].mina)
	{
		if(!tablica[i-1][j+1].show) draw1(i-1,j+1);
	}
	if(tablica[i][j].liczba_min==0 && j<m-1 && i<n-1 && !tablica[i][j].mina)
	{
		if(!tablica[i+1][j+1].show) draw1(i+1,j+1);
	}
	if(tablica[i][j].liczba_min==0 && j>0 && i<n-1 && !tablica[i][j].mina)
	{
		if(!tablica[i+1][j-1].show) draw1(i+1,j-1);
	}
}

void p_mysz(int x, int y)
{
	iw = (y-yp)/bok;
	jw = (x-xp)/bok;
	
	if(!wygrana && !przegrana && x>xp && x<xp+bok*m && y>yp && y<yp+bok*n && !tablica[iw][jw].show)
	{
		if(tablica[iw][jw].na==0)
		{
			setcolor(COLOR(255,0,0));
            line(xp+jw*bok+9,yp+iw*bok+15,xp+jw*bok+bok/2,yp+iw*bok+9);
            line(xp+jw*bok+9,yp+iw*bok+15,xp+jw*bok+bok/2,yp+iw*bok+21);
            line(xp+jw*bok+bok/2,yp+iw*bok+9,xp+jw*bok+bok/2,yp+iw*bok+21);
            setfillstyle(1,COLOR(255,0,0));
            floodfill(xp+jw*bok+10,yp+iw*bok+15,COLOR(255,0,0));
            setcolor(COLOR(0,0,0));
            line(xp+jw*bok+bok/2+1,yp+iw*bok+9,xp+jw*bok+bok/2+1,yp+iw*bok+27);
            line(xp+jw*bok+bok/2+1,yp+iw*bok+27,xp+jw*bok+9,yp+iw*bok+bok-9);
            line(xp+jw*bok+bok/2+1,yp+iw*bok+27,xp+jw*bok+bok-9,yp+iw*bok+bok-9);
            line(xp+jw*bok+9,yp+iw*bok+bok-9,xp+jw*bok+bok-9,yp+iw*bok+bok-9);
            setfillstyle(1,COLOR(0,0,0));
            floodfill(xp+jw*bok+9+10,yp+iw*bok+bok-9-1,COLOR(0,0,0));
            tablica[iw][jw].na=1;
		}
		else
		{
			if(tablica[iw][jw].na==1)
			{
				klocek(xp+jw*bok, yp+iw*bok, bok);
				settextstyle(1,0,2);
				setcolor(COLOR(0,0,205));
				setbkcolor(COLOR(189,189,189));
				outtextxy(xp+jw*bok+12, yp+iw*bok+10,"?");
				tablica[iw][jw].na=2;
			}
			else
			{
				if(tablica[iw][jw].na==2)
				{
					klocek(xp+jw*bok, yp+iw*bok, bok);
					tablica[iw][jw].na=0;
				}
			}
		}
	}
	if(!start_gry && !przegrana) licznik();
}

int liczba_bomb()
{
	int licz=0, i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			if(tablica[i][j].mina)
			{
				licz++;
			}
		}
	}
	return licz;
}

int liczba_klockow()
{
	int licz=0, i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			if(!tablica[i][j].show)
			{
				licz++;
			}
		}
	}
	return licz;
}

int liczba_choragiewek()
{
	int licz=0, i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			if(tablica[i][j].na == 1)
			{
				licz++;
			}
		}
	}
	return licz;
}

void licznik()
{
	stringstream stan_licznika;
	stan_licznika.str("");
	if(liczba_bomb()-liczba_choragiewek()<10) stan_licznika<<"0";
	stan_licznika<<liczba_bomb()-liczba_choragiewek();
	
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,7);
	outtextxy(120,yp-80,stan_licznika.str().c_str());
}

void lose_game()
{
	int i, j;
	przegrana = true;
	miny();
}

void win_game()
{
	wygrana = true;
	pomoc = 1;
	if(liczba_bomb()==liczba_choragiewek()  && liczba_klockow()==liczba_bomb() &&!przegrana && pomoc==1)
	{	
		if(level==1)
	{
		wynik[0]=stan_zegara;
		wpis(k);
	    save_sort(k);
	    pomoc = 0;
	}
	
        if(level==2)
	{
		wynik[0]=stan_zegara;
		wpis(k);
	    save_sort(k);
	    pomoc = 0;

	}
	
        if(level==3)
	{
		wynik[0]=stan_zegara;
		wpis(k);
	    save_sort(k);
	    pomoc = 0;

	}
	}
	else wygrana = false;
}

void miny()
{
	int i, j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			if(tablica[i][j].mina)
			{
				setcolor(COLOR(200,200,250));
				rectangle(xp+j*bok, yp+i*bok,xp+j*bok+bok, yp+i*bok+bok);
				setfillstyle(1,COLOR(150,150,150));
				floodfill(xp+j*bok+bok/2,yp+i*bok+bok/2,COLOR(200,200,250));	
				setcolor(COLOR(0,0,0));
				circle(xp+j*bok+bok/2,yp+i*bok+bok/2,(bok/2)-5);
				setfillstyle(1,COLOR(0,0,0));
				floodfill(xp+j*bok+bok/2,yp+i*bok+bok/2,COLOR(0,0,0));
	
			}
		}
	}
}

void zegar()
{
	stringstream czas;
	czas.str("");
	if(czas_teraz-czas_start>9 && czas_teraz-czas_start<100) czas<<"0";
	if(czas_teraz-czas_start<10) czas<<"00";
	if(czas_teraz-czas_start>99) czas<<"";
	czas<<czas_teraz-czas_start;
	
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,7);
	outtextxy(680,yp-80,czas.str().c_str());
}

void podpis()
{
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,6);
	outtextxy(150,800,"Kajetan Fr�tczak 2ap");
}
void clear()
{
	setcolor(COLOR(100,100,100));
	setbkcolor(COLOR(100,100,100));
	settextstyle(1,0,3);
	outtextxy(xp+170,yp+650,"PRZEGRANA");	
}

void log()
{
	if(przegrana)
	{
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,3);
	outtextxy(xp+170,yp+650,"PRZEGRANA");
	}
	
	if(wygrana)
	{
	setcolor(COLOR(255,0,0));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,2);
	outtextxy(xp+170,yp+650,"WYGRANA");
	}
}

void wpis(int k)
{
    if(k==1)
    {
        plik.open("wyniki_pocz.txt",ios::in);
    }
    if(k==2)
    {
        plik.open("wyniki_zaaw.txt",ios::in);
    }
    if(k==3)
    {
        plik.open("wyniki_exp.txt",ios::in);
    }
    q=1;
    string linia;
    int p;
    while(getline(plik,linia))
    {
        p=linia.find_last_of(" ");
        if(p==NULL)
        {
          pseudo[q]=linia.substr(0,p-(linia.size()-9));
		}
		else if(p!=0)
        {
         pseudo[q]=linia.substr(9,p-16);

		}
        if(linia[p+2])
        {
            wynik[q]=10*(linia[p+1]-48)+linia[p+2]-48;
        }
        else
        {
            wynik[q]=linia[p+1]-48;
        }
        q++;
    }
    
    plik.close();
}

void save_sort(int k)
{
    if(k==1)
    {
        plik.open("wyniki_pocz.txt",ios::out);
    }
    if(k==2)
    {
        plik.open("wyniki_zaaw.txt",ios::out);
    }
    if(k==3)
    {
        plik.open("wyniki_exp.txt",ios::out);
    }
    int i,j,pom1;
    string pom2;
    for(i=0; i<q-1; i++)    //Sortowanie
    {
        for(j=0; j<q-i-1; j++)
        {
            if(wynik[j]>wynik[j+1])
            {
                pom1=wynik[j];
                wynik[j]=wynik[j+1];
                wynik[j+1]=pom1;
                pom2=pseudo[j];
                pseudo[j]=pseudo[j+1];
                pseudo[j+1]=pom2;
            }
        }
    }
    if(q==6)
    {
        q--;
    }
    for(int i=0; i<q; i++)
    {
        plik<<i+1<<". "<<"Nick: "<<pseudo[i]<<" "<<"Czas : "<<wynik[i]<<endl;
    }
    plik.close();
}


void restart()
{
	start_gry = true;
	przegrana = false;
	wygrana = false;
	clear();
	if(level == 1) start_game_pocz();
	if(level == 2) start_game();
	if(level == 3) start_game_expert();
}

void log_2()
{
	if(level == 1) 
	{
	cleardevice();
	clear();
	start_gry=true;
	przegrana = false;
	wygrana = false;
	setbkcolor(COLOR(100,100,100));
	settextstyle(1,0,5);
	setcolor(COLOR(255,0,0));
	outtextxy(100,50,"REKORDY - POCZATKUJACY");

	setcolor(COLOR(150,150,150));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,3);
	outtextxy(xp+170,yp+650,"PRZEGRANA");
	
	setcolor(COLOR(100,100,100));
	rectangle(500,800,700,850);
	settextstyle(1,0,3);
	setcolor(COLOR(0,0,0));
	setbkcolor(COLOR(150,150,150));
	outtextxy(510,815,"WYJSCIE");
	tabela_pocz();
	}

	if(level == 2) 
	{
	cleardevice();
	clear();
	start_gry=true;
	przegrana = false;
	wygrana = false;
	setbkcolor(COLOR(100,100,100));
	settextstyle(1,0,5);
	setcolor(COLOR(255,0,0));
	outtextxy(100,50,"REKORDY - ZAAWANSOWANY");
	
	setcolor(COLOR(150,150,150));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,3);
	outtextxy(xp+170,yp+650,"PRZEGRANA");
	
	setcolor(COLOR(100,100,100));
	rectangle(500,800,700,850);
	settextstyle(1,0,3);
	setcolor(COLOR(0,0,0));
	setbkcolor(COLOR(150,150,150));
	outtextxy(510,815,"WYJSCIE");
	tabela();
	}

	if(level == 3) 
	{
	cleardevice();
	clear();
	start_gry=true;
	przegrana = false;
	wygrana = false;
	setbkcolor(COLOR(100,100,100));
	settextstyle(1,0,5);
	setcolor(COLOR(255,0,0));
	outtextxy(100,50,"REKORDY - EXPERT");

	setcolor(COLOR(150,150,150));
	setbkcolor(COLOR(150,150,150));
	settextstyle(1,0,3);
	outtextxy(xp+170,yp+650,"PRZEGRANA");
	
	setcolor(COLOR(150,150,150));
	rectangle(500,800,700,850);
	settextstyle(1,0,3);
	setcolor(COLOR(0,0,0));
	setbkcolor(COLOR(150,150,150));
	outtextxy(510,815,"WYJSCIE");
	tabela_expert();
	}
}

