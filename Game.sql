
DROP DATABASE IF EXISTS GameAdria;
CREATE DATABASE GameAdria;

USE GameAdria;


CREATE TABLE Jugador (
Alias VARCHAR(30) PRIMARY KEY NOT NULL,
Experiencia INTEGER,
Password VARCHAR(30) NOT NULL,
Pais VARCHAR(30) 
)ENGINE = InnoDB;


CREATE TABLE Partida (
Id INTEGER PRIMARY KEY NOT NULL,
Fecha VARCHAR(30) NOT NULL,
Hora_Fin VARCHAR(30) NOT NULL,
Duracion_Segundos_Partida INTEGER NOT NULL,
Ganador VARCHAR(30) NOT NULL,
FOREIGN KEY (Ganador) REFERENCES Jugador(Alias)
)ENGINE = InnoDB;


CREATE TABLE JugadorPartida (
AliasJugador VARCHAR(30) NOT NULL,
IdPartida INTEGER NOT NULL,
Puntos INTEGER NOT NULL,
FOREIGN KEY (AliasJugador) REFERENCES Jugador(Alias),
FOREIGN KEY (IdPartida) REFERENCES Partida(Id)
)ENGINE = InnoDB;      


INSERT INTO Jugador VALUES('Gerard',0, '1234','Alemania');
INSERT INTO Jugador VALUES('Adria',1,'4321','Francia');
INSERT INTO Jugador VALUES('Antonia',5,'0000','Italia');
INSERT INTO Partida VALUES('1','19/02/2005','19:00',12,'Gerard');
INSERT INTO Partida VALUES('2','20/02/2005','12:00',34,'Adria');
INSERT INTO Partida VALUES('3','22/02/2005','13:00',44,'Antonia');
INSERT INTO JugadorPartida VALUES ('Gerard',1,100);
INSERT INTO JugadorPartida VALUES ('Adria',2,8030);
INSERT INTO JugadorPartida VALUES ('Antonia',3,0);
