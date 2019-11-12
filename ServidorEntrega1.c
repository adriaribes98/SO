#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
int main(int argc, char **argv)
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char entrada[512];
	char salida[512];
	
	//abrir socket
	if ((sock_listen = socket (AF_INET, SOCK_STREAM, 0))<0)
		printf("Error creando socket");
	
	//bind en el puerto
	memset(&serv_adr,0,sizeof(serv_adr)); //inicia a 0 el serv_addr
	serv_adr.sin_family=AF_INET;
	
	//asociacion socket a una de las IP de la maquina
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	//escuchamos puerto
	serv_adr.sin_port=htons(9070);
	if(bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr))<0)
		printf("Error en el bind\n");
	
	//cola de peticiones no superior a 5
	if(listen(sock_listen,4)<0)
		printf("Error en el listen\n");
	
	while (0==0)
	{	printf("Escuchando...\n");
	    sock_conn=accept(sock_listen, NULL, NULL);
		printf("Conexion exitosa\n");
		
		//sock_conn--socket para cliente
		//recibimos nombre y guardamos en entrada
		ret=read(sock_conn,entrada,sizeof(entrada));
		printf("Recibido\n");
		
		//evitar escribir lo que hay despues
		entrada[ret]='\0';
		
		//mostramos nombre en consola
		printf("Mensaje de peticion: %s\n",entrada);
	
	//Ahora conectamos con la base de datos	
		MYSQL *conn;
		int err;
	
	//Creamos una conexion al servidor MYSQL 
		conn = mysql_init(NULL);
		if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), 
				mysql_error(conn));
				exit (1);
		}
	//inicializar la conexin
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "GameAdria",0, NULL, 0); //Game nombre de la bbd
		if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
		}
    
	int opcion;
	char vector[500];
	sscanf(entrada,"%d/%s", &opcion, vector);
	printf("Opcion: %d\n", opcion);
	//OPCION 1:Introducimos Alias para saber su experiencia.
	//OPCION 2:Introducimos Alias y te da su contraseña.
	//OPCION 3:Introducimos el pais para saber el jugador.
	if (opcion==1)
	{
		MYSQL_RES *resultado;
		MYSQL_ROW row;
	// Pregunto los nombres de los dos jugadores
	char jugador[30];
	strcpy(jugador, vector);
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
	int exp = atoi(row[0]);
	if (row == NULL)
		sprintf (salida,"No se han obtenido datos en la consulta");
	else
		sprintf (salida,"La persona %s tiene una experiencia de: %d\n", jugador, exp);
		
	    mysql_close (conn);
	}
	
	
	
	if (opcion==2)
	{
		MYSQL_RES *resultado1;
		MYSQL_ROW row;
		char Alias[30];
		char consulta1 [600];
		printf ("Dame el Nombre de la persona que quieres buscar\n"); 
		scanf ("%s", Alias);
		// construimos la consulta SQL
		sprintf (consulta1,"SELECT JugadorPartida.Puntos FROM JugadorPartida WHERE JugadorPartida.AliasJugador = '%s';", Alias);
		
		// hacemos la consulta 
		err=mysql_query (conn, consulta1); 
		if (err!=0) {
			printf ("Error al consultar datos de la base %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//recogemos el resultado de la consulta 
		resultado1 = mysql_store_result (conn); 
		row = mysql_fetch_row (resultado1);
		if (row == NULL)
			printf ("No se han obtenido datos en la consulta\n");
		else
			// El resultado debe ser una matriz con una sola fila
			// y una columna que contiene el autor
			printf ("Puntos del Jugador: %s\n", row[0] );
		// cerrar la conexion con el servidor MYSQL 
		mysql_close (conn);
		exit(0);
	}
	
	if (opcion==3)
	{
		
		MYSQL_RES *resultado;
		MYSQL_ROW row;
		char pais[500];
		strcpy(pais,vector);
		char query [512];
		sprintf(query, "SELECT Jugador.Alias FROM Jugador WHERE Jugador.Pais = '%s'", pais);
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
		char consultaresultante[512];
		strcpy(consultaresultante,"");
		if (row == NULL)
			sprintf (consultaresultante,"No se han obtenido datos en la consulta\n");
		else
			{
			char textoconsulta [600];
			strcpy (textoconsulta,"");
			while (row !=NULL) {
				sprintf (textoconsulta,"La persona %s es del Pais %s.\n", row[0], pais);
				strcat(consultaresultante, textoconsulta);
				row = mysql_fetch_row (resultado);
			}
		}
			strcpy(salida,consultaresultante);
			mysql_close (conn);
	}
	write (sock_conn,salida,strlen(salida));
	close(sock_conn);

	}
		return(0);
}
