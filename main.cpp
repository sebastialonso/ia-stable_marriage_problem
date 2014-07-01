#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

// Codigo clase Man

class Man
{
  vector< vector<string> > prefs;
  int id;
  int couple_id;
public:
  //Constructor
  Man(){
    id = -1;
    couple_id = -1;
    vector< vector<string> > tmp;  prefs = tmp;
  }

  Man(vector < vector<string> > preferences, int idSource) {
    prefs = preferences;
    couple_id = -1;
    id = idSource;
  }

  //Destructor
  ~Man(){
  }

  //setters
  void setId (int idSource){
    id = idSource;
  }

  void setSingle ()
  {
    couple_id = -1;
  }

  void setCoupleId(int coupleId){
    couple_id = coupleId;
  }
  //Receives the ID of the bride, and the marriageMatrix
  //No changes in Man instances here
  void marry(Man& girl, vector< vector<bool> >& marriageMatrix)
  {
    //Set the matrix
    marriageMatrix[id - 1][girl.getId() - 1] = true;
    // marriageMatrix[girl.getId() - 1][id - 1] = true;
    //Set couple_id field
    setCoupleId(girl.getId());
    girl.setCoupleId(id);
  }

  //Receives the wife and performs the divorce operation, for both.
  void unmarry(Man& girl ,vector< vector<bool> >& marriageMatrix)
  {
    //Divorce inside the matrix
    marriageMatrix[id -1][girl.getId() - 1] = false;
    // marriageMatrix[girl.getId() - 1][id -1] = false;
    //Set couple_id field
    couple_id = -1;
    girl.couple_id = -1;
  }

  //Removes the element in index position and updates accordingly
  void removeAndUpdate(int index)
  {
    vector< vector<string>> updated_prefs = prefs;
    updated_prefs.erase(updated_prefs.begin() + index);
    prefs = updated_prefs;
  }

  //Returns whether a person prefers Man A over current couple
  bool prefersNewGuy(Man newGuy, vector< vector<string> > originalPrefs, vector< vector<bool> > marriageMatrix){
    //Necesito el id de la pareja actual idA
    //Debo comparar la posicion que ocupa idA frente a la posicion que ocupa otherPersonId
    //en la lista de preferencia original
    //POR lo tanto necesito la lista original sin eliminaciones, solo verla no modificarla
    if(newGuy.getId() == couple_id) {
      cout << "Uhmm...she's already married to that guy." << endl;
      return false;
    }
    else {
      string currentPartnerId = to_string(couple_id);
      string newGuyId = to_string(newGuy.getId());
      int currentPartnerPosition, newGuyPosition;
      for (int i = 0; i < originalPrefs.size(); i++)
      {
        //TODO: Manage vectors with more than one element in this comparison
        if(originalPrefs[i][0].compare(currentPartnerId) == 0) {
          currentPartnerPosition = i;
        }
        else if(originalPrefs[i][0].compare(newGuyId) == 0) {
          newGuyPosition = i;
        }
      }
      //ATTENTION We must compare minus. Zero should be bigger for us!
      if (newGuyPosition < currentPartnerPosition)
      {
        //Prefiere al actual
        return true;
      }
      else
      {
        return false;
      }
    }
  }

