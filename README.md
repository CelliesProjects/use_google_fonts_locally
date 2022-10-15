# How to host Google fonts from your ESP32 with ESPAsyncWebServer

In this example we are going to serve the Google Roboto font from your esp32 instead of through the Google API. This means downloading the font files, renaming them, converting to C-style header files and finally use them with ESPAsynWebServer.

A simple example sketch on how to set up the HTML files and [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) is included.

## Steps needed

1. Locate the font on [https://fonts.google.com/](https://fonts.google.com/). 

2. You will need to download the font through the Google font API which can be found at `https://fonts.googleapis.com/css2`.<br>
See [https://developers.google.com/fonts/docs/css2](https://developers.google.com/fonts/docs/css2). No account is needed to download the font files.

3. Open the link [https://fonts.googleapis.com/css2?family=Roboto](https://fonts.googleapis.com/css2?family=Roboto) in your browser and locate your local font variants. 
In this case `latin` and `latin-ext`. (Western alphabet and western accented letters) 

4. Download the files linked to in the line `src: url()` to the folder where your sketch is.
These are for Roboto `https://fonts.gstatic.com/s/roboto/v30/KFOmCnqEu92Fr1Mu7GxKOzY.woff2` for latin-ext and `https://fonts.gstatic.com/s/roboto/v30/KFOmCnqEu92Fr1Mu4mxK.woff2` for latin.

5. Rename the files to some easier names to remember, I used `Roboto-latin.woff2` and `Roboto-latin-ext.woff2`.

6. Copy the complete CSS `@font-face{}` section containg the desired variants to the CSS section of the HTML file hosted on the ESP32. Change the `src: url` lines like below:
```css
/* latin-ext */
@font-face {
  font-family: 'Roboto';
  font-style: normal;
  font-weight: 400;
  src: url(/Roboto-latin-ext.woff2) format('woff2');
  unicode-range: U+0100-024F, U+0259, U+1E00-1EFF, U+2020, U+20A0-20AB, U+20AD-20CF, U+2113, U+2C60-2C7F, U+A720-A7FF;
}
/* latin */
@font-face {
  font-family: 'Roboto';
  font-style: normal;
  font-weight: 400;
  src: url(/Roboto-latin.woff2) format('woff2');
  unicode-range: U+0000-00FF, U+0131, U+0152-0153, U+02BB-02BC, U+02C6, U+02DA, U+02DC, U+2000-206F, U+2074, U+20AC, U+2122, U+2191, U+2193, U+2212, U+2215, U+FEFF, U+FFFD;
}
```

7. To be able to use the font files in your project you have to convert the font files to C-style header files with `xxd`:  
`xxd -i inputfile > outputfile`.<br>  
This will become `xxd -i Roboto-latin.woff2 > Roboto-latin.h` for the `latin` file and `xxd -i Roboto-latin-ext.woff2 > Roboto-latin-ext.h` for the `latin-ext` file.  
This works on most Linux installs, if you are using Windows you have to find an alternative to `xxd`.

8. In the generated files, change the first line to `const unsigned char Roboto_latin_woff2[] ...` and the last line to `const unsigned int Roboto_latin_woff2_len ...;`   
Because if you don't declare the array as a `const`, it will end up in RAM memory instead of in FLASH memory.  
You will have to do this for all files hosted on your ESP32. (`index.htm` and both `*.woff2` files)

9. Include `Roboto-latin.h` and `Roboto-latin-ext.h` in your sketch. See below.

10. Set up ASyncWebServer to use the `Content-Type` header `application/x-font-woff2` when serving these files.<br>
Use the [`request->beginResponse_P()`](https://github.com/me-no-dev/ESPAsyncWebServer#send-binary-content-from-progmem) function to serve data from flash memory.
```c++
#include "index_htm.h"
#include "Roboto-latin.h"
#include "Roboto-latin-ext.h"
...
...
void setup() {
    ...
    ...
    static AsyncWebServer http_server(80);
    static const char* CONTENT_TYPE_HTML {"text/html"};
    static const char* CONTENT_TYPE_WOFF2 {"application/x-font-woff2"};

    http_server.on("/", HTTP_GET, [](AsyncWebServerRequest * const request) {
        AsyncWebServerResponse* const response = request->beginResponse_P(200, CONTENT_TYPE_HTML, index_htm, index_htm_len);
        request->send(response);
    });

    http_server.on("/Roboto-latin-ext.woff2", HTTP_GET, [](AsyncWebServerRequest * const request) {
        AsyncWebServerResponse* const response = request->beginResponse_P(200, CONTENT_TYPE_WOFF2, Roboto_latin_ext_woff2, Roboto_latin_ext_woff2_len);
        request->send(response);
    });

    http_server.on("/Roboto-latin.woff2", HTTP_GET, [](AsyncWebServerRequest * const request) {
        AsyncWebServerResponse* const response = request->beginResponse_P(200, CONTENT_TYPE_WOFF2, Roboto_latin_woff2, Roboto_latin_woff2_len);
        request->send(response);
    });
...
    ...
```

## That's it

Included are the files used in this example. The example sketch will set up a simple webserver and show some text in the locally hosted `Roboto` font.
