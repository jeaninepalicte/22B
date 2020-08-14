/*********

Friday May 8, 2020

Assignment 3:
This program creates a movie database by reading info from a user inputted
file name (movies.txt). It uses a struct called Movie that contains the movie
title, gross total, director's name, release date, and the movie's runtime--
stored in a dynamically allocated array. The user will be prompted to enter a
movie title (noncase sensitive) and will be shown the movie's info. If they
choose to favorite the movie, it will be added to a separate output file
(favorites.txt). Then prompted to continue or quit.

*********/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <string>
#include <cstring>

using namespace std;

struct Movie
{
    string Title, Director, ReleaseDate;
    double GrossTotal;
    int RunTime;
};

int numberOfLines(ifstream& inFile);
void populateMovieFromFile(ifstream&, Movie&);
void displayMovie(const Movie&);
Movie* createDatabaseFromFavorites(int& Count);
Movie* createDatabase(int&);
bool caseInsensitiveCmp(string, string);
bool CheckIfFileGood(std::string& FileName);
void findMovie(Movie*, int);
void saveToFile(const Movie&);
bool promptToContinue();
void displayFavorites();
bool AddtoFavorites();


int main()
{
    int Count;
    Movie* Movies = createDatabase(Count);
    do
    {
        findMovie(Movies, Count);
    } while (promptToContinue());
    cout << endl;

    displayFavorites();
    delete [] Movies;
}

/* The findMovie function accepts the Movie pointer array and the number of
movies. It prompts the user for a movie title and searches through the array
for the movie title.
--> returns nothing */
void findMovie(Movie* movie, int Count)
{
    string title;
    cout << "Enter a movie title to search for: ";
    getline(cin, title);
    cout << endl;

    for (int i = 0; i < Count; i++)
    {
        if (caseInsensitiveCmp(movie[i].Title, title))
        {
            displayMovie(movie[i]);
            if (AddtoFavorites())
            {
                cout << "Sucessfully added to favorites!" << endl;
                saveToFile(movie[i]);
            }
            return;
        }
    }
    cout << "Error: \'" << title << "\' not found in database. Please try 
            again." << endl;
}

/* The saveToFile function accepts the Movie object and saves it to the
outfile.
--> returns nothing */
void saveToFile(const Movie& movie)
{
    ofstream outFile("favorites.txt", ios::app);

    outFile << movie.Title << ", "
            << movie.GrossTotal << ", "
            << movie.Director << ", "
            << movie.ReleaseDate << ", "
            << movie.RunTime << endl;

    outFile.close();

}

/*  The displayFavorites function prints out favorite movies from the info in
the favorites file. If there are no saved movies, it prints out that the user
does not have any favorites.
--> returns nothing */
void displayFavorites()
{
    int Count;
    string FileName = "favorites.txt";
    if (!CheckIfFileGood(FileName))
    {
        cout << "You have no favorites." << endl;
        return;
    }
    Movie* favs = createDatabaseFromFavorites(Count);
    cout << "Your saved favorites are:\n" << endl;
    for (int i = 0; i < Count; i++)
    {
        displayMovie(favs[i]);
        cout << endl << endl;
    }
    delete [] favs;
}

/* The AddtoFavorites function prompts the user if they want to add the movie
to favorites.
--> returns a bool value */
bool AddtoFavorites()
{
    string yn;
    
    cout << "\nAdd To Favorites? ";
    cin >> yn;
    return toupper(yn[0]) == 'Y';
}

/* The promptToContinue fuction determines whether or not the user want to
continue the program and returns false or true based on their choice.
returns a bool */
bool promptToContinue()
{
   char choice;
   cout << "\nWould you like to exit? (y/n): ";
   cin >> choice;
   cout << endl;
   choice = toupper(choice);

   while (choice != 'Y' && choice != 'N')
   {
       cout << "Error, Please enter choice y/n";
       cout << "\n\nWould you like to exit? (y/n): ";
       cin >> choice;
       choice = toupper(choice);
   }
   if (choice == 'Y')
   {
       cin.ignore();
       return false;
   }
   cin.ignore();
   return true;
}

/* The createDatabaseFromFavorites function opens the favorites file and
populates the Movie array.
--> returns the Movie array pointer */
Movie* createDatabaseFromFavorites(int& Count)
{
    string FileName = "favorites.txt";
    ifstream inFile(FileName.c_str());
    Count = numberOfLines(inFile);

    Movie* arr = new Movie[Count];
    for (int i = 0; i < Count; i++)
    {
        populateMovieFromFile(inFile, arr[i]);
    }

    return arr;
}

/* The createDatabase function first prompts the user for the file name, and
prompts to try again if invalid. Using the numberOfLines function, it creates
a dynamically allocated array of movie objects through a for loop. Populates
the array using populateMovieFromFile.
--> returns the Movie array pointer */
Movie* createDatabase(int& Count)
{
    string FileName;
    char tmp[500];
    cout << "Please enter the filename: ";
    cin.getline(tmp, 500);
    FileName = tmp;

    cout << endl;
    
    if (!CheckIfFileGood(FileName))
    {
        return createDatabase(Count);
    }

    ifstream inFile(FileName.c_str());
    Count = numberOfLines(inFile);

    Movie* arr = new Movie[Count];
    for (int i = 0; i < Count; i++)
    {
        populateMovieFromFile(inFile, arr[i]);
    }
    inFile.close();

    return arr;
}

