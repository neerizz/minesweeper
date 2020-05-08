/* INSTRUCTIONS:
1)There are a certain number of mines hidden in a grid. Your task is to uncover all the cells except the ones holding mines.
2)You need to flag all the cells containing mines.
3)You will never lose in the first move.
4)If you select s, it will uncover that cell.
5)If the cell contains a number>0, it will uncover only that cell and display the number.
6)If the cell contains a number=0, it will recursively all others around it until there is a complete boundary of cells containing 
  numbers>0 around it.
5)The numbers in the cells represent the number of neighbours of that cell containg a mine.
  There can be a maximum of 8 neighbors of a cell as shown below:
					n1   n2   n3
					n4  root  n5
					n6   n7   n8
6)It is a strategic game, so don't click around randomly. All the best :)
Lastly! you need to enter moves in this format-->  row_number column_number s/f(whatever you choose)
*/

#include<bits/stdc++.h>
using namespace std;
const int MAXSIZE = 30;
int nrows;
int ncolumns;
int nmines;
int flags;
int row1;
int column1;

void difficulty()
{
	int temp;
	
	cout<<"Choose a difficulty level: "<<endl;
	cout<<"Enter 1 for beginner(9*9 cells and 10 mines)"<<endl;
	cout<<"Enter 2 for intermediate(16*16 cells and 40 mines)"<<endl;
	cout<<"Enter 3 for expert(16*30 cells and 99 mines)"<<endl;
	cin>>temp;
	if(temp==1)
	{
		nrows=9;
		ncolumns=9;
		nmines=10;
		flags=nmines;
	}
	else if(temp==2)
	{
		nrows=16;
		ncolumns=16;
		nmines=40;
		flags=nmines;
	}
	else if(temp==3)
	{
		nrows=16;
		ncolumns=30;
		nmines=99;
		flags=nmines;
	}
}

bool validmovechecker(char row,char column, char gameboard[MAXSIZE][MAXSIZE],int flags,char choice)
{
	if(row<='9')
	row1 = (int)row - '0';
	else
	row1 = (int)row - 39-'0';
	
	if(column<='9')
	column1 = (int)column - '0';
	else
	column1 = (int)column -39 -'0';
	
	if(row1>=nrows || column1>=ncolumns || row1<0 || column1<0)
	return false;
	else if(choice=='f' && (gameboard[row1][column1]!='.' && gameboard[row1][column1]!='F'))
	{
		cout<<"Cell is a number"<<endl;
		return false;	
	}
	else if(choice=='f' && flags==0 && gameboard[row1][column1]!='F')
	{
		cout<<"Sorry you have run out of flags"<<endl;
		return false;
	}
	else if(choice=='s' && gameboard[row1][column1]=='F')
	{
		cout<<"Cell is already flagged. Hence, it cannot be uncovered."<<endl;
		return false;
	}
	else if(choice=='s' && gameboard[row1][column1]!='.')
	{
		cout<<"Cell is already uncovered"<<endl;
		return false;
	}
	else if(choice!='f' && choice!='s')
	{
		cout<<"Please select out of f or s only."<<endl;
		return false;
	}
	
	return true;
}

void displaygame(char gameboard[MAXSIZE][MAXSIZE])
{
	int n=0;
	char extra = 'a';
	cout<<"  ";
	for(int i=0;i<ncolumns;i++)
	{
		if(n<=9)
		cout<<" "<<n++;
		else
		cout<<" "<<extra++;
	}
	
	cout<<endl;
	cout<<endl;
	n=0;
	extra='a';
	for(int i=0;i<nrows;i++)
	{
		if(i<=9)
		cout<<n<<" ";
		else
		cout<<extra<<" ";
		for(int j=0;j<ncolumns;j++)
		{
			cout<<" "<<gameboard[i][j];
		}
		if(i<=9)
		cout<<"  "<<n++<<endl;
		else
		cout<<"  "<<extra++<<endl;
	}
	cout<<endl;
	n=0;
	extra='a';
	cout<<"  ";
	for(int i=0;i<ncolumns;i++)
	{
		if(i<=9)
		cout<<" "<<n++;
		else
		cout<<" "<<extra++;
	}
	cout<<endl<<endl;
}

//counts the number of adjacent mines for any give row and column position of a single square
int countadjacentmines(char mineboard[MAXSIZE][MAXSIZE], char gameboard[MAXSIZE][MAXSIZE],int row1,int column1)
{
	
	int count=0;
	
	if(mineboard[row1][column1-1]=='#' && column1>0)
	count++;
	if(mineboard[row1][column1+1]=='#' && column1<ncolumns)
	count++;
	if(mineboard[row1-1][column1]=='#' && row1>0)
	count++;
	if(mineboard[row1+1][column1]=='#' && row1<nrows)
	count++;
	if(mineboard[row1-1][column1+1]=='#' && row1>0 && column1<ncolumns)
	count++;
	if(mineboard[row1-1][column1-1]=='#' && row1>0 && column1>0)
	count++;
	if(mineboard[row1+1][column1+1]=='#' && row1<nrows && column1<ncolumns)
	count++;
	if(mineboard[row1+1][column1-1]=='#' && row1<nrows && column1>0)
	count++;
	
	return count;
}

