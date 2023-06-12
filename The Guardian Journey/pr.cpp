#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
#include<vector>
#include<time.h>
#include<limits>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include<random>
#include<list>
#include<algorithm>
#include<climits>
#include <queue>
#include<ctime>
#include<cstdlib>
using namespace std;

struct Guardian //declaracion de la estructura del guardian
{	
	//atributos del guardian
	string name;
	int powerLevel;
	string mainMaster;
	string village;
	vector<Guardian*> aprendices;
	vector<string> recorrido;	
	bool isMaster() const //verificar si el guardian es maestro y sacar el tamaño de los aprendices del guardian
	{
		return aprendices.size() > 0; 
	}
	Guardian() //constructor por defecto
	{

	}
	Guardian(string name) //constructor por parametros
	{
		this->name = name;
	}
};

struct Aldea //declarcion de la estructura de la aldea
{
	//atributos de la aldea
    string nombre;
	string conectadaCon;
	vector<Aldea*> aldeas;
};

////////////////////////////////////////////////////////////////

bool comparador(Guardian* first, Guardian* second) //comparacionde niveles de poder para posteriormente ordenarlos del mas fuerte al mas debil
{
	return first->powerLevel>second->powerLevel;
}

class GrafoAldea //declaracion de clase
{
	private:
		unordered_map<string, unordered_set<string>> conexiones;     //mapa donde se guardaran las conexciones entre las aldeas
		unordered_map<string,vector<Guardian*>> guardianesporAldea; //mapa en el cual se almacenan los guardianes dentro de sus respectivas aldeas
		
	public:

		GrafoAldea();                           //constructor por defecto
		GrafoAldea(const list<Aldea*>& aldeas);//constructor para almacenar una lista de aldeas
		~GrafoAldea();                        //destructor
		
		void agregarAldea(const string& nombre);                                      //funcion para añadir una aldea
		void agregarConexion(const string& aldea, const string& connectedAldea);     //funcion para añadir una conexion entre aldeas a traves de aristas
		bool verificarConexion(const string& aldea, const string& connectedAldea);  //funcion para verificar si es que una aldea esta conectada o no a otra
		void imprimirGrafo();                                                      //funcion para imprimir el grafo en forma de lista de adyacencia
		void viajarEntreAldeas(Guardian* g, const string& origen, const string& destino, const list<Aldea*>& aldeas); //funcion que permite viajar entre aldeas, toma la aldea del guardian y le permite viajar a otra
		bool existeAldea(const string& aldea, const list<Aldea*>& aldeas);                                                                       //funcion para determinar si una aldea existe en la lista de aldeas 																
		void cargarGuardianes(list<Guardian*> guardianes, list<Aldea*>aldeas);                                                                  //funcion para poder cargas la lista de guardianes en el grafo y almacenarlos dependiendo de su aldea
		void imprimirGuardianesporAldea();                                                                                                     //funcion para imprimir los guardianes dependiendo de su respectiva aldea
		void Alquimista(Guardian* g,const string& aldea, const string& connectedAldea);                                                       //funcion para viajar a cualquier aldea

};

GrafoAldea::GrafoAldea()//constructor por defecto
{
	cout<<"constructor por defecto"<<endl;
}

GrafoAldea::GrafoAldea(const list<Aldea*>& aldeas)//constructor para almacenar una lista de aldeas
{
	for(const Aldea* aldea: aldeas)
	{
		agregarAldea(aldea->nombre);
		
		if (!aldea->conectadaCon.empty()) //establecer conexiones
		{
            stringstream ss(aldea->conectadaCon);// Separar las aldeas conectadas por comas
            string connectedVilage;
            while (getline(ss, connectedVilage, ',')) 
			{
                
                agregarConexion(aldea->nombre, connectedVilage);// Agregar conexión entre las aldeas
            }
        }

		
	}
	
}

GrafoAldea::~GrafoAldea()//destructor
{
	cout<<"llamando a destructor"<<endl;
}

void GrafoAldea::agregarAldea(const string& nombre)//funcion para añadir una aldea
{
	conexiones[nombre]; // Crea una entrada en el mapa para la aldea sin conexiones
	guardianesporAldea[nombre];

}

void GrafoAldea::agregarConexion(const string& aldea, const string& connectedAldea)//funcion para añadir una conexion entre aldeas a traves de aristas
{
	conexiones[aldea].insert(connectedAldea);
	conexiones[connectedAldea].insert(aldea);
}

bool GrafoAldea::verificarConexion(const string& aldea, const string& connectedAldea)//funcion para verificar si es que una aldea esta conectada o no a otra
{
	return conexiones[aldea].count(connectedAldea)>0; //returna el tamaño de la concexion y si es mayor que 0 es que esta conectada
}

void GrafoAldea::imprimirGrafo()//funcion para imprimir el grafo en forma de lista de adyacencia
{
	for(const auto& intro : conexiones) //recorriendo las conexiones entre aldeas en el mapa para poder imprimirlas 
	{
		const string& aldea = intro.first;
		const unordered_set<string>& conexionesAldea = intro.second;

		cout<< "Aldea: "<<aldea<<endl;
		cout<<"Conexiones: ";
		for(const string& conexion: conexionesAldea)
		{
			cout<<conexion<<" ";
		}
		cout<<endl;  
	}
}

void GrafoAldea::viajarEntreAldeas(Guardian* g, const string& origen, const string& destino, const list<Aldea*>& aldeas)//funcion que permite viajar entre aldeas, toma la aldea del guardian y le permite viajar a otra
{
	if (!existeAldea(destino, aldeas)) //verificacion de la existencia de la aldea
	{
		cout << "Una o ambas aldeas no existen" << endl;
		return;
	}

	if (!verificarConexion(origen, destino)) //verificacion si es que esta conectada
	{
		cout << "Las aldeas no están conectadas" << endl;
		return;
	}

	// Almacenar la aldea actual del Guardian
	string aldeaActual = g->village;

	for (const Aldea* aldea : aldeas) // Buscar la aldea de destino en la lista de aldeas y asignarla como la nueva aldea del Guardian
	{
		if (aldea->nombre == destino) 
		{
			g->village = aldea->nombre;

			
      		g->recorrido.push_back(aldea->nombre);// Agregar el nombre de la aldea actual al recorrido del Guardian

			break;
		}
	}
	
	g->powerLevel +=1;	// Incrementar los puntos del Guardian en 1
	cout << "Viaje completado correctamente" << endl;


} 