/* The displayMovie function accepts a Movie object passed by const reference
and displays the formatted contents of the object.
--> returns nothing */
void displayMovie(const Movie& movie)
{
    cout << setw(20) << right << "Title:" << " " << left << movie.Title 
         << endl;
    cout << setw(20) << right << "Director:" << left << movie.Director 
         << endl;
    cout << setw(20) << right << "Gross Total:" << " " << left 
         << movie.GrossTotal << endl;
    cout << setw(20) << right << "Release Date:" << left << movie.ReleaseDate
         << endl;
    cout << setw(20) << right << "Run Time:" << " " << left << movie.RunTime
         << endl;
}

/* The caseInsensitiveCmp function compares two strings to see if they are the
same. It first compares length and then uses toupper to make it non case
-sensitive.
--> returns nothing */
bool caseInsensitiveCmp(string first, string second)
{
    if (first.length() != second.length())
    {
        return false;
    }

    for (size_t i = 0; i < first.length(); i++)
    {
        if (toupper(first[i]) != toupper(second[i]))
        {
            return false;
        }
    }
    return true;
}

/* The CheckIfFileGood function makes sure a input file exists through a bool
value.
returns a bool value */
bool CheckIfFileGood(std::string& FileName)
{
    if (FileName.length() == 0) return false;
    ifstream inFile(FileName.c_str());
    bool IsGood = inFile.good();
    inFile.close();
    
    return IsGood;
}

/* The populateMovieFromFile function accepts the movie object and populates
one single ovie object. It uses a delimiter to read in a line from the file
with getline.
--> returns nothing */
void populateMovieFromFile(ifstream& inFile, Movie& movie)
{
    char tmp[200];
    int RunTime;
    inFile.getline(tmp, 200, ',');
    string Title = string(tmp);
    inFile.getline(tmp, 200, ',');
    double GrossTotal = (double)atof(tmp);
    inFile.getline(tmp, 200, ',');
    string Director = string(tmp);
    inFile.getline(tmp, 200, ',');
    string ReleaseDate = string(tmp);
    inFile >> RunTime;
    char i = inFile.peek();
    if (i == '\n')
    {
        inFile.get();
    }
    movie.Director = Director;
    movie.Title = Title;
    movie.GrossTotal = GrossTotal;
    movie.ReleaseDate = ReleaseDate;
    movie.RunTime = RunTime;
}

/* The numberOfLines function accepts an input file and returns an int that
represents the number of lines in the fine using getline.
--> returns count (int, number of lines) */
int numberOfLines(ifstream& inFile)
{
    int Count = 0;
    string line;
    char tmp[200];

    while (inFile.getline(tmp, 200))
    {
        Count++;
    }

    inFile.clear();
    inFile.seekg(0, ios::beg);

    return Count;
}

/* OUTPUT: 

--> No favorites:
Please enter the filename: movies.txt

Enter a movie title to search for: minons

Error: minons not found in database. Please try again.
Would you like to exit? (y/n): n

Enter a movie title to search for: minions

              Title: Minions
           Director: Pierre Coffin & Kyle Blada
        Gross Total: 1.159
       Release Date: 7/10/15
           Run Time: 91

Add To Favorites? n
Sucessfully added to favorites!

Would you like to exit? (y/n): y


You have no favorites


--> 1 favorite:
Please enter the filename: movies.txt

Enter a movie title to search for: the avengers

              Title: The Avengers
           Director: Joss Whedon
        Gross Total: 1.519
       Release Date: 5/4/12
           Run Time: 143

Add To Favorites? y
Sucessfully added to favorites!

Would you like to exit? (y/n): n

Enter a movie title to search for: minions

              Title: Minions
           Director: Pierre Coffin & Kyle Blada
        Gross Total: 1.159
       Release Date: 7/10/15
           Run Time: 91

Add To Favorites? n

Would you like to exit? (y/n): y


Your saved favorites are:

              Title: The Avengers
           Director:  Joss Whedon
        Gross Total: 1.519
       Release Date:  5/4/12
           Run Time: 143


--> 2 favorites:
Please enter the filename: movies.txt

Enter a movie title to search for: minions

              Title: Minions
           Director: Pierre Coffin & Kyle Blada
        Gross Total: 1.159
       Release Date: 7/10/15
           Run Time: 91

Add To Favorites? y
Sucessfully added to favorites!

Would you like to exit? (y/n): n

Enter a movie title to search for: the avengers

              Title: The Avengers
           Director: Joss Whedon
        Gross Total: 1.519
       Release Date: 5/4/12
           Run Time: 143

Add To Favorites? y
Sucessfully added to favorites!

Would you like to exit? (y/n): y


Your saved favorites are:

              Title: Minions
           Director:  Pierre Coffin & Kyle Blada
        Gross Total: 1.159
       Release Date:  7/10/15
           Run Time: 91

              Title: The Avengers
           Director:  Joss Whedon
        Gross Total: 1.519
       Release Date:  5/4/12
           Run Time: 143

*/