//mark mines in the gameboard after it is won or lost
void markmines(char gameboard[MAXSIZE][MAXSIZE],char mineboard[MAXSIZE][MAXSIZE], bool won)
{
	
	for(int i=0;i<nrows;i++)
	{
		for(int j=0;j<ncolumns;j++)
		{
			if(mineboard[i][j]=='#' && won==true)
			gameboard[i][j]='F';
			else if(mineboard[i][j]=='#' && won == false)
			gameboard[i][j]='#';
		}
	}
}

vector <pair<int,int>> getneighbours(int row1,int column1)
{
	vector <pair<int,int>> nei;
	
	for(int i=row1-1;i<=row1+1;i++)
	{
		for(int j=column1-1;j<=column1+1;j++)
		{
			if(i!=row1 || j!=column1)
			{
				if(i>=0 && i<nrows && j>=0 && j<ncolumns)
				nei.push_back(make_pair(i,j));
			}
		}
	}
	
	return nei;
}

void uncoverboard(char gameboard[MAXSIZE][MAXSIZE], char mineboard[MAXSIZE][MAXSIZE], int row1, int column1, int *cellsopened)
{
	if(gameboard[row1][column1]=='.')
	(*cellsopened)++;
	int count = countadjacentmines(mineboard,gameboard,row1,column1);
	gameboard[row1][column1]=count+'0';
	
	if(count==0)
	{
		vector <pair<int,int>> adjacent = getneighbours(row1,column1);
		for(int i=0;i<adjacent.size();i++)
		{
			if(gameboard[adjacent[i].first][adjacent[i].second]=='.')
			uncoverboard(gameboard,mineboard,adjacent[i].first,adjacent[i].second,cellsopened);
		}
	}
}

int main()
{
	difficulty();
	cout<<endl<<endl;
	char mineboard[MAXSIZE][MAXSIZE],gameboard[MAXSIZE][MAXSIZE];
	srand(time(0));
	char row,column;
	char choice;
	int allopenablecells = nrows*ncolumns-nmines;
	int cellsopened = 0;
	
	//initializing gameboard
	for(int i=0;i<nrows;i++)
	{
		for(int j=0;j<ncolumns;j++)
		{
			gameboard[i][j]='.';
		}
		cout<<endl;
	}
	
	//initializing mineboard
	for(int i=0;i<nrows;i++)
	{
		for(int j=0;j<ncolumns;j++)
		{
			mineboard[i][j]='.';
		}
		cout<<endl;
	}
	
	//mine assignment
	for(int i=0;i<nmines;i++)
	{
		label:
		int c=rand()%nrows,d=rand()%ncolumns;
		if(mineboard[c][d]=='#')
		goto label;
		else
		mineboard[c][d]='#';
	}
	
	cout<<"****Welcome to minesweeper****"<<endl<<"------------------------------"<<endl<<endl;
	
	bool gameplay=true;
	while(gameplay)
	{
		displaygame(gameboard);
		cout<<" "<<flags<<" flags left."<<endl;
		
		cout << "Enter your move, (row, column, safe(s)/flag(f)) -> ";
		cin>>row>>column>>choice;
		cout<<endl;
		while(!validmovechecker(row,column,gameboard,flags,choice))
		{
			cout<<"Illegal move. Please try again!"<<endl;
			cout << "Enter your move, (row, column, safe(s)/flag(f)) -> ";
			cin>>row>>column>>choice;
			cout<<endl;
		}
		//Ensuring first move does not lead to game over
		if(cellsopened==0)
		{
			if(mineboard[row1][column1]=='#')
			{
				mineboard[row1][column1]='.';
				label1:
				int a=rand()%nrows,b=rand()%ncolumns;
				if(mineboard[a][b]=='#')
				goto label1;
				else
				mineboard[a][b]='#';
			}
		}
		
		
		if(choice=='s')
		{
			if(gameboard[row1][column1]=='.' && mineboard[row1][column1]=='.')
			{
				uncoverboard(gameboard,mineboard,row1,column1,&cellsopened);
				if(cellsopened == allopenablecells)
				{
					markmines(gameboard,mineboard,true);
					displaygame(gameboard);
					cout<<"Congratulations, You won!!!";
					return 0;
				}
			}
			else if(gameboard[row1][column1]=='.' && mineboard[row1][column1]=='#')
			{
				markmines(gameboard,mineboard,false);
				displaygame(gameboard);
				cout<<"You lost :( Better luck next time!";
				return 0;
			}
		}
		else if(choice=='f')
		{
			if(gameboard[row1][column1]=='.')
			{
				gameboard[row1][column1]='F';
				flags--;
			}
			else if(gameboard[row1][column1]=='F')
			{
				gameboard[row1][column1]='.';
				flags++;
			}
		}
		
	}
	
	return 0;
}


