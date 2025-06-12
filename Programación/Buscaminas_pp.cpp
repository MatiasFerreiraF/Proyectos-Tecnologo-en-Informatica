#include <iostream>
#include <string>
#include <time.h>
#define CANT_BOMBAS 4
#define MAX_JUGADORES 2
#define MAX_PARTIDAS_TOTALES 50
using namespace std;

//Definir estructuras:
struct Fecha{
    unsigned dia;
    unsigned mes;
    unsigned anio;
    unsigned hora;
    unsigned minuto;
};
struct Datos_del_jugador{
    string ci;
    Fecha fecha_nacimiento;
    string nombre;
    string apellido;
    string nickName;
    string contrasenia;
    int cant_partidas_ganadas = 0;
};
struct Partidas{
    string nickName;
    Fecha fecha_partida;
    string resultado_partida;
};

Datos_del_jugador arreglo_jugadores[MAX_JUGADORES];
Datos_del_jugador dados_de_baja[MAX_JUGADORES];
Partidas historial_partidas[MAX_PARTIDAS_TOTALES];
int cant_jugadores = 0;
int cant_partidas = 0;

void imprimir_matriz(char matriz[8][8]);
void imprimir_portada();
void jugada(char arr[3]);
void calcular_numeros_adyacentes (char tablero[8][8], char adyacentes[8][8]);
void explorar (char arr[3], char tablero[8][8], char adyacentes[8][8], char juego[8][8], int & cont_exploradas);
void marcar(char arr[3], char juego[8][8],  int & casillas_marcadas);
int menu();
int menu_jugador();
int menu_consultas();
void opcion_menu();
void pre_juego();
void jugar(string nick, Fecha f);
bool ci_valida(string cedula);
bool fecha_valida(Datos_del_jugador arr[], int i);

int main(){
    opcion_menu();
}

