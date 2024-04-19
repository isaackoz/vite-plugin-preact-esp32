#include <WiFi.h>
#include <WebServer.h>
#include "static_files.h"

WebServer server(80);

const char *ssid = "Based_2.0";
const char *password = "practicetude123";

void setup()
{
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  delay(100);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // This is optional. It defines the default entry point at /. So when a user enters example.com/, they are served the index.html
  server.on("/", []
            {
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", (const char *)static_files::f_index_html_contents, static_files::f_index_html_size); });

  // Create your api routes here
  server.on("/api/hello", []
            { server.send(200, "text/html", "Success!"); });

  // Finally, create a route for each of the build artifacts.
  // If you look in the static_files.h, at the bottom you will see the files[].
  // In there, each file has a .path.
  // This creates a route for each path, so if you have test.html, you would be able to access it at example.com/test
  // Or if you have test.png, you would be able to access it at example.com/test.png
  for (int i = 0; i < static_files::num_of_files; i++)
  {
    server.on(static_files::files[i].path, [i]
              {
      server.sendHeader("Content-Encoding", "gzip");
      server.send_P(200, static_files::files[i].type, (const char *)static_files::files[i].contents, static_files::files[i].size); });
  }
  server.begin();
}

void loop()
{
  server.handleClient();
}