bool GrafoAldea::existeAldea(const string& aldea, const list<Aldea*>& aldeas)//funcion para determinar si una aldea existe en la lista de aldeas 																
{
    for (const Aldea* a : aldeas)
    {
        if (a->nombre== aldea)
		{
			return true;
			cout<<"La aldea existe"<<endl;
		}
            
    }
    return false;
}

void GrafoAldea::cargarGuardianes(list<Guardian*> guardianes, list<Aldea*>aldeas) //funcion para poder cargas la lista de guardianes en el grafo y almacenarlos dependiendo de su aldea
{
	if(!guardianes.empty() && !aldeas.empty())
	{
		// Reiniciar los guardianes por aldea
        guardianesporAldea.clear();
		// Agregar las aldeas al grafo
		for (const Aldea* aldea : aldeas)
		{
			agregarAldea(aldea->nombre);

			// Agregar las aldeas conectadas al grafo
			if (!aldea->conectadaCon.empty())
			{
				stringstream ss(aldea->conectadaCon);
				string connectedVilage;
				while (getline(ss, connectedVilage, ','))
				{
					agregarAldea(connectedVilage);
				}
			}
		}
				
		for(Guardian* guardian: guardianes)
		{
			// Verificar si la aldea del guardián existe en el grafo
			if (guardianesporAldea.find(guardian->village) != guardianesporAldea.end())
			{
				guardianesporAldea[guardian->village].push_back(guardian);
			}
			else
			{
				cout << "La aldea " << guardian->village << " no existe en el grafo" << endl;
			}
		}
	}	
}

void GrafoAldea::imprimirGuardianesporAldea()//funcion para imprimir los guardianes dependiendo de su respectiva aldea
{
	for (auto& pair: guardianesporAldea)
	{	
		cout<<"Aldea:"<<pair.first<<endl;
		for(auto& guardian: pair.second)
		{
			cout<<" Guardianes: ";
			cout<<guardian->name<<endl;
		}
	}
}

void GrafoAldea::Alquimista(Guardian* g, const string& aldea, const string& connectedAldea)//funcion para viajar a cualquier aldea
{				
    srand(time(NULL));                   // Inicializar la semilla para generar números aleatorios
    int puntosRestados = rand() % 3 + 2;// Generar un número aleatorio entre 2 y 4
   
    if (g->powerLevel >= puntosRestados)// Verificar si el jugador tiene suficientes puntos de poder
    { 
        agregarConexion(aldea, connectedAldea);  // Establecer la conexión entre las aldeas
        g->powerLevel -= puntosRestados;        // Restar puntos de poder al jugador según el número aleatorio generado
		g->village = connectedAldea;           // Actualizar la aldea del guardián
        cout << "Se establecio la conexion entre las aldeas " << aldea << " y " << connectedAldea << " por " << puntosRestados << " puntos de poder" << endl;
		cout << "Viajaste a la aldea " << connectedAldea << endl;
    }
    else
    {
        cout << "No tienes suficientes puntos de poder para establecer la conexión" << endl;
    }
}

////////////////////////////////////////////////////////////////

class GuardiansTree
{
	private:

	vector<Guardian*> vectorGuardianes;              //vector donde se guardan los guardianes cargados desde el archivo
	unordered_map<string, vector<Guardian*>> aldea; //vector para guardar las aldeas y sus guardianes
	vector<GuardiansTree*>children;                //vector que sirve para almacenar los hijos del arbol
	Guardian* root;                                 
	vector<Guardian*> findWeakestGuardians();
	
	Guardian* findGuardian(const string& nameSearch) //funcion para buscar a un guardian en el vector de guardianes donde se almacena la lista de guardianes
	{
		for(Guardian* guardian: vectorGuardianes)
		{
			if(guardian->name == nameSearch)
			{
				return guardian;
			}
		}
		return nullptr;
	}
	
	void updateTreeStructure() //funcion para poder actualizar el arbol en caso de que haya un guardian sin aprendices
	{
        for (Guardian* guardian : vectorGuardianes) 
		{
            if (guardian->mainMaster != "") 
			{
                Guardian* master = findGuardian(guardian->mainMaster);
                if (master != nullptr) 
				{
                    master->aprendices.push_back(guardian);
                }
            }
            else 
			{
                root = guardian;
            }
        }
		cout<<"Arbol Actualizado"<<endl;
    }
	
	void printGuardTree(Guardian* guardian, int indent) //funcion para imprimir el arbol de guardianes
	{
        if (guardian != nullptr) 
		{
            cout << string(indent, ' ') << "- " << guardian->name << " Puntos de Poder: " << guardian->powerLevel << ", Aldea Perteneciente: " << guardian->village << " " <<endl;
            for (Guardian* aprendiz : guardian->aprendices) 
			{
                printGuardTree(aprendiz, indent + 4);
            }
        }
    }

	public:

	GuardiansTree() : root(nullptr) {} //constructor para acceder a la raiz del arbol


	
	const unordered_map<string, vector<Guardian*>>& getAldeaMap() const;           //funcion para obtener los datos del mapa de aldeas del arbol
	void loadGuardList(const list<Guardian*> guardianes);                         //funcion para cargar la lista de guardianes tras haber cargado el archivo
	Guardian* findGuardianFromVillage(Guardian* guardian, const string& village);//funcion para encontrar a un guardian aprendiz dentro de una aldea
	Guardian* getRoot();                                                        //funcion para obtener la raiz del arbol
	void addApprentice(Guardian* apprentice, const string& masterName);        //funcion para añadir un guardian al arbol y si es un aprendiz se mete a las ramas del maestro
	void printGuardians();                                                    //funcion para imprimir el arbol de guardianes  
	void printAldeaGuardians();                                              //funcion para imprimir las aldeas y sus guardianes   
	void addToAldea(Guardian* guardian, const string& village);             //funcion para añadir al guardian a su respectiva aldea dentro del mapa de aldeas    
	Guardian* selectGuardian(Guardian* guardian, const string& nameSearch);//funcion para seleccionar a un guardian del arbol
	vector<Guardian*> getAldeaGuardians(const string& village);           //funcion para obtener la aldea a la cual pertenece el guardian 
	void updateHierarchy();                                              //funcion para actualizar la jerarquia del arbol despues de modificarlo  
	void clearHierarchy(GuardiansTree* node);                           //funcion para limpiar los nodos del arbol de jerarquia
	void buildHierarchy(GuardiansTree* node);                          //funcion para actualizar la jerarquia de los aprendices del arbol tras modificar el nombre de un guardian en caso de ser maestro 
	bool isAldeaExistente(const string& nombrealdea);                 //funcion para verificar si una aldea existe  
	void updateGuardianNameInTree(Guardian* node, const string& targetName, const string& newName); //funcion que actualiza el nombre del guardian seleccionado dentro del arbol
	void printHierarchy() const;                                                                   //funcion que se encarga de imprimir el arbol con la jerarquia actualizada
	void printHierarchyHelper(Guardian* node, int level) const;                                   //funcion que imprime la informacion del arbol actualizado y es llamada por el printhierarchy  
	
};

