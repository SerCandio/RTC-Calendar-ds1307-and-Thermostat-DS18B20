#include "pcf8574.h"


extern bool pcf8574_SendData(uint8_t i2cSlaveAddr, uint8_t dataByte){
     /*Iniciar Bus I2C : START*/
    I2C_StartMaster();
    /*Enviar la direccion del esclavo I2C en modo WRITE*/
    if(I2C_WriteMaster(i2cSlaveAddr) ==true){
        return true;       //Fallo Bus I2C !!
    }
    /*Enviar el byte de data I2C*/
    if(I2C_WriteMaster(dataByte) ==true){
        return true;       //Fallo Bus I2C !!
    }
    /*Detener bus I2C : STOP*/
    I2C_StopMaster();
    
    return false ; //I2C OK !!
}
