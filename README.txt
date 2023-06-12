la problematica incial era sobre tener un mundo con aldeas y guardianes.
cada guardian pertenece a una aldea y tiene nombre, maestro y puntos de poder.
por lo cual en este escenario se plantea una jerarquia dentro de un mundo con aldeas
conectadas entre si.

uno como usuario puede crear o seleccionar a un personaje para poder empezar la travesia
por el mundo con el objetivo de entrenar con los maestros y aprendices de las aldeas
para poder llegar a la ciudad principal de tesla y enfrentar al maestro de todos: stormheart
reuniendo puntos en el camino

las problematicas planteadas fueron:
1.tener los datos de las aldeas y guardianes.
2. jerarquizar los guardianes.
3. tener un sistema de creacion o seleccion de personaje
4. poder viajar por el mundo.
5. poder entrenar con los guardianes de sus aldeas y obtener puntaje.
6. almacenar el recorrido del guardian y sus entrenamientos.
7.conectar las aldeas
8.tener una lista de rivales para poder enfrentar al mas debil
9.poder viajar a cualquier aldea en caso de que no haya conexion directa entre ellas
10.tener un sistema de combate simple contra los guardianes.
11.almacenar al guardian
12. almacenar las aldeas
13. cumplir el objetivo y llegar a stormheart
para la resolucion de la problematica se uso:

1.para este punto se cargan los datos externamente a traves de un txt con la informacion de las aldeas conectadas y los datos de los guardianes.

2.para jerarquizar a los guardianes se uso un arbol general para ellos, donde se cargan y se ordenan segun jerarquias usando vectores, de esta forma almacenando a un maestro con varios aprendices, ademas de tener la informacion del contenido de las aldeas(los que viven ahi).

3.para crear al personaje se usa una funcion que sirve de menu para crearlo y otra que sirve para almacenarlo en una instancia de guardian y meterlo a un vector de aprendices.
para seleccionar al guardian se selecciona directamente del arbol, modificando la jerarquia para poder almacenar al seleccionado en otra instancia, mientras que a la instancia original se le modifica el nombre tanto en la lista original como en el arbol y el mapa de aldeas con guardianes del arbol,para que al luchar contra guardianes uno no se enfrente a si mismo(NOTA: lamentablemente a pesar de que funciona lo anterior dicho, uno no se puede enfrentar al guardian modificado, pues no se podra por razones desconocidas).

4.para viajar por el mundo se utilizo una clase grafoaldea que carga los datos externos de las conexiones y los almacena en un grafo que se encarga de conextarlas, ademas en dicha clase se almacena la informacion de las aldeas y los guardianes que tiene ahi. 
 
5.para esto se hizo una funcion que toma o al seleccionado o creado y segun probabilidades de un dado y usando condicionales uno puede ganar puntos para el guadian, tambien se tiene en cuenta que en esa funcion uno se enfrenta a los guardianes que estan en la aldea del personaje seleccionado o creado.

6.por temas de tiempo no se logro hacer.

7.para conectar las aldeas se uso un grafo en el cual a traves de una lista de adyacencia se conectaron las aldeas segun lo señalado en el archivo externo.

8.se uso una lista que almacena a los guardianes mas debiles y despues los carga en otra lista de oponentes.

9.para esto se implemento una funcion de alquimista que permite agragar una conexion entre la aldea del guardian con la del destino y viajar a cambio de puntos.

10.se uso un dado y dependiendo se si el guardian a enfrentar tenia mas o menos poder o si era o no maestro las probabilidades de ganar varian.

11. para esto se utilizo una estructura llamada guardian, donde se almacenan los atriutos de este que 
posteriormente se llamaran en otros metodos.

12. para esto se utilizo una estructura llamada aldea, donde se almacenan los atributos de esta, para posteriormente se usados en otros metodos.

13. en la misma funcion que se utiliza para entrenar hay una condicion, y es que si uno llega a los 90 puntos, si anteriormente se viajo a tesla uno puede enfrentarse a stormheart y terminar el juego.

NOTAS IMPORTANTES A TENER EN CUENTA:
-PRIMERO CARGUE LOS GUARDIANES Y DESPUES LAS ALDEAS, PUES PARA QUE EL MAPA DE ALDEAS DEL GRAFO DE ALDEAS FUNCIONE DEBE TENER LA INFORMACION DE LOS GUARDIANES PARA ALMACENARLOS.
-POR ALGUN MOTIVO UNO NO PUEDE VIAJAR A TESLA PERO SI PUEDE VIAJAR DESDE TESLA, POR ENDE SI UNO QUIERE VIAJAR DESDE OTRO LUGAR A TESLA DEBE USAR LA FUNCION DE ALQUIMISTA.
-SI UNO CREA AL PERSONAJE, POR TEMAS DE CODIGO UNO TIENE QUE PRIMERO VIAJAR A OTRA ALDEA PARA QUE LAS VARIABLES DEL CREADO SE GUARDEN EN UN INSTANCIA NUEVA DENTRO DEL CODIGO, TRAS VIAJAR A OTRA ALDEA, AL JUGADOR SELECCIONADO SE LE PERMITE ENTRENAR O USAR LA FUNCION DE ALQUIMISTA.
