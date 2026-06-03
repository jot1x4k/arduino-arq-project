#include "globals.h"

float temperatura = 0;
short hall = 0;
short luz = 0;
short sonido = 0;

bool claveCorrecta = false;
bool sistemaBloqueado = false;
bool botonPresionado = false;
bool intruso = false;
bool alarma = false;
int EEAddress = 0;

int TEMP_HIGH = 20;
int LIGHT_HIGH = 150;

short contadorPuertas = 0;
short contadorAlarmas = 0;
short contadorKeypad = 0;
char tecla = 0;
char entradaKeypad[4];
unsigned short aciertos = 0;
unsigned short intentos = 0;

unsigned short puertaServo = 0;

Perfil perfil;

char claveKeypad[4] = {'1', '2', '3', 'A'};
byte validKeyRFID[4] = { 0x3A, 0xD4, 0xA8, 0x00 }; 

Servo myservo;

short current_id = 0;

/**
 * @brief Crea un nuevo perfil de usuario y lo almacena en la EEPROM.
 * @param clave Clave del usuario.
 * @param rfid Código RFID del usuario.
 * @param horario Horario permitido del usuario.
 */
void crearPerfil(const char clave[4], const byte rfid[4], const unsigned short horario[2]) {
    Perfil p;
    p.id = current_id;
    snprintf(p.nombre, sizeof(p.nombre), "Usuario%d", current_id);
    memcpy(p.clave, clave, 4);
    memcpy(p.rfid, rfid, 4);
    memcpy(p.horario, horario, 2 * sizeof(unsigned short));
    
    int address = EEPROM_PROFILES_OFFSET + (current_id * sizeof(Perfil));
    EEPROM.put(address, p);
    current_id++;
    EEPROM.put(EEPROM_CURRENT_ID_OFFSET, current_id);
}