  //Returns whether a person still has women to propose to
  bool stillHasToPropose(){
    if (getPreferences().empty())
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  //Returns the front element of the vector, and updates accordingly
  vector<string> getFirstPreferenceAndRemove(){
    vector< vector<string>> updated_prefs = prefs;
    vector<string> front = updated_prefs.front();
    updated_prefs.erase(updated_prefs.begin());
    prefs = updated_prefs;
    return front;
  }

  //getters
  vector< vector<string> > getPreferences () { return prefs; }
  int getId () { return id; }
  bool isSingle () 
  { 
    //ARGUMENT = vector< vector<bool> > marriageMatrix
    //If the whole id row is false, then IT IS single
    // for (int i = 0; i < marriageMatrix[id - 1].size(); i++)
    // {
    //   if (marriageMatrix[id - 1][i])
    //   {
    //     return false;
    //   }
    // }
    // return true;
    if(couple_id == -1) {
      return true;
    }
    else {
      return false;
    }
  }
  int getCoupleId () { return couple_id;}

};
//END clase Man

vector< vector< vector<string> > > loadData(char const filename[]){
  vector< vector< vector<string> > > preferences;
  vector< vector<string> > indiv_prefs;
  vector<string> aux;
  ifstream inputFile(filename);

  bool first_line_flag = true;
  bool last_value_was_new_line = false;

  if (inputFile)
  {
    string value;
    while(inputFile >> value) {
      if (value.find(":") != std::string::npos)
      { 
        //Pasamos por nueva lista de preferencia
        if (first_line_flag)
        { 
          first_line_flag = false;
        }
        else
        {
          preferences.push_back(indiv_prefs);
          indiv_prefs.clear();
          aux.clear();
          if (last_value_was_new_line)
          {
            //Encontramos una lista vacia
            //se agrega a preferencias
            preferences.push_back(indiv_prefs);
            last_value_was_new_line = true;
          }
        }
      }
      else
      {
        //Si pasamos por un parentesis, seguir apendeando items hasta que termine el empate
        if (value.find("(") != std::string::npos)
        {
          while(inputFile >> value) {
              if (value.find(")") != std::string::npos)
              {
                //Se encontro el final del empate. Limpiamos aux para lo que venga
                // y agregamos el empate a la lista de pref.
                indiv_prefs.push_back(aux);
                aux.clear();
                break;
              }
              else 
              {
                aux.push_back(value);
              }
          }
        }
        else
        {
          //Si no hay empate, entonces es valor
          aux.push_back(value);
          indiv_prefs.push_back(aux);
          aux.clear();
        }
        last_value_was_new_line = false;
      }
    }
  }
  preferences.push_back(indiv_prefs);
  inputFile.close();


  return preferences;
}

string bigger_print(vector< vector< vector<string> > > tokens){
  cout << "listas de preferencia: " <<  tokens.size() << endl;
}

string print_vector(vector<string> vec)
{
  string result = "[ ";
  if (vec.empty())
  {
    return result += "]";
  }
  else 
  {
    for (int i = 0; i < vec.size(); i++)
    {
      result += vec[i];
      if (i != vec.size() - 1)
      {
        result += ", ";
      }
    }
    result += " ]";
    return result;  
  }
  
}
string print_vector_vector(vector< vector<string>> token){
  string result;
  for (int i = 0; i < token.size(); i++)
  {
    // cout << print_vector(token[i]);
    if (token[i].empty())
    { 
      result += "[ ]";
    }
    else
    {
      result += print_vector(token[i]);
    }
  }
  return result;
}

void print(string asdf)
{
  cout << asdf << endl;
}

//Retorna verdadero si existe al menos un hombre que aun tenga elementos en su lista de preferencias
bool notAllHaveProposed(vector<Man> men){
  for (int i = 0; i < men.size(); i++)
  {
    if (men[i].isSingle() and men[i].stillHasToPropose())
    {
      return true;
    }
  }
  return false;
}

int chosenMan(vector<Man> men, int currentChosenMan)
{
  for (int i = 0; i < men.size(); i++)
  {
    if (men[i].isSingle() and men[i].stillHasToPropose() and i != currentChosenMan)
    {
      return i;
    }
  }
  return currentChosenMan;
}
//Prints the marriage matrix
void printMatrix(vector< vector<bool> > matrix){
  for (int i = 0; i < matrix.size(); i++)
  {
    cout << "[ ";
    for (int j = 0; j < matrix[i].size(); j++)
    {
      if (j == matrix[i].size() - 1)
      {
        cout << matrix[i][j] << " ]" << endl; 
      }
      else
      {
        cout << matrix[i][j] << ", "; 
      }
    }
  }
}

int main(int argc, char const *argv[])
{ 
  //ALGORITMO A GRANDES RASGOS
  //mientras exista al menos un hombre m que no haya propuesto a todas
    //tomar la mujer w mejor rankeada que quede en la lista de m 
    //(hacer un pop o algo del vector, de manera que desaparezca de la lista de pref)
    // si esta soltera
      // m y w son pareja
    // else si w esta emparejada
      // si w prefiere a m antes que a su esposo m'
        // m y w forman pareja
        // m' ahora es soltero
      // else w y m' siguen en pareja

  // Starting the time measurement
  const clock_t begin_time = clock();
  
  vector< vector< vector<string> > > prefs;
  prefs = loadData(argv[1]);

  vector<Man> men;
  vector<Man> women;
  //Initialize the marriage matrix
  vector< vector<bool> > marriageMatrix(prefs.size()/2, vector<bool> (prefs.size()/2, false) );
  
  //People construction
  //Men (first half of preferences vector)
  for (int i = 0; i < prefs.size()/2; i++)
  {
    Man auxiliary_man(prefs[i], i + 1);
    men.push_back(auxiliary_man);
  }
  //Women (second half of preferences vector)
  for (int i = prefs.size()/2; i < prefs.size(); i++)
  {
    Man auxiliary_woman(prefs[i], i - prefs.size()/2 + 1);
    women.push_back(auxiliary_woman);
  }
  //Fin construccionpuesto:" << notAllHaveProposed(men) << endl;
    
  int currentManIndex = 0;
  while(notAllHaveProposed(men)){
    //mientras exista al menos un hombre m SOLTERO que no haya propuesto a todas
    // cout << "Our champ has id: " << men[currentManIndex].getId() << endl;
    // cout << "es soltero " << men[currentManIndex].isSingle() << endl;
    // cout << "le quedan personas para proponer " << men[currentManIndex].stillHasToPropose() << endl;
    
    //tomar la mujer w mejor rankeada (primera) que quede en la lista de m 
    //(hacer un pop o algo del vector, de manera que desaparezca de la lista de pref)
    //TODO: Revisar si es un vector de 1 elemento o n
    vector<string> bestGalId = men[currentManIndex].getFirstPreferenceAndRemove();
    //Copia local de mujer
    int bestGalIndex = stoi(bestGalId[0]) - 1;
    Man bestGal = women[bestGalIndex];
    // cout << "Girl id: " << women[bestGalIndex].getId() << endl;

    // si esta soltera
    if (women[bestGalIndex].isSingle())
    {
      // cout << "The girl is single. " << endl;
      // cout << "marriageMatrix before weeding" << endl;
      // printMatrix(marriageMatrix);
      // m y w son pareja
      men[currentManIndex].marry(women[bestGalIndex], marriageMatrix);
      // cout << "marriageMatrix after weeding" << endl;
      // printMatrix(marriageMatrix);
    }
    else // else si w esta emparejada
    { 
      // cout << "Estas son las preferencias de la chica id:" << women[bestGalIndex].getId() << endl;
      // cout << print_vector_vector(prefs[bestGalIndex + prefs.size()/ 2]) << endl;
      // cout << "She's married to " << women[bestGalIndex].getCoupleId() << "...";        
      // si w prefiere a m antes que a su esposo m'
      
      if (women[bestGalIndex].prefersNewGuy(men[currentManIndex], prefs[bestGalIndex + prefs.size()/ 2] , marriageMatrix))
      {

        // cout << "she's gonna cheat the husband!!" << endl;        
        // m' ahora es soltero
        // cout << "Loading divorce....";
        men[women[bestGalIndex].getCoupleId() - 1].unmarry(women[bestGalIndex], marriageMatrix);
        // cout << "marriageMatrix after divorce but before weeding" << endl;
        // printMatrix(marriageMatrix);
        // cout << "Done." << endl;
        // m y w forman pareja
        // cout << "Performing weeding..." << endl;
        men[currentManIndex].marry(women[bestGalIndex], marriageMatrix);
        // cout << "marriageMatrix after weeding" << endl;
        // printMatrix(marriageMatrix);
      }
      else
      {
        // // else w y m' siguen en pareja
        // cout << "nope, she's gonna stay with her man." << endl;
        // cout << "Sorry, champ. Try harder. " << endl;
      }
    }
    currentManIndex = chosenMan(men, currentManIndex);
  }
  // Measuring the elapsed time
  // cout << "We're done. This is the matching." << endl;
  printMatrix(marriageMatrix);
  cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " seconds" << endl;
}