const unordered_map<string, vector<Guardian*>>& GuardiansTree::getAldeaMap() const
{
	return aldea;
} 

void GuardiansTree::loadGuardList(const list<Guardian*> guardianes)
{
	vectorGuardianes.clear(); // Limpiar el vector antes de cargar los nuevos guardianes
	aldea.clear();
	if(!guardianes.empty())
	{
		for(Guardian* listGuard: guardianes)
		{
				
			vectorGuardianes.push_back(listGuard);	
			cout<<"guardian cargado de la lista"<<endl;		
			addApprentice(listGuard,listGuard->mainMaster); //funcion para insertar un aprendiz en el arbol
			addToAldea(listGuard,listGuard->village); //funcion para almacenar a los guardianes en sus respectivas aldeas
		}
	}
	else
	{
		cout<<"lista vacia"<<endl;
	}
}

Guardian* GuardiansTree::findGuardianFromVillage(Guardian* guardian, const string& village) 
{
    if (guardian != nullptr) 
	{
        if (strcmp(guardian->village.c_str(),village.c_str()) == 0) 
		{
            return guardian;
        }
        for (Guardian* aprendiz : guardian->aprendices) 
		{
            return findGuardianFromVillage(aprendiz, village);
        }
    }
    return nullptr;

}

Guardian* GuardiansTree::getRoot()
{
	return root;
}

void GuardiansTree::addApprentice(Guardian* apprentice, const string& masterName)
{
	if (root == nullptr)
    {
        root = apprentice;
        cout << "root" << endl;
    }
    else
    {
        Guardian* master = findGuardian(masterName);
        if (master != nullptr)
        {
            master->aprendices.push_back(apprentice);
        }
    }
}

void GuardiansTree::printGuardians()
{
	printGuardTree(root,0);
}

void GuardiansTree::printAldeaGuardians()
{
	cout << "Guardianes por aldea:" << endl;
    for (const auto& pair : aldea)
    {
        const string& aldea = pair.first;
        const vector<Guardian*>& guardianes = pair.second;

        cout << "Aldea: " << aldea << endl;
        for (Guardian* guardian : guardianes)
        {
            cout << "- " << guardian->name <<",Puntos de Poder:"<<guardian->powerLevel << endl;
        }
        cout << endl;
    }
}

void GuardiansTree::addToAldea(Guardian* guardian, const string& village)
{
	if (guardian != nullptr)
	{
		if (guardian->village == village)
		{
			aldea[village].push_back(guardian);
		}
		for (Guardian* aprendiz : guardian->aprendices)
		{
			addToAldea(aprendiz, village);
		}
	}
}

Guardian* GuardiansTree::selectGuardian(Guardian* guardian, const string& nameSearch)
{
	if (guardian != nullptr) 
	{
		if (guardian->name == nameSearch) 
		{
			return guardian;
		}
		for (Guardian* aprendiz : guardian->aprendices) 
		{
			Guardian* selectedGuardian = selectGuardian(aprendiz, nameSearch);
			if (selectedGuardian != nullptr) 
			{
				return selectedGuardian;
			}
		}
	}
	// Si no se encuentra el personaje en el árbol, se devuelve nullptr.
	return nullptr;
}

vector<Guardian*> GuardiansTree::getAldeaGuardians(const string& village)
{
	vector<Guardian*> guardians;
	if (aldea.count(village) > 0)
	{
		guardians = aldea[village];
	}
	return guardians;
}

void GuardiansTree::updateHierarchy()
{
	// Reiniciar la jerarquía del árbol
	clearHierarchy(this);

	// Actualizar la estructura del árbol
	buildHierarchy(this);

	cout << "Jerarquia actualizada" << endl;
}

void GuardiansTree::clearHierarchy(GuardiansTree* node)
{
	if (node == nullptr)
	{
		return;
	}
	for (GuardiansTree* child : node->children)
	{
		clearHierarchy(child);
		delete child;
	}
	node->children.clear();  // Limpiar el vector de hijos del nodo actual
	delete node;  // Eliminar el nodo actual
}

void GuardiansTree::buildHierarchy(GuardiansTree* node)
{
	if (node == nullptr)
	{
		return;
	}
	Guardian* guardian = node->getRoot();  // Obtener el nodo raíz del árbol

	if (guardian == nullptr)
		return;

	for (Guardian* apprentice : guardian->aprendices)
	{
		GuardiansTree* child = new GuardiansTree();
		// Realizar las operaciones necesarias en el nuevo nodo child, como asignar el aprendiz
		list<Guardian*> guardianList;
		guardianList.push_back(apprentice);
		child->loadGuardList(guardianList);
		node->children.push_back(child);
		buildHierarchy(child);
	}
}

bool GuardiansTree::isAldeaExistente(const string& nombrealdea) 
{
	return aldea.find(nombrealdea) != aldea.end();
}

void GuardiansTree::updateGuardianNameInTree(Guardian* node, const string& targetName, const string& newName)
{
	if (node == nullptr) 
	{
    	return;
    }
	if (node->name == targetName) 
	{
		node->name = newName;
		cout << "Nombre del guardian actualizado en el arbol de jerarquia" << endl;
    	cout << "Nuevo nombre: " << node->name << endl;
	}
	// Recorrer los aprendices del nodo actual
	for (Guardian* aprendiz : node->aprendices) 
	{
		updateGuardianNameInTree(aprendiz, targetName, newName);
	}

	// Buscar y actualizar el nombre del maestro
	if (node->isMaster() && node->mainMaster == targetName) 
	{
		node->mainMaster = newName;
		cout << "Nombre del maestro actualizado en el arbol de jerarquia" << endl;
		cout << "Nuevo nombre del maestro: " << node->mainMaster << endl;
	}	
} 

