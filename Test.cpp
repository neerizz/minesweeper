#include<bits/stdc++.h>
using namespace std;
const int MAXSIZE = 30;
int nrows = 9;
int ncolumns = 9;
int nmines = 10;
int flags = nmines;

//2) Fix game for other difficulty levels
bool validmovechecker(int row,int column, char gameboard[MAXSIZE][MAXSIZE],int flags,char choice)
{
	if(row>=nrows || column>=ncolumns || row<0 || column<0)
	return false;
	else if(choice=='f' && (gameboard[row][column]!='.' && gameboard[row][column]!='F'))
	{
		cout<<"Cell is a number"<<endl;
		return false;	
	}
	else if(choice=='f' && flags==0 && gameboard[row][column]!='F')
	{
		cout<<"Sorry you have run out of flags"<<endl;
		return false;
	}
	else if(choice=='s' && gameboard[row][column]=='F')
	{
		cout<<"Cell is already flagged. Hence, it cannot be uncovered."<<endl;
		return false;
	}
	else if(choice=='s' && gameboard[row][column]!='.')
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
	cout<<"  ";
	for(int i=0;i<nrows;i++)
	cout<<" "<<n++;
	cout<<endl;
	cout<<endl;
	n=0;
	for(int i=0;i<nrows;i++)
	{
		cout<<n<<" ";
		for(int j=0;j<ncolumns;j++)
		{
			cout<<" "<<gameboard[i][j];
		}
		cout<<"  "<<n++<<endl;
	}
	cout<<endl;
	n=0;
	cout<<"  ";
	for(int i=0;i<nrows;i++)
	cout<<" "<<n++;
	cout<<endl<<endl;
}

//counts the number of adjacent mines for any give row and column position of a single square
int countadjacentmines(char mineboard[MAXSIZE][MAXSIZE], char gameboard[MAXSIZE][MAXSIZE],int row,int column)
{
	
	int count=0;
	
	if(mineboard[row][column-1]=='#' && column>0)
	count++;
	if(mineboard[row][column+1]=='#' && column<ncolumns)
	count++;
	if(mineboard[row-1][column]=='#' && row>0)
	count++;
	if(mineboard[row+1][column]=='#' && row<nrows)
	count++;
	if(mineboard[row-1][column+1]=='#' && row>0 && column<ncolumns)
	count++;
	if(mineboard[row-1][column-1]=='#' && row>0 && column>0)
	count++;
	if(mineboard[row+1][column+1]=='#' && row<nrows && column<ncolumns)
	count++;
	if(mineboard[row+1][column-1]=='#' && row<nrows && column>0)
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

vector <pair<int,int>> getneighbours(int row,int column)
{
	vector <pair<int,int>> nei;
	
	for(int i=row-1;i<=row+1;i++)
	{
		for(int j=column-1;j<=column+1;j++)
		{
			if(i!=row || j!=column)
			{
				if(i>=0 && i<nrows && j>=0 && j<ncolumns)
				nei.push_back(make_pair(i,j));
			}
		}
	}
	
	return nei;
}

void uncoverboard(char gameboard[MAXSIZE][MAXSIZE], char mineboard[MAXSIZE][MAXSIZE], int row, int column, int *cellsopened)
{
	if(gameboard[row][column]=='.')
	(*cellsopened)++;
	int count = countadjacentmines(mineboard,gameboard,row,column);
	gameboard[row][column]=count+'0';
	
	if(count==0)
	{
		vector <pair<int,int>> adjacent = getneighbours(row,column);
		for(int i=0;i<adjacent.size();i++)
		{
			if(gameboard[adjacent[i].first][adjacent[i].second]=='.')
			uncoverboard(gameboard,mineboard,adjacent[i].first,adjacent[i].second,cellsopened);
		}
	}
}

int main()
{
	nrows=9;
	ncolumns=9;
	nmines=10;
	flags=nmines;
	char mineboard[MAXSIZE][MAXSIZE],gameboard[MAXSIZE][MAXSIZE];
	srand(time(0));
	int row,column;
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
			if(mineboard[row][column]=='#')
			{
				mineboard[row][column]='.';
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
			if(gameboard[row][column]=='.' && mineboard[row][column]=='.')
			{
				uncoverboard(gameboard,mineboard,row,column,&cellsopened);
				if(cellsopened == allopenablecells)
				{
					markmines(gameboard,mineboard,true);
					displaygame(gameboard);
					cout<<"Congratulations, You won!!!";
					return 0;
				}
			}
			else if(gameboard[row][column]=='.' && mineboard[row][column]=='#')
			{
				markmines(gameboard,mineboard,false);
				displaygame(gameboard);
				cout<<"You lost :( Better luck next time!";
				return 0;
			}
		}
		else if(choice=='f')
		{
			if(gameboard[row][column]=='.')
			{
				gameboard[row][column]='F';
				flags--;
			}
			else if(gameboard[row][column]=='F')
			{
				gameboard[row][column]='.';
				flags++;
			}
		}
		
	}
	
	return 0;
}


