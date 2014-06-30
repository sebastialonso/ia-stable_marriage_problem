#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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

  //Receives the ID of the bride, and the marriageMatrix
  void marry (int someone_id, vector< vector<bool> >& marriageMatrix)
  {
    marriageMatrix[id - 1][someone_id - 1] = true;
    marriageMatrix[someone_id - 1][id - 1] = true;
  }

  //Removes the element in index position and updates accordingly
  void removeAndUpdate(int index)
  {
    vector< vector<string>> updated_prefs = prefs;
    updated_prefs.erase(updated_prefs.begin() + index);
    prefs = updated_prefs;
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
  bool isSingle (vector< vector<bool> > marriageMatrix) 
  { 
    //If the whole id row is false, then IT IS single
    for (int i = 0; i < marriageMatrix[id - 1].size(); i++)
    {
      if (marriageMatrix[id - 1][i])
      {
        return false;
      }
    }
    return true;
  }
  int getCoupleId () { return couple_id;}

};
//END clase Man

//Codigo clase Couple
// class Couple
// {
//   Man man;
//   Man woman;
// public:
//   Couple(){
//   }

//   Couple(Man male, Man female)
//   {
//     man = male;
//     woman = female;
//     man.marry(woman.getId());
//     woman.marry(man.getId());
//   }
//   ~Couple(){}

//   Man getMan() { return man; }
//   Man getWoman() { return woman; }
// };

//END clase Couple
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
    vector< vector<string> > prefs = men[i].getPreferences();
    if (!prefs.empty())
    {
      return true;
    }
  }
  return false;
}

//Returns the first Man whose preference list is non-empty
Man firstWithNonEmptyPrefs(vector<Man> men){
  for (int i = 0; i < men.size(); i++)
  {
    if (!men[i].getPreferences().empty())
    {
      return men[i];
    }
  }
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
  vector< vector< vector<string> > > prefs;
  prefs = loadData(argv[1]);
  //Already have the data, safely free memory
  //TODO: free memory deleting prefs variable

  vector<Man> men;
  vector<Man> women;
  //Initialize the bool matrix
  vector< vector<bool> > marriageMatrix(prefs.size()/2, vector<bool> (prefs.size()/2, false) );
  
  //Construccion de individuos
  //Hombres
  for (int i = 0; i < prefs.size()/2; i++)
  {
    Man auxiliary_man(prefs[i], i + 1);
    men.push_back(auxiliary_man);
  }
  //Mujeres
  for (int i = prefs.size()/2; i < prefs.size(); i++)
  {
    Man auxiliary_woman(prefs[i], i - prefs.size()/2 + 1);
    women.push_back(auxiliary_woman);
  }
  //Fin construccion

  // men[0].remove_and_update(0);
  // cout << "No todos han propuesto:" << notAllHaveProposed(men) << endl;

  /* Cuando se hace algo como
  Man man = men[0]  se realiza una copia de ese vector. Todo cambio que haga en man NO
  se almacena en men[0]. El vector men se utilizara como data original. Las variables
  Man firstOne y Man bestGal serán utilizadas como variables temporales existentes solo 
  en cada iteracion. La informacion que se necesita guardar, esto es, quien se casa con quien
  (matrimonios), quedará almacenada en la matriz de matrimonio marriageMatrix
  */
  
  //mientras exista al menos un hombre m que no haya propuesto a todas
  while(notAllHaveProposed(men)){
    //Copia local de hombre
    Man firstOne = firstWithNonEmptyPrefs(men);
    cout << "nuesto campeon tiene id: " << firstOne.getId() << endl;
    //tomar la mujer w mejor rankeada (primera) que quede en la lista de m 
    //(hacer un pop o algo del vector, de manera que desaparezca de la lista de pref)
    //TODO: Revisar si es un vector de 1 elemento o n
    vector<string> bestGalId = firstOne.getFirstPreferenceAndRemove();
    //Copia local de mujer
    Man bestGal = women[stoi(bestGalId[0]) - 1];
    cout << "la chica tiene id: " << bestGal.getId() << endl;

    // si esta soltera
    if (bestGal.isSingle(marriageMatrix))
    {
      cout << "la chica esta soltera " << endl;
      cout << "marriageMatrix antes del casamiento" << endl;
      printMatrix(marriageMatrix);
      // m y w son pareja
      firstOne.marry(stoi(bestGalId[0]), marriageMatrix);
      cout << "marriageMatrix despues del casamiento" << endl;
      printMatrix(marriageMatrix);
    }
    else // else si w esta emparejada
    {
      // si w prefiere a m antes que a su esposo m'
        // m y w forman pareja
        // m' ahora es soltero
      // else w y m' siguen en pareja
      cout << "la chica esta casada y no se quiere separar. Try harder. " << endl;
    }
    break;
  }
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



}