void GuardiansTree::printHierarchy() const 
{
	if (root == nullptr) 
	{
		cout << "el arbol esta vacio" << endl;
		return;
	}

	printHierarchyHelper(root, 0);
}

void GuardiansTree::printHierarchyHelper(Guardian* node, int level) const
{
	// Imprimir el nivel actual
	for (int i = 0; i < level; ++i) 
	{
		cout << "    ";
	}

	// Imprimir el nombre del guardian
	cout << "|-- " << node->name << endl;

	// Imprimir los aprendices recursivamente
	for (Guardian* aprendiz : node->aprendices) 
	{
		printHierarchyHelper(aprendiz, level + 1);
	}
} 

void getGuardiansData(Guardian* guardian, int iterador) //funcion para obtener los datos de los guardianes
{
	if(guardian != nullptr)
	{
		cout << string(iterador, ' ') << "- " << guardian->name << " Puntos de Poder: " << guardian->powerLevel << ", Aldea Perteneciente: " << guardian->village << " " << endl;
        for (Guardian* aprendiz : guardian->aprendices) 
		{
            getGuardiansData(aprendiz, iterador + 4);
        }
	}
}

int getRollResult() //funcion de dado
{
	static random_device rd;                                // Obtener una semilla aleatoria del dispositivo o del generador de números aleatorios
	static mt19937 gen(rd());                              // Generador de números aleatorios utilizando la semilla aleatoria
	static uniform_int_distribution<> distribution(1, 6); // Rango de valores entre 1 y 6 

	int dado = distribution(gen); // Generar un número aleatorio entre 1 y 6
	cout<<"dado:"<<dado<<endl;
	return dado;                // Retornar el resultado del dado
}

void cargarGuardianesExternos(list<Guardian*>& guardianes, const string& filename, GuardiansTree& guardTree) //funcion para cargar el archivo de los guardianes por externo
{
	ifstream file(filename);
	if (file.is_open()) 
	{
		string line;
		getline(file, line); // Descartar la primera línea del archivo

		while (getline(file, line)) 
		{
			stringstream ss(line);
			string nameC, power, mainMaster, village;
			int powerC;
			getline(ss, nameC, ',');
			getline(ss, power, ',');
			getline(ss, mainMaster, ',');
			getline(ss, village, ',');
			
			powerC = stoi(power);
			
			Guardian* guard = new Guardian();
			guard->name = nameC;
			guard->powerLevel = powerC;
			guard->mainMaster = mainMaster;
			guard->village = village;
			
			guardianes.push_back(guard);
		}
		file.close();
	} 
	else 
	{
		cout << "Error, no se ha podido abrir el archivo" << endl;
	}

	guardTree.loadGuardList(guardianes);

	// Imprimir los datos de los guardianes almacenados en la lista
	cout << "----------------INFORMACION DE LA LISTA----------------" << endl;
	cout << "La cantidad de personajes son: " << guardianes.size() << endl;
	for (const Guardian* guard : guardianes) 
	{
		cout << "Nombre Jugador: " << guard->name << endl;
		cout << "Maestro Jugador: " << guard->mainMaster << endl;
		cout << "Poder Jugador: " << guard->powerLevel << endl;
		cout << "Villa Jugador: " << guard->village << endl;
		cout << "------------------------------" << endl;
	}
	
	cout << "----------------ARBOL DE GUARDIANES----------------:" << endl;
	guardTree.printGuardians();
	cout << "---------ALDEAS Y SUS HABITANTES---------" << endl;
	guardTree.printAldeaGuardians();
}

void cargarAldeasExternas(list<Aldea*>& aldeas, const string& aldea)//funcion para poder cargar las aldeas por archivo externo
{
	ifstream filev(aldea);
	if(filev.is_open())
	{
		string liner;
		getline(filev,liner); // Descartar la primera línea del archivo
		while(getline(filev,liner))
		{
			stringstream sd(liner);
			string villa,connectedVilage;

			getline(sd,villa,',');
			getline(sd,connectedVilage,',');

			Aldea* villages = new Aldea();
			villages->nombre = villa;
			villages->conectadaCon = connectedVilage;
			aldeas.push_back(villages);
						
		}
		filev.close();
					
	}
	else
	{
		cout<<"error, no se ha podido abrir el archivo"<<endl;
	}

}

void crearAprendiz(string& nombre, string& Mainvillage) //funcion para crear un aprendiz
{
	cout<<"Introduce el nombre de tu guardian"<<endl;
	cin>> nombre;

	int opcion;

	cout<<"Selecciona tu aldea:"<<endl;
	cout<<"1: Mountain Village"<<endl;
	cout<<"2: Forest Village"<<endl;
	cout<<"3: Capital City"<<endl;
	cout<<"4: Cliff Village"<<endl;
	cout<<"5: Hill Village"<<endl;
	cout<<"6: Desert Village"<<endl;
	cout<<"7: Valley Village"<<endl;
	cout<<"8: Storm Village"<<endl;
	cout<<"9: River Village"<<endl;
	cout<<"10: Coast Village"<<endl;
	cout<<"11: Island Village"<<endl;
	cout<<"12: Tesla"<<endl;
	cin>>opcion;
	switch(opcion)
	{
		case 1 :
		Mainvillage = "Mountain Village";
		break;
		case 2 :
		Mainvillage = "Forest Village";
		break;
		case 3 :
		Mainvillage = "Capital City";
		break;
		case 4 :
		Mainvillage = "Cliff Village";
		break;
		case 5 :
		Mainvillage = "Hill Village";
		break;
		case 6 :
		Mainvillage = "Desert Village";
		break;
		case 7 :
		Mainvillage = "Valley Village";
		break;
		case 8 :
		Mainvillage = "Storm Village";
		break;
		case 9 :
		Mainvillage = "River Village";
		break;
		case 10 :
		Mainvillage = "Coast Village";
		break;
		case 11 :
		Mainvillage = "Island Village";
		break;
		case 12:
		Mainvillage = "Tesla";
		break;
	}	
}

void agregarAprendiz(vector<Guardian*>& aprendices, const string& name, int powerLevel, const string& village) //funcion que sirve para poder agregar un aprendiz al vector de aprendices de la estructura guardian
{
	Guardian* newGuardian = new Guardian();
    newGuardian->name = name;
    newGuardian->powerLevel = powerLevel;
    newGuardian->village = village;
	aprendices.push_back(newGuardian);
}

