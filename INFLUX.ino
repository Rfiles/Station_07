#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
// #define INFLUXDB_URL "http://raspberrypi:8086"
// #define INFLUXDB_TOKEN "ZeTylAat802zasqSa84xA6qPLJmxWDtD5Dx1zIv1hg8KUpZVhQBg2OYkCQHnJdu5TnLCtqWZIfLFWLGlUZ5WWQ=="
// #define INFLUXDB_ORG "Rfiles"
// #define INFLUXDB_BUCKET "sensors"
InfluxDBClient clientinflux(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensors("esp32");
#define WRITE_PRECISION WritePrecision::MS
#define MAX_BATCH_SIZE 1
#define WRITE_BUFFER_SIZE 5


void influx () {

  if ( !connectinflux ) return;
  
  if (!timeClient.isTimeSet())  {
    printf( "INFLUX: Time not set. Abort\n" );
    return;
  }

  // Add tags
  sensors.clearFields();
  sensors.addTag("device", "ESP32");
  sensors.addField("BMET", SensorData.BMEData.temperature);
  sensors.addField("BMEP", SensorData.BMEData.pressure);
  sensors.addField("BMEH", SensorData.BMEData.humidity);
  sensors.setTime(timeClient.getEpochTime());  //set the time

  
  // Print what are we exactly writing
  Serial.print("INFLUX: ");
  Serial.println(clientinflux.pointToLineProtocol(sensors));

  // Write point
  if ( !clientinflux.writePoint(sensors) ) {
    Serial.print("INFLUX: write failed: ");
    Serial.println(clientinflux.getLastErrorMessage());
  }

  //Serial.println("Flushing data into InfluxDB");
  if (!clientinflux.flushBuffer()) {
    Serial.print("InfluxDB flush failed: ");
    Serial.println(clientinflux.getLastErrorMessage());
    Serial.print("Full buffer: ");
    Serial.println(clientinflux.isBufferFull() ? "Yes" : "No");
  }


}

bool connectinflux () {

  if ( !connectwifi() ) {
    Serial.println("connectinflux: No Wifi");
    return false;
  }
  if (!clientinflux.validateConnection()) {
    Serial.print("connectinflux: Server Fail: ");
    Serial.println(clientinflux.getLastErrorMessage());
    return false;
  }    
    
  //clientinflux.setWriteOptions(WriteOptions().writePrecision(WRITE_PRECISION).batchSize(MAX_BATCH_SIZE).bufferSize(WRITE_BUFFER_SIZE));
  return true;
}