void pre_juego(){
    string nick, clave;
    Fecha f;
    cout <<"Ingrese nickName: ";
    cin >> nick;
    cout <<"Ingrese contraseña: ";
    cin >> clave;
    bool encontro_jugador = false;
    //Buscar si ya existe el jugador
    for(int i = 0; i < cant_jugadores; i++){
        if(arreglo_jugadores[i].nickName == nick){
            encontro_jugador = true;
            if(arreglo_jugadores[i].contrasenia!= clave){
                cout << "Contraseña incorrecta, vuelva a ingresar" <<endl;
                cout << "NickName";
                cin >> nick;
                cout <<"Contraseña: ";
                cin >> clave;
                i--;
            }
            else{
                cout <<"Ingrese fecha del dia " <<endl;
                cout <<"Dia: ";
                cin >> f.dia;
                historial_partidas[cant_partidas].fecha_partida. dia = f.dia;
                cout <<"Hora: ";
                cin >> f.hora;
                historial_partidas[cant_partidas].fecha_partida.hora = f.hora;
                cout <<"Minuto: ";
                cin >> f.minuto;
                historial_partidas[cant_partidas].fecha_partida.minuto = f.minuto;
                historial_partidas[cant_partidas].nickName = nick;
                break;
            }
        }
    }
    if(!encontro_jugador){
        cout <<"NickName incorrecto. No se encuentra el jugador "<< nick <<endl;
        return;
    }
    jugar(nick, f);
}
void jugar(string nick, Fecha f){
    //Crear tablero
    int cont_exploradas = 0;
    char tablero[8][8];
    char adyacentes[8][8];
    char juego[8][8];
    int casillas_marcadas = 0;

    for  (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++){
            tablero[i][j] = '#';
            juego[i][j] = '#';
        }

    imprimir_portada();
    imprimir_matriz(juego);
    //arrglo de jugadas
    char arr [3] ={'0', '0', '0'};
    jugada(arr);
    if(arr[0] == 'R'){
        historial_partidas[cant_partidas].resultado_partida = "ABANDONO";
        cant_partidas++;
        return;
    }
    //Generar minas
    srand(time(0));
    int minas = 0;
    while(minas < CANT_BOMBAS){
        int i = rand() % 8;
        int j = rand() % 8;
        
        if (tablero[i][j] != '*' && (i != arr[1] - '0' || j != arr[2] - '0')){
            tablero[i][j] = '*';
            minas++;
        }
    }
    system("clear");
    calcular_numeros_adyacentes (tablero, adyacentes);

    if(arr[0] == 'E'){
        explorar(arr, tablero, adyacentes, juego, cont_exploradas);
        imprimir_portada();
        imprimir_matriz(juego);
    }
    if(arr[0] == 'M'){
        marcar(arr, juego, casillas_marcadas);
        imprimir_portada();
        imprimir_matriz(juego);
    }
    //Partida:
    bool partida_terminada = false;
    while(!partida_terminada){
        jugada(arr);
        int i = arr[1] - '0';
        int j = arr[2] - '0';
        if(arr[0] == 'R'){
            historial_partidas[cant_partidas].resultado_partida = "ABANDONO";
            cant_partidas++;
            return;
        }
        //Perder
        if(arr[0] == 'E' && tablero [i][j] == '*' && juego[i][j] != '>'){
            system ("clear");
            for(int i = cant_partidas; i < MAX_PARTIDAS_TOTALES; i++)
                if(historial_partidas[i].fecha_partida.minuto == f.minuto)
                    historial_partidas[i].resultado_partida = "PERDIO";            
            cout <<
            "  ____                         ___                  " << endl <<
            " / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __  " << endl <<
            "| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__| " << endl <<
            "| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |    " << endl <<
            " \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|    " << endl<<endl<<endl;

            for(int x = 0; x < 8; x++)
                for (int y = 0; y < 8; y++)
                    if(tablero [x][y] == '*')
                        juego [x][y] = '*';
            cout <<endl;
            juego [i][j] = 'X';
            imprimir_matriz(juego);
            partida_terminada = true;
            cant_partidas++;
            return;
        }
        if(arr[0] == 'M' && !partida_terminada)
            marcar(arr, juego, casillas_marcadas);
        if(arr[0] == 'E' && !partida_terminada)
            explorar(arr, tablero, adyacentes, juego, cont_exploradas);
        imprimir_portada();
        imprimir_matriz(juego);
        //Ganar
        if (cont_exploradas >= 64 - CANT_BOMBAS){
            partida_terminada = true;
            for(int i = 0; i < MAX_JUGADORES; i++)
                if(arreglo_jugadores[i].nickName == nick)
                    arreglo_jugadores[i].cant_partidas_ganadas ++;
            for(int i = cant_partidas; i < MAX_PARTIDAS_TOTALES; i++)
                if(historial_partidas[i].fecha_partida.minuto == f.minuto)
                    historial_partidas[i].resultado_partida = "GANO";
            cout << "  ___  ___  _  _  ___  ___  _____  ___"<<endl<<
                    " / __|/   \\| \\| |/   \\/ __||_   _|| __|"<<endl<<
                    "| (_ || - || .  || - |\\__ \\  | |  | _| "<<endl<<
                    " \\___||_|_||_|\\_||_|_||___/  |_|  |___|"<<endl<<endl;
            cant_partidas ++;
        }
    } 
}
int menu_principal(){
    int opcion;
    cout <<"Ingrese una opcion" <<endl;
    cout <<"1- Jugador" <<endl;
    cout <<"2- consulta" <<endl;
    cout <<"3- Jugar" <<endl;
    cout <<"0- Salir" <<endl<<endl;
    cin >> opcion;
    cout <<endl;
    return opcion;
}
int menu_jugador(){
    int opcion;
    cout <<"Ingrese una opcion" <<endl;
    cout <<"1- Alta de jugador" <<endl;
    cout <<"2- Baja de jugador" <<endl;
    cout <<"3- Modificacion de jugador" <<endl;
    cout <<"0- Volver al menu principal" <<endl<<endl;
    cin >> opcion;
    cout <<endl;
    return opcion;
}
int menu_consultas(){
    int opcion;
    cout <<"Ingrese una opcion" <<endl;
    cout <<"1- Listado de jugadores" <<endl;
    cout <<"2- Listado de todas las partidas" <<endl;
    cout <<"3- Listado de partidas por jugador" <<endl;
    cout <<"4- Listado de partidas por fecha" <<endl;
    cout <<"0- Volver al menu principal" <<endl<<endl;
    cin >> opcion;
    cout <<endl;
    return opcion;
}
void Alta_de_jugador(){
    if(cant_jugadores < MAX_JUGADORES){
        //Nickname
        cout <<"Ingrese nickName: ";
        string nick;
        cin >> nick;
        for(int i = 0; i < cant_jugadores; i++){
            if(arreglo_jugadores[i].nickName == nick){
                cout <<"Este nickName ya está en uso, vuelva a darse de alta" <<endl;
                return;
            }
        }
        for (int i = 0; i < MAX_JUGADORES; i ++)
            if(dados_de_baja[i].nickName == nick){
                cout <<"Jugador registrado previamente" <<endl;
                cout <<"Desea restaurar los datos?, si (s)? no (n)?";
                char c;
                cin >> c;
                if(c == 's'){
                    cout <<"ingrese contraseña para restaurar sus datos: ";
                    string contrasenia;
                    cin >> contrasenia;
                    if(contrasenia != arreglo_jugadores[i].contrasenia){
                        cout <<"Contraseña incorrecta, vuelva a darse de alta" <<endl;
                        return;
                    }
                    else {
                        arreglo_jugadores[cant_jugadores] = dados_de_baja[i];    
                        cant_jugadores ++;
                        return;
                    }
                }
                else return;
            }
        arreglo_jugadores[cant_jugadores].nickName = nick;

        //Cedula
        cout <<"Ingrese ci en el formato n.nnn.nnn-n: ";
        string cedula;
        cin >> cedula;
        bool cedula_repetida = false;
        for(int i = 0; i < cant_jugadores; i++)
            if(arreglo_jugadores[i].ci == cedula) {
                cedula_repetida = true;
                break;
            }
        while(cedula_repetida || ci_valida(cedula) == false){
            if(cedula_repetida){
                bool cedula_repetida = false;
                cout <<"Esta cedula ya está en uso, vuelva a ingresar: ";
                for(int i = 0; i < cant_jugadores; i++)
                    if(arreglo_jugadores[i].ci == cedula) {
                        cedula_repetida = true;
                        break;
                    }
            }
            if(ci_valida(cedula) == false)
                cout <<"Cedula no válida, vuelva a ingresar: ";
            cin >> cedula;
        }
        arreglo_jugadores[cant_jugadores].ci = cedula;

        //Fecha
        cout<<"Ingrese fecha de nacimiento"<<endl;
        cout <<"Día: ";
        cin >>arreglo_jugadores[cant_jugadores].fecha_nacimiento.dia;
        cout <<"Mes: ";
        cin >>arreglo_jugadores[cant_jugadores].fecha_nacimiento.mes;
        cout <<"Año: ";
        cin >>arreglo_jugadores[cant_jugadores].fecha_nacimiento.anio;
        while(!fecha_valida(arreglo_jugadores, cant_jugadores)){
            cout <<"Fecha no valida, vuelva a ingresar" <<endl;
            cout <<"Dia: ";
            cin >>arreglo_jugadores[cant_jugadores].fecha_nacimiento.dia;
            cout <<"Mes: ";
            cin >>arreglo_jugadores[cant_jugadores].fecha_nacimiento.mes;
            cout <<"Año: ";
            cin >>arreglo_jugadores[cant_jugadores].fecha_nacimiento.anio;
        }

        //Nombre, apellido y contraseña
        cout <<"Ingrese nombre: ";
        cin >> arreglo_jugadores[cant_jugadores].nombre;
        cout <<"Ingrese apellido: ";
        cin >> arreglo_jugadores[cant_jugadores].apellido;
        cout <<"Ingrese contraseña: ";
        cin >> arreglo_jugadores[cant_jugadores].contrasenia;
        cout <<endl <<endl;
        cant_jugadores ++;
    }
    else cout<<"Ya se alcanzó el maximo de jugadores" <<endl<<endl;
}
void Baja_de_jugador(){
    string nick, clave;
    cout <<"Ingrese nickName" <<endl;
    cin >> nick;
    cout <<"contrasenia" <<endl;
    cin >> clave;
    for(int i = 0; i < cant_jugadores; i++)
        if(arreglo_jugadores[i].nickName == nick && arreglo_jugadores[i].contrasenia == clave){
            dados_de_baja[i] = arreglo_jugadores[i];
            for(int j = i; j < cant_jugadores -1; j++)
                arreglo_jugadores[j] = arreglo_jugadores[j+1];
            cant_jugadores --;
            cout << nick <<" fue dado de baja exitosamente"<<endl<<endl;
            return;
        }
    }