Guardian* seleccionarGuardian(list<Guardian*>& guardianes, GuardiansTree& guardTree, Guardian* newGuard, Guardian* root, Guardian* seleccionado) //funcion para seleccionar al guardian
{
	root = guardTree.getRoot();
	if (root != nullptr) 
	{
		for (Guardian* aprendiz : root->aprendices) 
		{
			getGuardiansData(aprendiz,0);
		}
	}
	string nombre;
	cout<<"Ingrese el nombre del guardian que quiere seleccionar:"<<endl;
	cin>>nombre;
	newGuard = guardTree.selectGuardian(root,nombre);
	if(newGuard != nullptr)
	{
		// Buscar la aldea del guardian seleccionado en el mapa de aldeas
		vector<Guardian*> guardianesAldea = guardTree.getAldeaGuardians(newGuard->village);

		cout << "Guardian seleccionado: " << newGuard->name << endl;
		newGuard->powerLevel = 50;
		cout << "Puntos de Poder: " << newGuard->powerLevel << endl;
		cout << "Aldea Perteneciente: " << newGuard->village << endl;
		cout<<"DATOS PASADOS AL PERSONAJE SELECCIONADO"<<endl;
		seleccionado = new Guardian();
		seleccionado->name = newGuard->name;
		seleccionado->powerLevel = newGuard->powerLevel;
		seleccionado->village = newGuard->village;

		//iterador al Guardian en la lista
		auto it = find_if(guardianes.begin(), guardianes.end(), [&nombre](const Guardian* guardian) 
		{
			return guardian->name == nombre;
		});

		// Verifica si se encontró el Guardian en la lista
		if (it != guardianes.end()) 
		{
			// Acceder al objeto Guardian a través del iterador y actualizar su nombre
			(*it)->name = "Xoclo";
			cout << "Nombre del Guardian actualizado en la lista" << endl;
		} 
		else 
		{
			cout << "No se encontró el Guardian con el nombre especificado." << endl;
		}

		if(guardTree.isAldeaExistente(newGuard->village))
		{

			// Actualizar el nombre del guardian en el mapa de aldeas
			for(Guardian* guardian : guardianesAldea)
			{
				if(guardian == newGuard)
				{
					// Actualizar el nombre del maestro en el mapa de aldeas
					if (newGuard->isMaster()) 
					{
						guardian->name = "Xoclo";
					}
					else
					{
						// Actualizar el nombre del aprendiz en el mapa de aldeas
						guardian->name = "Xoclo";
					}
					cout << "Nombre del guardian actualizado en el mapa de aldeas" << endl;
					cout << "Nuevo nombre: " << guardian->name << endl;

					// Actualizar el nombre en el árbol de jerarquía
					guardTree.updateGuardianNameInTree(guardTree.getRoot(), newGuard->name, guardian->name);
					break; // Salir del bucle una vez que se ha encontrado y actualizado el guardian
								
				}
									
			}
			// Imprimir la información de la lista
			cout << "Informacion de la lista de guardianes:" << endl;
			for (const Guardian* guardian : guardianes) 
			{
				cout << "Nombre: " << guardian->name << ", Aldea: " << guardian->village << endl;
			}
			guardTree.printAldeaGuardians();// Imprimir la información del mapa de aldeas
			guardTree.updateHierarchy(); // Actualizar la estructura del árbol
			guardTree.printHierarchy(); // Imprimir el árbol actualizado

			cout<<"----------INFORMACION DEL GUARDIAN SELECCIONADO----------"<<endl;
			cout<<"NOMBRE: "<<seleccionado->name<<endl;
			cout<<"PUNTOS DE PODER: "<<seleccionado->powerLevel<<endl;
			cout<<"ALDEA PERTENECIENTE: "<<seleccionado->village<<endl;
		}
		else
		{
			cout<<"La aldea no existe"<<endl;
		}
						
	}
	else
	{
		cout << "No se encontró el guardian con el nombre especificado." << endl;
	}
	return seleccionado;
}

