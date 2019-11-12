#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>

typedef struct{      //estuctura de conectados
	char nombre[30];
	int socket;
}Tconectado;

typedef struct{  //estructuda de lista de conectados
	int num;
	Tconectado conectados[50];
}TListaConectados;

TListaConectados misconectados; //lista de conectados
char conectados[50];
int contador=0;

//estructura necesaria para aceso excluyente
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int inserta(char nombre[30],int socket, TListaConectados*misconectados){//añadir un conectado a la lista
	int i=0;
	int encontrado=0;
	while ((i<(misconectados->num)&&(encontrado==0))){
		if(strcmp(misconectados->conectados[i].nombre,nombre)==0)
			encontrado=1;//el usuario esta en la lista
		else
			i=i+1;
	}
	if(encontrado==0){//si el jugador no esta en la lista
		if(misconectados->num >=100)//miramos si hay espacio libre para agregarlo
			return -1;
		else{
			strcpy(misconectados->conectados[misconectados->num].nombre, nombre);
			misconectados->conectados[misconectados->num].socket=socket;
			misconectados->num=misconectados->num+1; //añade jugador y suma uno al contador de conectados
			printf("Added user with socket %d\n", socket);
			return 0;
			
		}
	}
	else return -2;
}
	
	int elimina(char nombre[30],TListaConectados*misconectados){//el usuario que se desconecta hay que eliminarlo de la lista de conectados
		int i=0;
		int encontrado=0;
		while ((i < (misconectados->num)) && (encontrado == 0)) //primero busca al usuario en la lista y se guarda la posicion en i
		{
			if (strcmp(misconectados->conectados[i].nombre,nombre)==0)
				encontrado = 1; //usuario encontrado
			else
				i = i + 1;
		}
		while (i < (misconectados->num - 1)) //mueve todos los usuarios que estan por encima de la posicion i
		{
			strcpy(misconectados->conectados[i].nombre,misconectados->conectados[i+1].nombre);
			printf("%s",misconectados->conectados[i].nombre);
			misconectados->conectados[i].socket = misconectados->conectados[i+1].socket;
			i = i + 1;
		}
		
		if(encontrado == 1) //elimina los datos de la ultima posicion de la lista de conectados
		{
			strcpy ( misconectados->conectados[misconectados->num -1].nombre,"");
			misconectados->conectados[misconectados->num -1].socket = 0;
			return 0;
			
		}
		else
		   return -1; //si no se ha encontrado al usuario a eliminar de la lista
	}
	
	void dameconectados(char conectados[50], TListaConectados*misconectados){//crea un vector con los nombres de todos los conectados separados por ,
		
		sprintf(conectados, "%d", misconectados->num); //primero agregamos la cantidad de conectados 
		int i;
		for (i=0;i<misconectados->num;i++) //bucle que va agregando los conectados al string conectados
		{
			printf("%s",misconectados->conectados[i].nombre);
			sprintf(conectados,"%s,%s,%d",conectados,misconectados->conectados[i].nombre,misconectados->conectados[i].socket);
	
		}
		
	}
	
    int damesocket (char nombre[30], TListaConectados*misconectados){
		//retorna socket 0 si okey -1 error
		int encontrado=0;
		int i=0;
		while((i<misconectados->num)&&(encontrado == 0)){
			if((strcmp(misconectados->conectados[i].nombre,nombre)==0))
				encontrado=1;
		else 
			i=i+1;
		}
		if(encontrado){
			printf("%d",misconectados->conectados[i].socket);
			return misconectados->conectados[i].socket;
		   
		}
		else
			return -1;
	}