void Modificacion_de_jugador(){
    string nick, clave;
    cout << "Ingrese nickName" << endl;
    cin >> nick;
    cout << "contrasenia" << endl;
    cin >> clave;
    for(int i = 0; i < cant_jugadores; i++)
        if(arreglo_jugadores[i].nickName == nick && arreglo_jugadores[i].contrasenia == clave) {
            cout << "Ingrese nuevo nick" << endl;
            cin >> arreglo_jugadores[i].nickName;
            cout << "Ingrese nueva contrasenia" << endl;
            cin >> arreglo_jugadores[i].contrasenia;  
        }
}
void Listado_de_jugadores(){
    for(int i = 0; i <cant_jugadores; i++){
        cout <<"Nickname: " <<arreglo_jugadores[i].nickName <<endl;
        cout <<"Nombre: " <<arreglo_jugadores[i].nombre <<endl;
        cout <<"Apellido: " <<arreglo_jugadores[i].apellido <<endl;
        cout <<"Cedula: " <<arreglo_jugadores[i].ci <<endl;
        cout <<"Cantidad de partidas ganadas: " <<arreglo_jugadores[i].cant_partidas_ganadas <<endl<<endl;
    }
}
void Listado_de_todas_las_partidas(){
    for(int i = 0; i < cant_partidas; i++){
        cout << "Fecha de partida (dia/hora/minuto): ";
        cout << historial_partidas[i].fecha_partida.dia <<"/";
        cout << historial_partidas[i].fecha_partida.hora <<"/";
        cout << historial_partidas[i].fecha_partida.minuto <<endl;
        cout << "Nickname: " << historial_partidas[i].nickName<<endl;
        cout << "Resultado de la partida: " <<historial_partidas[i].resultado_partida <<endl<<endl;
    }
}
void Listado_de_partidas_por_jugador(){
    string nick;
    cout << "Ingrese Nickname a consultar: ";
    cin >> nick;
    system("clear");
    bool encontro_nick = false;
    for(int i = 0; i < cant_partidas; i++)
        if(nick == historial_partidas[i].nickName){
            encontro_nick = true;
            cout << "Fecha de partida (dia/hora/minuto): ";
            cout << historial_partidas[i].fecha_partida.dia <<"/";
            cout << historial_partidas[i].fecha_partida.hora <<"/";
            cout << historial_partidas[i].fecha_partida.minuto <<endl;
            cout << "Nickname: " << historial_partidas[i].nickName<<endl;
            cout << "Resultado de la partida: " <<historial_partidas[i].resultado_partida <<endl<<endl;
        }
    if(!encontro_nick)
        cout << "No se encontro el nick" <<endl<<endl;
}
void Listado_de_partidiatidas_por_fecha(){
    Fecha f;
    cout << "Ingrese Fecha a consultar: ";
    cout <<"Día: ";
    cin >> f.dia;
    cout << "Hora: ";
    cin >> f.hora;
    cout << "Minuto: ";
    cin >> f.minuto;
    system("clear");
    bool encontro_fecha = false;
    for(int i = 0; i < MAX_PARTIDAS_TOTALES; i++)
        if(f.dia == historial_partidas[i].fecha_partida.dia && f.hora == historial_partidas[i].fecha_partida.hora && f.minuto == historial_partidas[i].fecha_partida.minuto){
            encontro_fecha = true;
            cout << "Fecha de partida (dia/hora/minuto): ";
            cout << historial_partidas[i].fecha_partida.dia <<"/";
            cout << historial_partidas[i].fecha_partida.hora <<"/";
            cout << historial_partidas[i].fecha_partida.minuto <<endl;
            cout << "Nickname: " << historial_partidas[i].nickName<<endl;
            cout << "Resultado de la partida: " <<historial_partidas[i].resultado_partida <<endl<<endl;
        }
    if(!encontro_fecha)
        cout << "No se encontro la fecha" <<endl<<endl;
}
void opcion_menu(){
    int opcion_principal = menu_principal();
    system("clear");
    if(opcion_principal == 0)
        return;
    if(opcion_principal == 1){
        int opcion_jugador = menu_jugador();
        if(opcion_jugador == 1) Alta_de_jugador();
        if(opcion_jugador == 2) Baja_de_jugador();
        if(opcion_jugador == 3) Modificacion_de_jugador();
        opcion_menu();
    }
    if(opcion_principal == 2){
        int opcion_consulta = menu_consultas();
        if(opcion_consulta == 1) Listado_de_jugadores();
        if(opcion_consulta == 2) Listado_de_todas_las_partidas();
        if(opcion_consulta == 3) Listado_de_partidas_por_jugador();
        if(opcion_consulta == 4) Listado_de_partidiatidas_por_fecha();
        opcion_menu();
    }
    if(opcion_principal == 3){
        pre_juego();
        opcion_menu();  
    }
}
bool ci_valida(string cedula){
    int longitud = cedula.length();
    if(longitud != 11)
        return false;
    if(cedula[1] != cedula[5] && cedula[1] != '.')
        return false;
    if(cedula[9] != '-')
        return false;
    for(int i = 0; i < longitud; i++)
        if(i != 1 && i != 5 && i != 9)
            if(!isdigit(cedula[i]))
                return false;
    return true;
}
bool fecha_valida(Datos_del_jugador arr[], int i){
    if(arr[i].fecha_nacimiento.dia > 31 || arr[i].fecha_nacimiento.dia < 0)
        if(arr[i].fecha_nacimiento.mes < 0 || arr[i].fecha_nacimiento.mes > 12) 
            if(arr[i].fecha_nacimiento.anio < 1900 || arr[i].fecha_nacimiento.anio > 2024)
                return false;
    return true;
}
void jugada(char arr[3]){
    cout << "Jugadas disponibles" <<endl <<"(E)xplorar" <<endl <<"(M)arcar" <<endl <<"(R)endirse"<<endl;
    char a, b, c;
    cin >> arr[0];
    if(arr[0] != 'R') cin>> arr[1] >> arr[2];
    cout <<endl;
    if ((arr[0]!='E'&& arr[0]!='M' && arr[0] != 'R')||(arr[1]<'0'||arr[1]>'7'||arr[2]<'0'||arr[2]>'7')){
        cout << "Jugada incorrecta" <<endl <<"Vuelva a ingresar" <<endl<<endl<<endl;
        jugada(arr);
    }
}
void imprimir_portada(){
    system("clear");
    cout <<
    " ___                             _"                    <<endl<<           
    "| _ ) _  _  ___ __  __ _  _ __  (_) _ _   __ _  ___"   <<endl<<
    "| _ || || |(_-// _|/ _` || '  | | || ' | / _` |(_-/"   <<endl<<
    "|___/|_.__|/__/\\__||__/_||_|_|_||_||_||_||__/_|/__/"   <<endl <<endl <<endl;
}
void imprimir_matriz(char matriz[8][8]){
    int coord = 0;
    cout << "0 1 2 3 4 5 6 7" <<endl <<endl;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j ++){
            cout << matriz[i][j] << " ";
        }
        cout <<"  " <<coord << endl;
        coord ++;
    }
    coord = 0;
    cout <<endl;
}
void calcular_numeros_adyacentes (char tablero[8][8], char adyacentes[8][8]){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j ++){
            int contador_Bombas = 0;
            if (tablero[i][j]!='*'){
                for (int x= -1; x<=1; x++){
                    for (int y = -1; y<= 1; y++){
                        if (i+x>=0 && i+x<8 && j+y>=0 && j+y<8){
                            if (tablero[i+x][j+y] == '*')
                                contador_Bombas++;
                        }
                    }
                }
                adyacentes[i][j]=contador_Bombas + '0';
                if (contador_Bombas == 0)
                    adyacentes[i][j] = ' ';
            } else adyacentes[i][j] = '*';
        }
    }
}
void explorar (char arr[3], char tablero[8][8], char adyacentes[8][8], char juego[8][8], int & cont_exploradas){
    int i = arr[1] - '0';
    int j = arr[2] - '0';
    if(juego[i][j] != '>'){
        if(adyacentes[i][j] != ' ') cont_exploradas++;
        if(adyacentes[i][j] == ' ' && tablero[i][j] != '*'){
            for (int x= -1; x<=1; x++)
                for (int y = -1; y<= 1; y++){
                    if (i+x>=0 && i+x<8 && j+y>=0 && j+y<8){
                        if(juego[i+x][j+y] != adyacentes[i+x][j+y]){
                            juego[i+x][j+y] = adyacentes[i+x][j+y];
                            if(adyacentes[i+x][j+y] == ' '){
                                cont_exploradas++;
                                arr[1] = i + x + '0';
                                arr[2] = j + y + '0';
                                explorar(arr, tablero, adyacentes, juego, cont_exploradas);
                            }
                            else cont_exploradas++;
                        }
                    }
            }
        }
        juego[i][j] = adyacentes [i][j];
    }
}
void marcar(char arr[3], char juego[8][8], int & casillas_marcadas){
    int i = arr[1] - '0';
    int j = arr[2] - '0';
    if (juego[i][j] == '>') //Desmarcar
        juego[i][j] = '#'; 
    else if(juego[i][j] == '#' && casillas_marcadas < CANT_BOMBAS){//Marcar
        juego[i][j] = '>';
        casillas_marcadas ++;
    }
}