void Entrenar(Guardian* personaje, list<Guardian*>& debiles, list<Guardian*>& oponentes, int dado,list<Guardian*> guardianes)//funcion para poder iniciar el entrenamiento del guardian con los maestros y aprendices
{
	for (Guardian* guardianDebil : guardianes)
	{
		if (guardianDebil->powerLevel <= 100)
		{
			debiles.push_back(guardianDebil);
		}
	}

	// Ordenar la lista "debiles" en orden descendente según el poder
	debiles.sort(comparador);

	// Imprimir las estadísticas de los guardianes más débiles
	cout << "----------------------------------------------" << endl;
	cout << "A continuacion se te presenta una lista con los guardianes mas debiles" << endl;
	cout << "Lista de Guardianes a Enfrentar" << endl;
	for (Guardian* guardianDebil : debiles)
	{
		cout << "Nombre: " << guardianDebil->name << endl;
		cout << "Puntos de Poder: " << guardianDebil->powerLevel << endl;
		cout << "Aldea Actual: " << guardianDebil->village << endl;
		cout << endl;
	}

	cout << "ALMACENANDO LOS DEBILES EN LA LISTA DE OPONENTES" << endl;
	for (Guardian* guardianDebil : debiles)
	{
		if (guardianDebil->village == personaje->village && guardianDebil != personaje)
		{
			oponentes.push_back(guardianDebil);
		}
	}

	cout << "Para empezar el entrenamiento elige a un oponente de la lista de debiles" << endl;
	if (!oponentes.empty())
	{
		cout << "Elige a un oponente de la lista de debiles de tu respectiva aldea:" << endl;
		int index = 1;
		for (Guardian* oponente : oponentes)
		{
			cout << index << ". Nombre: " << oponente->name << ", Puntos de Poder: " << oponente->powerLevel << endl;
			index++;
		}
		int opcion;
		cin >> opcion;

		// Verificar si la opción elegida es válida
		if (opcion >= 1 && opcion <= oponentes.size())
		{
			// Obtener el oponente seleccionado
			auto it = oponentes.begin();
			advance(it, opcion - 1);
			Guardian* oponenteSeleccionado = *it;

			// Verificar si el oponente seleccionado es Stormheart
            if (oponenteSeleccionado->name == "Stormheart" && personaje->powerLevel<90)
            {
                cout << "No puedes enfrentarte a Stormheart aun Elige otro oponente." << endl;
                return;
            }
            //Condicion en caso de llegar a enfrentar a stormheart
			if(oponenteSeleccionado->name == "Stormheart" && personaje->powerLevel>=90) 
			{
				cout<<"Increible llegaste que hayas llegado hasta aqui..."<<endl;
				cout<<"Ahora comenzara el enfrentamiento final entre:"<<personaje->name<<endl;
				cout<<"y: "<<oponenteSeleccionado->name<<endl;

				cout<<"---------ESTADISTICAS FINALES DE AMBOS CONTENDIENTES----------"<<endl;
				cout<<"Nombre del Personaje: "<<personaje->name<<endl;
				cout<<"Poder Final de Personaje: "<<personaje->powerLevel<<endl;
				cout<<"Aldea actual del Personaje: "<<personaje->village<<endl;	

				cout<<"Nombre del Rival: "<<oponenteSeleccionado->name<<endl;
				cout<<"Poder del Rival: "<<oponenteSeleccionado->powerLevel<<endl;
				cout<<"Aldea actual del Rival: "<<oponenteSeleccionado->village<<endl;	

				cout<<"Y asi inicio una pelea increible en un reino lejano..."<<endl;
				cout<<"Nadie sabe lo que paso..."<<endl;
				cout<<"Nadie sabe quien gano..."<<endl;
				cout<<"La gente del reino huyo..."<<endl;
				cout<<"y la pelea se transformo en un recuerdo de la palabra..."<<endl;
				cout<<"Segun cuentan las leyendas, el reino quedo en ruinas y el rugir de los espiritus..."<<endl;	
				cout<<"de ambos combatientes aun se oye susurrando en el lejano..."<<endl;
				cout<<"frio y solitario viento de lo que alguna vez fue un reino...."<<endl;
				cout<<"FIN..."<<endl;
				return;
			}
			cout << "INFORMACION DEL OPONENTE" << endl;
			cout << "Nombre del oponente: " << oponenteSeleccionado->name << endl;
			cout << "Puntos de Poder: " << oponenteSeleccionado->powerLevel << endl;
			cout << "Mastro: " << oponenteSeleccionado->mainMaster << endl;
			cout << "Aldea a la que pertenece: " << oponenteSeleccionado->village << endl;

			// Verificar si el oponente es maestro o aprendiz
			if (oponenteSeleccionado->isMaster())
			{
				cout << "El oponente es un maestro :O" << endl;
				// Oponente es maestro
				dado = getRollResult();
				if (personaje->powerLevel > oponenteSeleccionado->powerLevel)
				{
					cout << "El oponente es un maestro, pero tu eres mas fuerte que el B)" << endl;
					if (dado == 1 || dado == 4)
					{
						personaje->powerLevel += 2;
						cout << "Has derrotado al oponente maestro Obtienes 2 puntos de poder" << endl;
					}
					else
					{
						cout << "No lograste derrotar al oponente maestro" << endl;
					}
				}
				if (personaje->powerLevel < oponenteSeleccionado->powerLevel)
				{
					cout << "El oponente es un maestro, pero tu eres mas debil que el ._." << endl;
					if (dado == 3)
					{
						personaje->powerLevel += 2;
						cout << "Has derrotado al oponente maestro Obtienes 2 puntos de poder" << endl;
					}
					else
					{
						cout << "No lograste derrotar al oponente maestro" << endl;
					}
				}
			}
			else
			{
				dado = getRollResult();
				cout << "El oponente es un aprendiz :_" << endl;
				// Oponente es aprendiz
				if (personaje->powerLevel > oponenteSeleccionado->powerLevel)
				{
					cout << "El oponente es un aprendiz, pero tu eres mas fuerte que el B)" << endl;
					if (dado == 1 || dado == 2 || dado == 3 || dado == 4)
					{
						personaje->powerLevel += 1;
						cout << "Has derrotado al oponente aprendiz Obtienes 1 punto de poder" << endl;
					}
					else
					{
						cout << "No lograste derrotar al oponente aprendiz" << endl;
					}
				}
				if (personaje->powerLevel < oponenteSeleccionado->powerLevel)
				{
					cout << "El oponente es un aprendiz, pero tu eres mas debil que el ._." << endl;
					if (dado % 2 != 0)
					{
						personaje->powerLevel += 1;
						cout << "Has derrotado al oponente aprendiz Obtienes 1 punto de poder" << endl;
					}
					else
					{
						cout << "No lograste derrotar al oponente aprendiz" << endl;
					}
				}
			}
			if (personaje->powerLevel >= 90)
			{
				cout<<"NOTA: "<<endl;
				cout << "Felicidades Has alcanzado los 90 puntos de poder" << endl;
				cout << "Ahora tienes la oportunidad de enfrentar a Stormheart" << endl;
			}
		}
		else
		{
			cout << "Opcion invalida." << endl;
		}
	}
	else
	{
		cout << "No hay oponentes disponibles en la misma aldea que tu personaje" << endl;
	}	
}

