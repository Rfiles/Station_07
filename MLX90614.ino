#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

uint8_t mlx_module_num;

bool mlx_boot ( uint8_t mybootnum ) {
    mlx_module_num = mybootnum;
    tcaselect(TCA_MLX);

    if (!mlx.begin()) return false;
    return true;

}

void mlx_read () {
    if (!ModuleData[mlx_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_MLX);
    SensorData.mlxData.Ambient   = mlx.readAmbientTempC();
    SensorData.mlxData.Object    = mlx.readObjectTempC();
}

void mlx_print () {
    printf( "MLX: Amb[%0.2f] Obj[%0.2f]\n", 
      SensorData.mlxData.Ambient,
      SensorData.mlxData.Object  );
}