void * procesar(void *socket){
	
	//creamos socket y iniciamos conexion
	int sock_conn;
	int *s;
	s=(int *) socket;
	sock_conn=*s;
	printf("Valor de socket iniciado %d\n", sock_conn);
	int ret;
	char entrada[512];
	char salida[512];
	int sockets[100];
	MYSQL*conn;
	int err;
	int i;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[500]; //aqui guardaremos la respuesta de la consulta
	
	conn=mysql_init(NULL);
	//conectamos con la base de datos
	if(conn==NULL){
		printf("Error al crear la conexion:%u %s\n",mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "GameAdria",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	int acabar=0;
	while (acabar==0){
		//recibimos entrada
		ret=read(sock_conn,entrada,sizeof(entrada));
		printf("¡Recibido!\n");
		//añadimos la marca de final de string para que no escriba lo que hay despues del buffer
		entrada[ret]='\0';
		printf ("Opcion: %s\n", entrada);
		//miramos que opcion es
		char *p=strtok(entrada, "/");
		int opcion=atoi(p);
		printf("%d \n", opcion);
		//p=strtok(NULL,"/");
		//strcpy(consulta, p);
		
		//aqui ya tenemos el codigo de peticion despues de separar el numero
		//sscanf(entrada,"%d/%s", &opcion, consulta);
		if(opcion==1){//OPCION 1 = AÑADIR UN JUGADOR
			p=strtok(NULL,"/");
			strcpy(consulta, p);
			char Alias[20];
			char Password[20];
			strcpy (Alias, p);
			printf("%s\n", Alias);
			p=strtok(NULL, "/");
			strcpy (Password, p);
			printf("%s\n", Password);
			
			int num;
			num=atoi(salida); 
			char agregar[80];
			if(num==0){
				strcpy(agregar, "INSERT INTO Jugador (Alias, Password, Experiencia, Pais, Puntos) VALUES ('");
				strcat(agregar, Alias);
				strcat(agregar, "',");
				strcat(agregar, Password);
				strcat(agregar, ",0,0);");
				err=mysql_query(conn, agregar);
				if (err!=0){
					printf ("\n Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					sprintf (salida,"1\n");
					exit (1);
				}
				printf("%s", agregar);
				sprintf(salida,"0\n");
				int num;
				num=atoi(salida);
				if(num==1){
					printf("\n Datos introducidos incorrectos \n");
				}
				else{
					printf("\n Usuario creado con exito \n");
					sprintf (salida,"1/%s", Alias);
				}
			}
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
		}
		
		if(opcion==2){//OPCION 2 = INICIAR SESION
			p=strtok(NULL,"/");
			strcpy(consulta, p);
			char usuario[20];
			char password[20];
			strcpy(usuario,p);
			printf("%s \n", usuario);
			p=strtok(NULL, "/");
			strcpy (password, p);
			printf("%s \n", password);
			strcpy(consulta, "SELECT COUNT(Alias) FROM Jugador WHERE Jugador.Alias='");
			strcat(consulta, usuario);
			strcat(consulta, "' AND Jugador.Password= '");
			strcat(consulta, password);
			strcat(consulta, "';");
			err=mysql_query (conn, consulta); 
			
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			resultado = mysql_store_result (conn);
			row = mysql_fetch_row (resultado);
			
			if (row == NULL)
				sprintf (salida,"No se han obtenido datos en la consulta\n");
			else{
				while (row !=NULL) {
					sprintf (salida,"%s \n", row[0]);
					// obtenemos la siguiente fila
					row = mysql_fetch_row (resultado);
				}
				int num;
				num=atoi(salida);
				if(num==1){
					sprintf(salida,"2");
					printf("User logueado: %s, socket: %d\n", usuario, sock_conn); 
					inserta(usuario,sock_conn,&misconectados);
					//sprintf (salida,"2/%s", usuario);
				}
				else
				   printf("Datos introducidos erroneos \n");
				
			}
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
			
		}
		
		if (opcion==3){//OPCION 3 = Numero de veces que tengo exclusion
			sprintf(salida,"3/%d",contador);  
		}
		
		if (opcion==4){ //OPCION 4 = DAME JUGADOR Y TE DARE LA EXPERIENCIA
			// Pregunto los nombres de los dos jugadores
			p=strtok(NULL,"/");
			strcpy(consulta, p);
			char jugador[30];
			strcpy(jugador, consulta);
			char query [512];
			sprintf(query, "SELECT Jugador.Experiencia FROM Jugador WHERE Jugador.Alias = '%s'", jugador);
			err=mysql_query (conn, query);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			//recogemos el resultado de la consulta. El resultado de la
			//consulta se devuelve en una variable del tipo puntero a
			//MYSQL_RES tal y como hemos declarado anteriormente.
			//Se trata de una tabla virtual en memoria que es la copia
			//de la tabla real en disco.
			resultado = mysql_store_result (conn);
			// El resultado es una estructura matricial en memoria
			// en la que cada fila contiene los datos de una persona.
			
			// Ahora obtenemos la primera fila que se almacena en una
			// variable de tipo MYSQL_ROW
			row = mysql_fetch_row (resultado);
			if (row == NULL)
				sprintf (salida,"No se han obtenido datos en la consulta");
			else{
				int exp = atoi(row[0]);
				sprintf (salida,"%d\n", exp);
			}
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
			
		}
		
		
		if (opcion==5){//OPCION 5 = DAME JUGADOR Y TE DARÉ SUS PUNTOS
			p=strtok(NULL,"/");
			strcpy(consulta, p);
			printf("Opcion: %d\n",opcion);
			char jug[30];
			strcpy(jug, consulta);
			char cola[512];
			sprintf(cola, "SELECT JugadorPartida.Puntos FROM JugadorPartida WHERE JugadorPartida.AliasJugador = '%s'", jug);
			err=mysql_query (conn, cola);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			//recogemos el resultado de la consulta. El resultado de la
			//consulta se devuelve en una variable del tipo puntero a
			//MYSQL_RES tal y como hemos declarado anteriormente.
			//Se trata de una tabla virtual en memoria que es la copia
			//de la tabla real en disco.
			resultado = mysql_store_result (conn);
			// El resultado es una estructura matricial en memoria
			// en la que cada fila contiene los datos de una persona.
			
			// Ahora obtenemos la primera fila que se almacena en una
			// variable de tipo MYSQL_ROW
			row = mysql_fetch_row (resultado);
			char consultaresultante[512];
			strcpy(consultaresultante,"");
			if (row == NULL)
				sprintf (consultaresultante,"No se han obtenido datos en la consulta\n");
			else
			{
				char textoconsulta [600];
				strcpy (textoconsulta,"");
				while (row !=NULL) {
					sprintf (textoconsulta,"%s\n", row[0]);
					strcat(consultaresultante, textoconsulta);
					row = mysql_fetch_row (resultado);
				}
			}
			strcpy(salida,consultaresultante);
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
		}
		
		
		if (opcion==6){//OPCION 6 = DAME PAIS Y TE DARA EL JUGADOR
			p=strtok(NULL,"/");
			strcpy(consulta, p);
			char pais[500];
			strcpy(pais,consulta);
			char cola[512];
			sprintf(cola, "SELECT Jugador.Alias FROM Jugador WHERE Jugador.Pais = '%s'", pais);
			err=mysql_query (conn, cola);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			//recogemos el resultado de la consulta. El resultado de la
			//consulta se devuelve en una variable del tipo puntero a
			//MYSQL_RES tal y como hemos declarado anteriormente.
			//Se trata de una tabla virtual en memoria que es la copia
			//de la tabla real en disco.
			resultado = mysql_store_result (conn);
			// El resultado es una estructura matricial en memoria
			// en la que cada fila contiene los datos de una persona.
			
			// Ahora obtenemos la primera fila que se almacena en una
			// variable de tipo MYSQL_ROW
			row = mysql_fetch_row (resultado);
			char consultaresultante[512];
			strcpy(consultaresultante,"");
			if (row == NULL)
				sprintf (consultaresultante,"No se han obtenido datos en la consulta\n");
			else
			{
				char textoconsulta [600];
				strcpy (textoconsulta,"");
				while (row !=NULL) {
					sprintf (textoconsulta,"%s\n", row[0]);
					strcat(consultaresultante, textoconsulta);
					row = mysql_fetch_row (resultado);
				}
			}
			strcpy(salida,consultaresultante);
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
			
		}
		
		
		if(opcion==7){//OPCION 7: Lista de conectados
			char res[512];
			dameconectados(res,&misconectados);
			printf("Lista de conectados enviada: %s\n", res);
			strcpy(salida,res);
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
		}
		
		if((opcion==8)||(opcion==0)){
			char notificacion[512];
			if(opcion==8){//si se inicia sesion correctamente
				notificacion[0]='\0';
				if((misconectados.num)==0){
					//si no es el primero en conectarse la base de datos estara abierta
					conn=mysql_init(NULL);
					if(conn==NULL){
						printf("Error en crear la conexion %u %s\n",
							   mysql_errno(conn),mysql_error(conn));
						exit(1);
					}
					conn=mysql_real_connect (conn,"localhost","root","mysql","Game",0,NULL,0);
					if(conn==NULL){
						printf("Error en crear la conexion %u %s\n",
							   mysql_errno(conn),mysql_error(conn));
						exit(1);
					}
					
			     	//si es el primer conectado tambien rellena la tabla
					int m;
					//for((m=0);m<100;m++){
					//mitabla[m].socket1=-1;
					//}
				}
				inserta(entrada,sock_conn,&misconectados);
			}
			
			if(opcion==0){//OPCION 0 = DESCONECTAR
				acabar=1;
				notificacion[0]='\0';
				p=strtok(NULL,"/");
				strcpy(consulta, p);
				elimina(consulta, &misconectados);
				if((misconectados.num)==0){//cerramos base de datos cuando no haya ningun conectado
					mysql_close(conn);
				}
				
			}
			
			//notificamos a todos los usuarios la nueva lista de conectados
			char resultado[512];
			dameconectados(resultado, &misconectados);
			sprintf(notificacion,"8/%s",resultado);
			printf("%s\n",notificacion);
			int j;
			for(j=0;j<misconectados.num;j++)
				write(misconectados.conectados[j].socket,notificacion,strlen(notificacion));
			sprintf(salida, "%s", notificacion);
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
			
		}
		
		if(opcion==9){
			p=strtok(NULL,"/");
			strcpy(consulta, p);
			printf("El valor de nombre es: %s\n", consulta);
			damesocket(consulta,&misconectados);
			//Enviamos la respuesta
			write (sock_conn,salida, strlen(salida));
			//si se selecciona algo se envia su correspondiente respuesta
			printf ("Respuesta: %s\n", salida);
		}
		
		
		if ((opcion==1)||(opcion==4)||(opcion==5)||(opcion==6)||(opcion==7)){
			pthread_mutex_lock(&mutex); //no interrumpas ahora, bloqueamos el thread
			contador=contador+1;
			pthread_mutex_unlock(&mutex); //ya puedes interrumpirme, una vez ya haya acabado la funcion el thread se desbloquea para que pueda seguir el funcionamiento de threads
		}
		
		
		
	}
	//se acaba el servicio para este cliente
		close(sock_conn);
	    
}


int main(int argc, char **argv)
{
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	pthread_t thread[50];
	int sockets[10];
	misconectados.num =0;
	//INICIALIZAMOS
	//Abrimos socket
	if((sock_listen=socket(AF_INET, SOCK_STREAM,0))<0)
		printf("Error creando el socket\n");
	//Hacemos bind en el puerto
	memset(&serv_adr,0,sizeof(serv_adr)); //inicializamos a 0 serv_adr
	serv_adr.sin_family=AF_INET;
	//asocia el socket a las Ip de la maquina
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9070);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error en el bind\n");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error al escuchar\n");
	
	int m;
	m=0;
	//bucle infinito
	for(;;){
		printf("Escuchando...\n");
		sock_conn=accept(sock_listen,NULL,NULL);
		printf("Hay conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		sockets[m]=sock_conn;
		//pthread_create( &thread[i], NULL, procesar, sockets[i]);		
		
		pthread_create( &thread[m], NULL, procesar, &sockets[m]);
		printf("m: %d, sockets[m]: %d\n",m,sockets[m]);
		m++;
		
	}

}
	

	
	
	
	
	