void EmpezarViaje(Guardian* seleccionado, Guardian* personaje,list<Guardian*> guardianes,list<Aldea*> aldeas,vector<Guardian*>aprendices, int sel,list<Guardian*>&debiles, list<Guardian*>&oponentes, int dado) //funcion para poder empezar el entrenamiento del guardian a viajando por el mundo
{
	bool PCreado = false; // Variable para indicar si hay un personaje creado
    bool PSeleccionado = false; // Variable para indicar si hay un personaje seleccionado
	GrafoAldea grafoaldea;
	if(!guardianes.empty() && !aldeas.empty())
	{
			if(aprendices.size() > 0) //Verificar si hay un personaje creado
			{
				PCreado = true;
				cout << "Nombre del Guardian: " << aprendices[0]->name<< endl;
				cout << "Poder del Guardian: " << aprendices[0]->powerLevel << endl;
				cout << "Aldea del Guardian: " << aprendices[0]->village << endl;
			}
			if(seleccionado != nullptr) //Verificar si hay un personaje seleccionado
			{
				PSeleccionado = true;
				cout<<"Informacion del Guaridan seleccionado:"<<endl;
				cout <<"Guardian seleccionado: " << seleccionado->name << endl;
				cout <<"Poder: "<<seleccionado->powerLevel<<endl;
				cout << "Aldea Perteneciente: " << seleccionado->village << endl;
			}

			if(PCreado || PSeleccionado)
			{
				do
				{
					debiles.clear(); 	// Reiniciar la lista de oponentes debiles en cada iteración del ciclo
					oponentes.clear(); // Reiniciar la lista de oponentes debiles en cada iteración del ciclo
					if(seleccionado!=nullptr)
					{
						cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE SELECCIONADO"<<endl;
						cout<<"Nombre: "<<seleccionado->name<<endl;
						cout<<"Puntos de Poder: "<<seleccionado->powerLevel<<endl;
						cout<<"Aldea Actual: "<<seleccionado->village<<endl;
					}
					if(personaje!=nullptr)
					{
						cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE CREADO"<<endl;
						cout<<"Nombre: "<<personaje->name<<endl;
						cout<<"Puntos de Poder: "<<personaje->powerLevel<<endl;
						cout<<"Aldea Actual: "<<personaje->village<<endl;
					}		
					cout << "Tienes un personaje creado o seleccionado" << endl;
					cout<<"Seleccione la opcion a partir de ahora"<<endl;
					cout<<"1: Viajar entre aldeas"<<endl;
					cout<<"2: Entrenar con el guardian de la aldea"<<endl;
					cout<<"3: Alquimia para viajar a cualquier punto"<<endl;
					cout<<"4: Salir del viaje"<<endl;
					cin>>sel;
					cin.ignore(); // Agregar esta línea para descartar el carácter de nueva línea en el búfer
					switch (sel)
					{
					case 1:
						{
							if(PCreado || PSeleccionado)
							{
												
								cout << "Aldeas y sus conexiones" << endl;
								GrafoAldea grafoaldea(aldeas);
								grafoaldea.imprimirGrafo();

								if (seleccionado != nullptr)
								{
									// Pedir al usuario el destino de viaje
									string destino;
									cout << "Elija hacia donde quiere viajar:" << endl;
									getline(cin,destino);
									// Verificar si la aldea de destino existe en el grafo
									if (grafoaldea.existeAldea(destino, aldeas))
									{
										// Viajar entre aldeas														
										grafoaldea.viajarEntreAldeas(seleccionado,seleccionado->village,destino,aldeas);
										cout<<"Estadisticas del personaje seleccionado:"<<endl;
										cout<<"Nombre: "<<seleccionado->name<<endl;
										cout<<"Puntos de Poder: "<<seleccionado->powerLevel<<endl;
										cout<<"Aldea Actual: "<<seleccionado->village<<endl;
									}
									else
									{
										cout << "La aldea de destino no existe, asegurese de haberla escrito bien" << endl;
									}
														
								}
								if (aprendices.size() > 0)
								{
									// Obtener el personaje creado o seleccionado
									personaje = aprendices[0];
									// Pedir al usuario el destino de viaje
									string destino;
									cout << "Elija hacia donde quiere viajar:" << endl;
									getline(cin,destino);
													
									// Verificar si la aldea de destino existe en el grafo
									if (grafoaldea.existeAldea(destino, aldeas))
									{
										// Viajar entre aldeas
										grafoaldea.viajarEntreAldeas(personaje,personaje->village,destino,aldeas);
										cout<<"Estadisticas del personaje creado:"<<endl;
										cout<<"Nombre: "<<personaje->name<<endl;														
										cout<<"Puntos de Poder: "<<personaje->powerLevel<<endl;
										cout<<"Aldea Actual: "<<personaje->village<<endl;
															
									}
									else
									{
										cout << "La aldea de destino no existe, asegurese de haberla escrito bien" << endl;
									}
														
								}
							}	
							else
							{
								cout << "No hay personaje creado ni seleccionado" << endl;
							}

						}
					break;
					case 2:
						{
							if(seleccionado!=nullptr)
							{
								cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE SELECCIONADO"<<endl;
								cout<<"Nombre: "<<seleccionado->name<<endl;
								cout<<"Puntos de Poder: "<<seleccionado->powerLevel<<endl;
								cout<<"Aldea Actual: "<<seleccionado->village<<endl;
								Entrenar(seleccionado,debiles,oponentes,dado,guardianes);
							}	
							if(personaje!=nullptr)
							{
								cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE CREADO"<<endl;
								cout<<"Nombre: "<<personaje->name<<endl;
								cout<<"Puntos de Poder: "<<personaje->powerLevel<<endl;
								cout<<"Aldea Actual: "<<personaje->village<<endl;

								Entrenar(personaje,debiles,oponentes,dado,guardianes);
							}
							
						}
					break;
					case 3:
						{
							if(seleccionado!=nullptr)
							{
								cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE SELECCIONADO"<<endl;
								cout<<"Nombre: "<<seleccionado->name<<endl;
								cout<<"Puntos de Poder: "<<seleccionado->powerLevel<<endl;
								cout<<"Aldea Actual: "<<seleccionado->village<<endl;

								cout<<"------Bienvenido a la funcion alquimista------"<<endl;
								cout<<"Con esta funcion seras capaz de viajar a cualquier aldea"<<endl;
								cout<<"Solo tienes que pagar 2 o 4 puntos de tus puntos de poder"<<endl;
								string aldeaDestino;
								cout<<"Elige a que aldea quieres ir inmediatamente: "<<endl;
								cout << "Ingresa la aldea de destino: "<<endl;
								getline(cin,aldeaDestino);
								grafoaldea.Alquimista(seleccionado, seleccionado->village, aldeaDestino);
							}
							if(personaje!=nullptr)
							{
								cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE CREADO"<<endl;
								cout<<"Nombre: "<<personaje->name<<endl;
								cout<<"Puntos de Poder: "<<personaje->powerLevel<<endl;
								cout<<"Aldea Actual: "<<personaje->village<<endl;

								cout<<"------Bienvenido a la funcion alquimista------"<<endl;
								cout<<"Con esta funcion seras capaz de viajar a cualquier aldea"<<endl;
								cout<<"Solo tienes que pagar 2 o 4 puntos de tus puntos de poder"<<endl;
								string aldeaDestino;
								cout<<"Elige a que aldea quieres ir inmediatamente: "<<endl;
								cout << "Ingresa la aldea de destino: "<<endl;
								getline(cin,aldeaDestino);
								grafoaldea.Alquimista(personaje,personaje->village, aldeaDestino);
							}		
						}
					break;
					case 4:
						{
							cout<<"Saliendo..."<<endl;
						}
					break;
				}
			
			}while(sel!=4);
													
		}
		else
		{
			cout << "No hay personaje creado ni seleccionado" << endl;
		}
					
	}
	else
	{
		cout<<"Tienes que cargar los datos de las aldeas y los guardianes antes de empezar"<<endl;
	}

}

void menuOptions()
{
		
	list<Guardian*> guardianes;     //lista para almacenar a los guardianes cargados desde el archivo	
	list<Aldea*> aldeas;           //lista para guardar las aldeas conectadas cargadas desde el archivo
	GuardiansTree guardTree;      //arbol para guardar la lista en el arbol
	vector<Guardian*>aprendices; //vector para almacenar al personaje creado;
	list<Guardian*> debiles;    //lista de todos los guardianes ordenados del mas fuerte al mas debil
	list<Guardian*> oponentes; //lista de los oponentes a enfrentar
	Guardian* root = nullptr; //puntero a la raiz del arbol
	Guardian* newGuard = new Guardian(); //instancia para seleccionar a un guardian
	Guardian* seleccionado = nullptr;   //instancia para guardar las variables del guardian seleccionado
	Guardian* personaje = nullptr;     //instancia para guardar al personaje creado
	
	int k;         //variable para seleccionar en el menu
	int dado=0;   //variable para almacenar el dado
	int sel=0;   //variable para seleccionar en el submenu de la funcion empezar viaje
 	while(k!=9) //menu
 	{
		cout << "----------------Menu de Opciones---------------------" <<endl;
    	cout << "Seleccione la opcion para continuar" <<endl;
		cout << "1: Seleccionar un personaje" <<endl;
		cout << "2: Crear un Personaje" <<endl;
		cout << "3: Cargar las aldeas del jugador" <<endl;
		cout << "4: Cargar Datos Externos de los Guardianes" <<endl;
		cout << "5: Empezar viaje" <<endl;
		cout << "6: Saber el resultado del lanzamiento de dados" <<endl;
		cout << "7: Conocer el historial del viaje" <<endl;
		cout << "8: Conocer el historial de combates" <<endl;
		cout << "9: Salir" <<endl;
		cin >> k;
		switch (k)
		{
			case 1:
			{
				if (!guardianes.empty()) 
				{
					seleccionado = seleccionarGuardian(guardianes,guardTree,newGuard,root,seleccionado); //funcion para seleccionar un guardian del arbol
				} 
				else 
				{
					cout << "No se han cargado los guardianes desde el archivo." << endl;
				}
			}
			break;
			case 2:
			{
				string nombre,Mainvillage;
				int poder;
				
				crearAprendiz(nombre,Mainvillage);
				poder = 50; //estableciendo el poder del creado a 50 puntos
				agregarAprendiz(aprendices,nombre,poder,Mainvillage);
				
				cout << "Nombre del Guardian: " << aprendices[0]->name<< endl;
				cout << "Poder del Guardian: " << aprendices[0]->powerLevel << endl;
				cout << "Aldea del Guardian: " << aprendices[0]->village << endl;   
			}	
			break;
			case 3:
			{
				string aldea = "regiones.txt";
				cargarAldeasExternas(aldeas,aldea);//cargar las aldeas y meterlas a una lista

				GrafoAldea grafoaldea(aldeas);//grafo para almacenar la lista de aldeas
				
				
				cout<<"----------------ALDEAS----------------"<<endl;// Imprimir las aldeas almacenadas en la lista
				for (const Aldea* aldea : aldeas) 
				{
					cout << "Aldea: " << aldea->nombre << ", Conectada con: " << aldea->conectadaCon <<endl;
				}
				
				cout<<"Grafo de las Aldeas:"<<endl;//imprimir el grafo de aldeas			
				grafoaldea.imprimirGrafo();
                
				grafoaldea.cargarGuardianes(guardianes,aldeas);//imprimir las aldeas junto a sus respectivos guardianes
				cout<<"-----------GUARDIANES POR ALDEA:----------"<<endl;
				grafoaldea.imprimirGuardianesporAldea();			
			}		
			break;
			case 4:
			{
				string filename = "guardianes.txt";
				cargarGuardianesExternos(guardianes, filename, guardTree); //cargar los guardianes y meterlos a una lista
			}
			
			break;
			case 5:
			{
				
				EmpezarViaje(seleccionado,personaje,guardianes,aldeas,aprendices,sel,debiles,oponentes,dado); //funcion para empezar el viaje
			}	
			break;
			case 6:
			{
				dado = getRollResult();
				cout << "Resultado del lanzamiento de dados: " << dado << endl;
			}	
			break;
			case 7:
			{
                //CODIGO PARA EL HISTORIAL DEL VIAJE INCOMPLETO
                /*
				// Obtener el recorrido del Guardian
				if(seleccionado!=nullptr)
				{	
					
					cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE SELECCIONADO"<<endl;
					cout<<"Nombre: "<<seleccionado->name<<endl;
					cout<<"Puntos de Poder"<<seleccionado->powerLevel<<endl;
					cout<<"Aldea Actual: "<<seleccionado->village<<endl;
					
					vector<string> recorrido = seleccionado->recorrido;
					// Imprimir el recorrido
					cout << "Recorrido del Guardian:" << endl;
					for (const string& aldea : recorrido) 
					{
						cout << aldea << " -> ";
					}
					cout << "FIN" << endl;
				}
				else
				{
					cout<<"no se ha empezado a viajar aun "<<endl;
				}	

				if(personaje!=nullptr)
				{
					
					cout<<"ESTADISTICAS ACTUALIZADAS DEL PERSONAJE CREADO"<<endl;
					cout<<"Nombre: "<<personaje->name<<endl;
					cout<<"Puntos de Poder"<<personaje->powerLevel<<endl;
					cout<<"Aldea Actual: "<<personaje->village<<endl;
					
					vector<string> recorrido = personaje->recorrido;
					// Imprimir el recorrido
					cout << "Recorrido del Guardian:" << endl;
					for (const string& aldea : recorrido) 
					{
						cout << aldea << " -> ";
					}
					cout << "FIN" << endl;
				}
				else
				{
					cout<<"no se ha empezado a viajar aun "<<endl;
				}	
				*/
			}	
			break;
			case 8:
			{
                //ESPACIO PARA PODER HACER EL HISTORIAL DE COMBATES, NO SE PUDO IMPLEMENTAR
			}	
			break;
			case 9:
			{
				cout<<"Saliendo"<<endl; //salir del programa
			}	
			break;
		}
		
	}
		
}

int main()
{
	
	menuOptions(); //llamando al menu de opciones
	
	return 0;
}